#include <WacomGSS/STU/Tablet_OpenSSL.hpp>
#include <limits>

namespace WacomGSS
{
  namespace STU
  {


    void OpenSSL_EncryptionHandler::reset()
    {
      using namespace OpenSSL;

	  DH_set0_pqg(m_dh, nullptr, nullptr, nullptr);
	  DH_set0_key(m_dh, nullptr, nullptr);      

      m_key         = std::move(AES_KEY());
    }


    void OpenSSL_EncryptionHandler::clearKeys()
    {
      using namespace OpenSSL;

	  DH_set0_key(m_dh, nullptr, nullptr);
      m_key         = std::move(AES_KEY());
    }


    bool OpenSSL_EncryptionHandler::requireDH()
    {
		using namespace OpenSSL;
      return !DH_get0_g(m_dh) || !DH_get0_p(m_dh);
    }



    void OpenSSL_EncryptionHandler::setDH(Protocol::DHprime const & dhPrime, Protocol::DHbase const & dhBase)
    {
      using namespace OpenSSL;

      reset();

	  BIGNUM p(BN_bin2bn(dhPrime.data(), dhPrime.size(), nullptr));
      BIGNUM g(BN_bin2bn(dhBase.data() , dhBase.size() , nullptr));

	  if (!p || !g || !DH_set0_pqg(m_dh, std::move(p), nullptr, std::move(g)))
      {
        throw error("BN_bin2bn");
      }
    }



    Protocol::PublicKey OpenSSL_EncryptionHandler::generateHostPublicKey()
    {
      using namespace OpenSSL;

	  auto pub_key(DH_get0_pub_key(m_dh));

      if (!DH_generate_key(m_dh) || !pub_key || !DH_get0_priv_key(m_dh))
      {
        throw error("DH_generate_key");
      }

	  
      Protocol::PublicKey hostPublicKey;

      if (BN_num_bytes(pub_key) != hostPublicKey.size())
      {
        throw error("BN_num_bytes");
      }

      if (BN_bn2bin(pub_key, hostPublicKey.data()) != hostPublicKey.size())
      {
        throw error("BN_bn2bin");
      }

      return std::move(hostPublicKey);
    }



    void OpenSSL_EncryptionHandler::computeSharedKey(Protocol::PublicKey const & devicePublicKey)
    {
      using namespace OpenSSL;

      BIGNUM bnDevicePublicKey(BN_bin2bn(devicePublicKey.data(), devicePublicKey.size(), nullptr)); // convert endian

      Protocol::PublicKey sharedKey;

      size_t r = DH_compute_key(sharedKey.data(), bnDevicePublicKey, m_dh);
      if (r == (std::numeric_limits<size_t>::max)())
        throw error("DH_compute_key");

      if (r != sharedKey.size())
      {
        auto o = Protocol::PublicKey::difference_type(sharedKey.size() - r);
        if (o > 0)
        {
          std::copy_backward(sharedKey.begin(), sharedKey.begin()+Protocol::PublicKey::difference_type(r), sharedKey.end());
          std::fill(sharedKey.begin(), sharedKey.begin()+o, '\0');
        }
        else
        {
          throw std::logic_error("DH_compute_key() should never return a value larger than DH_size(dh)");
        }
      }

      AES_set_decrypt_key(sharedKey.data(), static_cast<int>(sharedKey.size()*8), &m_key);
    }



    void OpenSSL_EncryptionHandler::decrypt(std::uint8_t data[16]) 
    {
      OpenSSL::AES_decrypt(data, data, &m_key);
    }



    OpenSSL_EncryptionHandler2::OpenSSL_EncryptionHandler2()
    {
      using namespace OpenSSL;
    }



    void OpenSSL_EncryptionHandler2::reset()
    {
      using namespace OpenSSL;
      
      if (m_rsa)
      {
		RSA_set0_key(m_rsa, nullptr, nullptr, nullptr);
      }
      m_key   = std::move(AES_KEY());
    }



    void OpenSSL_EncryptionHandler2::clearKeys()
    {
      using namespace OpenSSL;

	  RSA_set0_key(m_rsa, nullptr, nullptr, nullptr);
      
      m_key   = std::move(AES_KEY());
    }

    
    
    void OpenSSL_EncryptionHandler2::getParameters(Protocol::SymmetricKeyType & symmetricKeyType, Protocol::AsymmetricPaddingType & asymmetricPaddingType, Protocol::AsymmetricKeyType & asymmetricKeyType)
    {
      symmetricKeyType      = Protocol::SymmetricKeyType_AES256;
      asymmetricPaddingType = Protocol::AsymmetricPaddingType_OAEP;      
      asymmetricKeyType     = Protocol::AsymmetricKeyType_RSA2048;
    }



    std::vector<std::uint8_t> OpenSSL_EncryptionHandler2::getPublicExponent()
    {
      using namespace OpenSSL;

      BIGNUM e;

      if (BN_set_word(e, RSA_F4) == 0) // =65537
        throw error("BN_set_word(RSA_F4)");

      auto num_bytes = BN_num_bytes(e);

      const auto blockSize = Protocol::EncryptionCommand().data.size();

      if (num_bytes > blockSize)
        throw error("Unexpectedly large size 'e'");

      std::vector<uint8_t> bin(num_bytes);

      if (BN_bn2bin(e, bin.data()) != num_bytes)
        throw error("BN_bn2bin");

      return bin;
    }



    std::vector<std::uint8_t> OpenSSL_EncryptionHandler2::generatePublicKey()
    {
      using namespace OpenSSL;
      
      if (!RSA_get0_n(m_rsa))
      {
        BIGNUM e;

        if (BN_set_word(e, RSA_F4) == 0) // =65537
          throw error("BN_set_word(RSA_F4)");

        int ret = RSA_generate_key_ex(m_rsa, 2048, e, nullptr);

        if (ret != 1)
          throw error("RSA_generate_key_ex");
      }

	  auto n(RSA_get0_n(m_rsa));
      auto num_bytes = BN_num_bytes(n);

      std::vector<uint8_t> bin(num_bytes);

      if (BN_bn2bin(n, bin.data()) != num_bytes)
        throw error("BN_bn2bin");

      return bin;
    }



    void OpenSSL_EncryptionHandler2::computeSessionKey(std::vector<uint8_t> & data)
    {
      using namespace OpenSSL;
    
      std::vector<uint8_t> key(data.size());

      const size_t keySizeBits = 256;
      const size_t keySizeBytes = keySizeBits/8;

      size_t r = RSA_private_decrypt(data.size(), data.data(), key.data(), m_rsa, RSA_PKCS1_OAEP_PADDING);      
      if (r < keySizeBytes || r == size_t(-1))
        throw error("unexpected size of decrypted key");

      auto e = AES_set_decrypt_key(key.data() + r - keySizeBytes, keySizeBits, &m_key);
      if (e != 0)
        throw error("AES_set_decrypt_key");
    }



    void OpenSSL_EncryptionHandler2::decrypt(std::uint8_t data[16]) 
    {
      OpenSSL::AES_decrypt(data, data, &m_key);
    }

  } // namespace STU

} // namespace WacomGSS
