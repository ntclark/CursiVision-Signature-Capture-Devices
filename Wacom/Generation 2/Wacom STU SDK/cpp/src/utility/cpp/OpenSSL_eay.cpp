/// @file      OpenSSL.cpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides a thin layer of objects from the OpenSSL library to provide basic exception safety.

#include <WacomGSS/OpenSSL.hpp>

#pragma warning(push, 3)
#include <openssl/dh.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#pragma warning(pop)

namespace WacomGSS
{
  namespace OpenSSL
  {



    class error_category : public std::error_category
    {
    public:
      char const * name() const noexcept override
      {
        return "OpenSSL";
      }

      //error_condition default_error_condition(int ev) const noexcept;

      std::string message(int ev) const override;
    };



    static error_category ec;
    std::error_category const & openssl_error_category() noexcept
    {
      return ec;
    }



    std::string error_category::message(int ev) const
    {
      // http://www.openssl.org/docs/crypto/ERR_error_string.html
      // buf must be at least 120 bytes long.
      const int buffer_size = 256;

      char buffer[buffer_size] = { 0 };
      ::ERR_error_string_n(static_cast<unsigned long>(ev), buffer, buffer_size);

      return std::string(buffer);
    }



    error::error()
    :
      std::system_error(static_cast<int>(ERR_get_error()), openssl_error_category())
    {
    }



    error::error(char const * what_arg)
    :
      std::system_error(static_cast<int>(ERR_get_error()), openssl_error_category(), what_arg)
    {
    }



    error::error(unsigned long err)
    :
      std::system_error(static_cast<int>(err), openssl_error_category())
    {
    }



    error::error(unsigned long err, char const * what_arg)
    :
      std::system_error(static_cast<int>(err), openssl_error_category(), what_arg)
    {
    }



    #define WacomGSS_fr(NAME)\
      static inline :: NAME *          f2r(NAME##_ *           value) { return reinterpret_cast< :: NAME *         >(value      ); } \
      static inline :: NAME const *    f2r(NAME##_ const *     value) { return reinterpret_cast< :: NAME const *   >(value      ); } \
      static inline NAME##_ *          r2f(:: NAME *           value) { return reinterpret_cast< NAME##_ *         >(value      ); } \
      static inline NAME##_ const *    r2f(:: NAME const *     value) { return reinterpret_cast< NAME##_ const *   >(value      ); } 

    #define WacomGSS_o(NAME)\
      static inline :: NAME const *    o2r(NAME const &        value) { return reinterpret_cast< :: NAME const *   >(value.get()); } \
      static inline :: NAME *          o2r(NAME &              value) { return reinterpret_cast< :: NAME *         >(value.get()); }

    #define WacomGSS_fro(NAME) WacomGSS_fr(NAME)  WacomGSS_o(NAME) 

    WacomGSS_fro(BIGNUM)
    WacomGSS_fro(DH)
    WacomGSS_fro(RSA)
    WacomGSS_fro(EVP_PKEY)
    WacomGSS_fro(X509)
    WacomGSS_fro(BIO)

    WacomGSS_fr(BIO_METHOD)
    WacomGSS_fr(asn1_string_st)
    WacomGSS_fr(X509_NAME)
    WacomGSS_fr(EVP_MD)
    WacomGSS_fr(AES_KEY)

    static ::AES_KEY *       o2r(AES_KEY *         value) { return reinterpret_cast< ::AES_KEY *      >(value->get()); }
    static ::AES_KEY const * o2r(AES_KEY const *   value) { return reinterpret_cast< ::AES_KEY const *>(value->get()); }



    void BIGNUM_BN_free_trait::close(BIGNUM_ * value) noexcept
    {
      if (value)
        ::BN_free(f2r(value));
    }



    void BIGNUM_BN_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }



    BIGNUM::BIGNUM()
    :
      base(r2f(::BN_new()))
    {
    }



    BIGNUM::BIGNUM(BIGNUM_ * ref)
    :
      base(ref)
    {
    }



    void DH_DH_free_trait::close(DH_ * value) noexcept
    {
      if (value) 
      {
        ::DH_free(f2r(value));
      }
    }



    void DH_DH_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }



    void RSA_RSA_free_trait::close(RSA_ * value) noexcept
    {
      if (value) 
      {
        ::RSA_free(f2r(value));
      }
    }



    void RSA_RSA_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }



    AES_KEY::AES_KEY()
    :
      unique_ptr<AES_KEY_>(r2f(new ::AES_KEY))
    {
      std::memset(get(), 0x00, sizeof(::AES_KEY));
    }



    AES_KEY::~AES_KEY()
    {      
      if (auto p = get())
      {
        memset_s(p, 0x00, sizeof(::AES_KEY)); // use secure memset
      }      
    }



    DH::DH()
    :
      base(r2f(::DH_new()))      
    {
    }



