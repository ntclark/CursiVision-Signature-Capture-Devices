/// @file      WacomGSS/STU/Tablet_OpenSSL.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2012-09-07
/// @brief     A higher level interface to provide simplified access.

#ifndef WacomGSS_STU_Tablet_OpenSSL_hpp
#define WacomGSS_STU_Tablet_OpenSSL_hpp

#include <WacomGSS/STU/Tablet.hpp>
#include <WacomGSS/OpenSSL.hpp>

namespace WacomGSS
{
  namespace STU
  {

    class OpenSSL_EncryptionHandler : public EncryptionHandler
    {
      OpenSSL::DH       m_dh;
      OpenSSL::AES_KEY  m_key;

    public:
      void                reset() override;
      void                clearKeys() override;
      bool                requireDH() override;
      void                setDH(Protocol::DHprime const & dhPrime, Protocol::DHbase const & dhBase) override; 
      Protocol::PublicKey generateHostPublicKey() override;
      void                computeSharedKey(Protocol::PublicKey const & devicePublicKey) override;
      void                decrypt(std::uint8_t data[16]) override;
    };



    class OpenSSL_EncryptionHandler2 : public EncryptionHandler2
    {
      OpenSSL::RSA     m_rsa;
      OpenSSL::AES_KEY m_key;

    public:
      OpenSSL_EncryptionHandler2();
      
      void                      reset() override;
      void                      clearKeys() override;
      void                      getParameters(Protocol::SymmetricKeyType &, Protocol::AsymmetricPaddingType &, Protocol::AsymmetricKeyType &) override;
      std::vector<std::uint8_t> getPublicExponent() override; 
      std::vector<std::uint8_t> generatePublicKey() override; 
      void                      computeSessionKey(std::vector<uint8_t> & data) override;
      void                      decrypt(std::uint8_t data[16]) override;
    };

  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Tablet_OpenSSL_hpp