    EVP_PKEY::EVP_PKEY()
    :
      base(r2f(::EVP_PKEY_new()))
    {
    }



    void EVP_PKEY_free_trait::close(EVP_PKEY_ * value) noexcept
    {
      if (value)
      {
        ::EVP_PKEY_free(f2r(value));
      }
    }



    void EVP_PKEY_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }



    X509::X509()
      :
      base(r2f(::X509_new()))
    {
    }


    X509::X509(X509_ * takeOwnership)
      :
      base(takeOwnership)
    {
    }



    void X509_free_trait::close(X509_ * value) noexcept
    {
      if (value)
      {
        ::X509_free(f2r(value));
      }
    }



    void X509_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }


	BIGNUM_ const * DH_get0_p(DH const & dh)
	{
	  return r2f(::DH_get0_p(f2r(dh.get())));
	}

	BIGNUM_ const * DH_get0_q(DH const & dh)
	{
		return r2f(::DH_get0_q(f2r(dh.get())));
	}
	BIGNUM_ const * DH_get0_g(DH const & dh)
	{
		return r2f(::DH_get0_g(f2r(dh.get())));
	}
	BIGNUM_ const * DH_get0_pub_key(DH const & dh)
	{
		return r2f(::DH_get0_pub_key(f2r(dh.get())));
	}

	BIGNUM_ const * DH_get0_priv_key(DH const & dh)
	{
      return r2f(::DH_get0_priv_key(f2r(dh.get())));
	}
	int DH_set0_pqg(DH & dh, BIGNUM && p, nullptr_t, BIGNUM && g)
	{
		return ::DH_set0_pqg(f2r(dh.get()), f2r(p.release()), nullptr, f2r(g.release()));
	}
	int DH_set0_pqg(DH & dh, nullptr_t, nullptr_t, nullptr_t)
	{
		return ::DH_set0_pqg(f2r(dh.get()), nullptr, nullptr, nullptr);
	}
	int DH_set0_key(DH & dh, BIGNUM && pub_key, BIGNUM && priv_key)
	{
		return ::DH_set0_key(f2r(dh.get()), f2r(pub_key.release()), f2r(priv_key.release()));
	}
	int DH_set0_key(DH & dh, nullptr_t, nullptr_t)
	{
		return ::DH_set0_key(f2r(dh.get()), nullptr, nullptr);
	}


    int DH_generate_key(DH & dh)
    {
      return ::DH_generate_key(o2r(dh));
    }



    size_t DH_compute_key(unsigned char * key, BIGNUM const & pub_key, DH & dh)
    {
      int ret = ::DH_compute_key(key, o2r(pub_key), o2r(dh));
      return static_cast<size_t>(ret);
    }



    int DH_check(DH & dh, int * codes)
    {
      return ::DH_check(o2r(dh), codes);
    }



    BIGNUM BN_bin2bn(unsigned char const * s, size_t len, std::nullptr_t)
    {
      return BIGNUM(r2f(::BN_bin2bn(s, static_cast<int>(len), nullptr)));
    }

	   
    size_t BN_bn2bin(BIGNUM_ const * a, unsigned char * to)
	{
		int ret = ::BN_bn2bin(f2r(a), to);
		return static_cast<size_t>(ret);
	}


    size_t BN_bn2bin(BIGNUM const & a, unsigned char * to)
    {
      int ret = ::BN_bn2bin(o2r(a), to);
      return static_cast<size_t>(ret);
    }



    size_t (BN_num_bytes)(BIGNUM_ const * a)
	{
		int ret = BN_num_bytes(f2r(a));
		return static_cast<size_t>(ret);
	}


    size_t (BN_num_bytes)(BIGNUM const & a)
    {
      int ret = BN_num_bytes(o2r(a));
      return static_cast<size_t>(ret);
    }



    int BN_set_word(BIGNUM & a, unsigned long w)
    {
      return ::BN_set_word(o2r(a), w);
    }



    //int BN_set_word(BIGNUM_ * a, unsigned long w)
    //{
    //  return ::BN_set_word(f2r(a), w);
   // }



    int AES_set_decrypt_key(unsigned char const * userKey, const int bits, AES_KEY * key)
    {
      return ::AES_set_decrypt_key(userKey, bits, o2r(key));
    }



    void AES_decrypt(unsigned char const * in, unsigned char * out, AES_KEY const * key)
    {
      ::AES_decrypt(in, out, o2r(key));
    }



    int RAND_pseudo_bytes(unsigned char * buf, int num)
    {
      return ::RAND_pseudo_bytes(buf, num);
    }



    RSA::RSA()
    :
      base(r2f(::RSA_new()))
    {
    }



    int RSA_generate_key_ex(RSA & rsa, int bits, BIGNUM & e, std::nullptr_t)
    {
      return ::RSA_generate_key_ex(o2r(rsa), bits, o2r(e), nullptr);
    }


	int RSA_set0_key(RSA & r, nullptr_t, nullptr_t, nullptr_t)
	{
		return ::RSA_set0_key(o2r(r), nullptr, nullptr, nullptr);
	}

	BIGNUM_ const * RSA_get0_n(RSA const & d)
	{
		return r2f(::RSA_get0_n(o2r(d)));
	}

    size_t RSA_size(RSA const & rsa)
    {
      int ret = ::RSA_size(o2r(rsa));
      return static_cast<size_t>(ret);
    }



    size_t RSA_private_decrypt(size_t flen, unsigned char const * in, unsigned char * to, RSA & rsa, int padding)
    {
      int flen_ = static_cast<int>(flen);
      int ret = ::RSA_private_decrypt(flen_, in, to, o2r(rsa), padding);
      return static_cast<size_t>(ret);
    }



    int (EVP_PKEY_assign_RSA)(EVP_PKEY & pkey, RSA && rsa)
    {
      int ret = ::EVP_PKEY_assign_RSA(o2r(pkey), o2r(rsa));
      rsa.release(); // pkey takes ownership
      return ret;
    }



    int X509_set_version(X509 & x509, long version)
    {
      return ::X509_set_version(o2r(x509), version);
    }



    ASN1_INTEGER_ * X509_get_serialNumber(X509 & x509)
    {
      return r2f(::X509_get_serialNumber(o2r(x509)));
    }


#pragma push_macro("X509_get_notBefore")
#undef X509_get_notBefore
    ASN1_TIME_ * (X509_get_notBefore)(X509 & x509)
    {
#pragma pop_macro("X509_get_notBefore")
      return r2f(X509_get_notBefore(o2r(x509))); // note: macro, not function
    }


#pragma push_macro("X509_get_notAfter")
#undef X509_get_notAfter
    ASN1_TIME_ * (X509_get_notAfter)(X509 & x509)
    {
#pragma pop_macro("X509_get_notAfter")
      return r2f(X509_get_notAfter(o2r(x509))); // note: macro, not function
    }



    ASN1_TIME_ * X509_time_adj_ex(ASN1_TIME_ * s, int offset_day, long offset_sec, time_t * in_tm)
    {
      return r2f(::X509_time_adj_ex(f2r(s), offset_day, offset_sec, in_tm));
    }



    int X509_set_pubkey(X509 & x509, EVP_PKEY & pkey)
    {
      return ::X509_set_pubkey(o2r(x509), o2r(pkey));
    }



    int ASN1_INTEGER_set(ASN1_INTEGER_ * a, long v)
    {
      return ::ASN1_INTEGER_set(f2r(a), v);
    }



    X509_NAME_ * X509_get_subject_name(X509 & x509)
    {
      return r2f(::X509_get_subject_name(o2r(x509)));
    }



    int X509_NAME_add_entry_by_txt(X509_NAME_ *name, const char *field, int type, const unsigned char *bytes, int len, int loc, int set)
    {
      return ::X509_NAME_add_entry_by_txt(f2r(name), field, type, bytes, len, loc, set);
    }



    int X509_set_issuer_name(X509 & x, X509_NAME_ * name)
    {
      return ::X509_set_issuer_name(o2r(x), f2r(name));
    }



    EVP_MD_ const * EVP_sha256()
    {
      return r2f(::EVP_sha256());
    }



    int X509_sign(X509 & x, EVP_PKEY & pkey, EVP_MD_ const *md)
    {
      return ::X509_sign(o2r(x), o2r(pkey), f2r(md));
    }



    void BIO_BIO_free_trait::close(BIO_ * value) noexcept
    {
      ::BIO_free(f2r(value));
    }



    void BIO_BIO_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }



    BIO::BIO(BIO_METHOD_ const * method)
    :
      base(r2f(::BIO_new(f2r(method))))
    {
    }



    BIO_METHOD_ const * BIO_s_null()
    {
      return r2f(::BIO_s_null());
    }



    void BIO_set_callback_arg(BIO & b, char *arg)
    {
      ::BIO_set_callback_arg(o2r(b), arg);
    }

	char * BIO_get_callback_arg(BIO_ const * b)
	{
		return ::BIO_get_callback_arg(f2r(b));
	}



    void BIO_set_callback(BIO & b, long (*callback)(BIO_ *, int, const char *, int, long, long))
    {
#pragma warning(suppress: 5039) // pointer or reference to potentially throwing function passed to extern C function
      ::BIO_set_callback(o2r(b), reinterpret_cast<long (*)(::BIO *, int, const char *, int, long, long)>(callback));
    }



    //char * & BIO_::cb_arg_()
    //{
    //  return f2r(this)->cb_arg;
    //}



  } // namespace OpenSSL
  
} // namespace WacomGSS
