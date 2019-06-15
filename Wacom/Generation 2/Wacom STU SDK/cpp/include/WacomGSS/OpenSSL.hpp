/// @file      WacomGSS/OpenSSL.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides a thin layer of objects from the OpenSSL library to provide basic exception safety.

#ifndef WacomGSS_OpenSSL_hpp
#define WacomGSS_OpenSSL_hpp

#include <WacomGSS/unique_handle.hpp>
#include <system_error>
#include <memory>
#include <cstdio>


namespace WacomGSS
{

  namespace OpenSSL
  {

    class LoaderError : public std::runtime_error
    {
    public:
      LoaderError();
    };

    /// @brief checks to ensure that the OpenSSL libraries are loaded.
    struct Loader
    {
      Loader();
      static bool isLoaded() noexcept;
    };

    std::error_category const & openssl_error_category() noexcept;

    class error : public std::system_error
    {
    public:
      error();
      explicit error(char const * what_arg);
      explicit error(unsigned long err);
      error(unsigned long err, char const * what_arg);
    };
  }

  namespace OpenSSL
  {
    /// @internal
    /// Aliases
    /// @brief These provide type aliases to point OpenSSL equivilents. 
    ///
    /// These types are provided so that a pointer type can be created
    /// without introducing the real OpenSSL types at global namespace level
    /// which can introduce ambiguities in name resolution.
    /// @{
    struct BIGNUM_  { };
    struct DH_      { };
    struct AES_KEY_ { };
    struct RSA_     { };

    struct EVP_PKEY_ {};
    struct X509_ {};    
    struct X509_STORE_CTX_ {};
    struct asn1_string_st_ {};
    typedef asn1_string_st_ ASN1_INTEGER_;
    typedef asn1_string_st_ ASN1_TIME_;

    struct X509_NAME_ {};
    struct EVP_MD_ {};
    struct SSL_CTX_ {};
    struct SSL_METHOD_ {};
    struct SSL_ {};
    struct BIO_ { };
    struct BIO_METHOD_ {};    
    /// @}


    struct BIGNUM_BN_free_trait
    {
      static BIGNUM_ * invalid() noexcept
      {
        return nullptr;
      }
  
      static bool is_valid(BIGNUM_ * value) noexcept
      {
        return value != nullptr;
      }
  
      static void close(BIGNUM_ * value) noexcept;
  
      static void throw_invalid_handle_exception();
    };
    
    /// @brief An exception-safe alias of the OpenSSL %BIGNUM struct.
    class BIGNUM : public unique_handle<BIGNUM_ *,BIGNUM_BN_free_trait>
    {
      typedef unique_handle<BIGNUM_ *,BIGNUM_BN_free_trait> base; 
    
    public:
      BIGNUM();
      BIGNUM(BIGNUM_ * value);

	  BIGNUM_ * && move() noexcept
	  {
		  return release();
	  }
    };



    struct DH_DH_free_trait
    {
      static DH_ * invalid() noexcept
      {
        return nullptr;
      }
  
      static bool is_valid(DH_ * value) noexcept
      {
        return value != nullptr;
      }

      static void close(DH_ * value) noexcept;

      static void throw_invalid_handle_exception();
    };



    struct RSA_RSA_free_trait
    {
      static RSA_ * invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(RSA_ * value) noexcept
      {
        return value != nullptr;
      }

      static void close(RSA_ * value) noexcept;

      static void throw_invalid_handle_exception();
    };



    /// @brief Provides an exception safe use of the OpenSSL ::%DH struct
    ///
    /// Use should be identical to using the real ::%DH object, without
    /// having to be concerned with API failures and null pointers.
    class DH : public unique_handle<DH_ *,DH_DH_free_trait>
    {
      typedef unique_handle<DH_ *,DH_DH_free_trait> base;

    public:
      DH();
    };

    /// @brief Provides an exception safe use of the OpenSSL ::%RSA struct
    ///
    /// Use should be identical to using the real ::%RSA object, without
    /// having to be concerned with API failures and null pointers.
    class RSA : public unique_handle<RSA_ *,RSA_RSA_free_trait>
    {
      typedef unique_handle<RSA_ *,RSA_RSA_free_trait> base;

    public:
      RSA();
    };



    /// @brief Provides an exception safe use of the OpenSSL AES_KEY object
    ///
    /// Use should be identical to using the real AES_KEY object, with safe (null)
    /// construction and safe deleting of the key memory at destruction.
    class AES_KEY : public std::unique_ptr<AES_KEY_>
    {
    public:
      /// @brief initializes the key to zero.
      AES_KEY();

      /// @brief securely deletes the key.
      ~AES_KEY();

      AES_KEY & operator = (AES_KEY && other)
      {
        static_cast<std::unique_ptr<AES_KEY_> &>(*this) = std::move(other);
        return *this;
      }
    };
    /// @}


    struct EVP_PKEY_free_trait
    {
      static EVP_PKEY_ * invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(EVP_PKEY_ * value) noexcept
      {
        return value != nullptr;
      }

      static void close(EVP_PKEY_ * value) noexcept;

      static void throw_invalid_handle_exception();
    };


    struct X509_free_trait
    {
      static X509_ * invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(X509_ * value) noexcept
      {
        return value != nullptr;
      }

      static void close(X509_ * value) noexcept;

      static void throw_invalid_handle_exception();
    };

    class EVP_PKEY : public unique_handle<EVP_PKEY_ *, EVP_PKEY_free_trait>
    {
      typedef unique_handle<EVP_PKEY_ *, EVP_PKEY_free_trait> base;

    public:      
      EVP_PKEY();      
      EVP_PKEY(EVP_PKEY && other) : base(static_cast<base &&>(other)){}
    };


    class X509 : public unique_handle<X509_ *, X509_free_trait>
    {
      typedef unique_handle<X509_ *, X509_free_trait> base;

    public:     
      X509();
      X509(X509 && other) : base(static_cast<base &&>(other)){}
      explicit X509(X509_ * takeOwnership);      
    };


    class SSL_CTX
    {
      struct SSL_CTX_Deleter
      {
        void operator ()(SSL_CTX_ * p);
      };
      std::shared_ptr<SSL_CTX_> m_p;
    public:
      SSL_CTX(SSL_CTX const & o) : m_p(o.m_p) {}
      SSL_CTX(SSL_METHOD_ const * method);

      SSL_CTX_ * get();
      SSL_CTX_ const * get() const;
      void reset(SSL_CTX & o) { m_p = o.m_p; }
    };


    struct SSL_SSL_free_trait
    {
      static SSL_ * invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(SSL_ * value) noexcept
      {
        return value != nullptr;
      }

      static void close(SSL_ * value) noexcept;

      static void throw_invalid_handle_exception();
    };

    class SSL : public unique_handle<SSL_ *, SSL_SSL_free_trait>
    {
      typedef unique_handle<SSL_ *, SSL_SSL_free_trait> base;
      SSL_CTX m_sslCtx;

    public:
      SSL();
      SSL(SSL_CTX & sslCtx);

      void reset(SSL_CTX & sslCtx);
    };


    struct BIO_BIO_free_trait
    {
      static BIO_ * invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(BIO_ * value) noexcept
      {
        return value != nullptr;
      }

      static void close(BIO_ * value) noexcept;

      static void throw_invalid_handle_exception();
    };

    class BIO : public unique_handle<BIO_ *, BIO_BIO_free_trait>
    {
      typedef unique_handle<BIO_ *, BIO_BIO_free_trait> base;

    public:
      BIO(BIO_METHOD_ const * method);
    };

    const int BIO_CB_RETURN  = 0x80;
    const int BIO_CB_FREE    = 0x01;
    const int BIO_CB_READ    = 0x02;
    const int BIO_CB_WRITE   = 0x03;
    const int BIO_CB_PUTS    = 0x04;
    const int BIO_CB_GETS    = 0x05;
    const int BIO_CB_CTRL    = 0x06;

		
	BIGNUM_ const * DH_get0_p(DH const & dh);
	BIGNUM_ const * DH_get0_q(DH const & dh);
	BIGNUM_ const * DH_get0_g(DH const & dh);
	BIGNUM_ const * DH_get0_pub_key(DH const & dh);
	BIGNUM_ const * DH_get0_priv_key(DH const & dh);
	int DH_set0_pqg(DH & dh, BIGNUM && p, nullptr_t, BIGNUM && g);
	int DH_set0_pqg(DH & dh, nullptr_t, nullptr_t, nullptr_t);
	int DH_set0_key(DH & dh, BIGNUM && pub_key, BIGNUM && priv_key);
	int DH_set0_key(DH & dh, nullptr_t, nullptr_t);


    /// @brief Calculdates the public and private keys given the p and g values.
    int DH_generate_key(DH & dh);

    /// @brief Calculates the shared key given the provided public key.
    /// @attention The return type has been changed to size_t to better accomodate C++.
    size_t DH_compute_key(unsigned char * key, BIGNUM const & pub_key, DH & dh); 

    /// @brief Validates Diffie-Hellman parameters.
    /// It checks that p is a safe prime, and that g is a suitable generator.
    /// In the case of an error, the bit flags DH_CHECK_P_NOT_SAFE_PRIME or
    /// DH_NOT_SUITABLE_GENERATOR are set in *codes.
    /// DH_UNABLE_TO_CHECK_GENERATOR is set if the generator cannot be checked,
    /// i.e. it does not equal 2 or 5.
    int DH_check(DH const & dh, int * codes);

    /// @attention Parameter @em len has been changed to size_t to better accomodate C++.
    ///            The third parameter is not supported and must be nullptr.
    BIGNUM BN_bin2bn(unsigned char const * s, size_t len, std::nullptr_t);

    /// @attention The return type has been changed to size_t to better accomodate C++.
    size_t BN_bn2bin(BIGNUM_ const * a, unsigned char * to);
    
    /// @attention The return type has been changed to size_t to better accomodate C++.
    size_t BN_bn2bin(BIGNUM const & a, unsigned char * to);

	/// @attention The return type has been changed to size_t to better accomodate C++.
	size_t (BN_num_bytes)(BIGNUM_ const * a);

    /// @attention The return type has been changed to size_t to better accomodate C++.
    size_t (BN_num_bytes)(BIGNUM const & a);

    /// @brief Initializes the value
    int BN_set_word(BIGNUM & a, unsigned long w);
    
    /// @brief Initializes the decryption key
    int AES_set_decrypt_key(unsigned char const * userKey, const int bits, AES_KEY * key);
    
    /// @brief Decrypts one data block.
    void AES_decrypt(unsigned char const * in, unsigned char * out, AES_KEY const * key);

    /// @brief Generates random data.
    int RAND_pseudo_bytes(unsigned char * buf, int num);



    /// @brief Generates a new random key pair.
    int RSA_generate_key_ex(RSA & rsa, int bits, BIGNUM & e, std::nullptr_t);

    /// @attention The return type has been changed to size_t to better accomodate C++.
    size_t RSA_size(RSA const & rsa);

	BIGNUM_ const * RSA_get0_n(RSA const & d);

	int RSA_set0_key(RSA & r, nullptr_t, nullptr_t, nullptr_t);

    /// @attention The return type and flen have been changed to size_t to better accomodate C++.
    size_t RSA_private_decrypt(size_t flen, unsigned char const * in, unsigned char * to, RSA & rsa, int padding);
    
    const unsigned long RSA_F4 = 0x10001;
    const int           RSA_PKCS1_PADDING      = 1;
    const int           RSA_NO_PADDING         = 3;
    const int           RSA_PKCS1_OAEP_PADDING = 4;



    int EVP_PKEY_assign_RSA(EVP_PKEY & pkey, RSA && rsa);
    
    int X509_set_version(X509 & x509, long version);
    ASN1_INTEGER_ * X509_get_serialNumber(X509 & x509);
    
    ASN1_TIME_ * X509_get_notBefore(X509 & x509);
    ASN1_TIME_ * X509_get_notAfter(X509 & x509);
    ASN1_TIME_ * X509_time_adj_ex(ASN1_TIME_ * s, int offset_day, long offset_sec, time_t * in_tm);
    int X509_set_pubkey(X509 & x509, EVP_PKEY & pkey);

    int ASN1_INTEGER_set(ASN1_INTEGER_ * a, long v);

    X509_NAME_ * X509_get_subject_name(X509 & x509);

    const int MBSTRING_FLAG = 0x1000;
    const int MBSTRING_UTF8 = MBSTRING_FLAG;
    const int MBSTRING_ASC  = MBSTRING_FLAG|1;

    int X509_NAME_add_entry_by_txt(X509_NAME_ *name, const char *field, int type, const unsigned char *bytes, int len, int loc, int set);

    int X509_set_issuer_name(X509 & x, X509_NAME_ * name);
    
    EVP_MD_ const * EVP_sha256();
    int X509_sign(X509 & x, EVP_PKEY & pkey, EVP_MD_ const *md);

    int PEM_write_X509(FILE * fp, X509 & x509);
    int i2d_X509_fp(FILE * fp, X509 & x509);

    int SSL_library_init();

    SSL_METHOD_ const * TLSv1_2_client_method();

    const int SSL_VERIFY_NONE                 = 0x00;
    const int SSL_VERIFY_PEER                 = 0x01;
    const int SSL_VERIFY_FAIL_IF_NO_PEER_CERT = 0x02;
    const int SSL_VERIFY_CLIENT_ONCE          = 0x04;

    int SSL_CTX_use_certificate(SSL_CTX & ctx, X509 & x);
    int SSL_CTX_use_PrivateKey(SSL_CTX & ctx, EVP_PKEY & pkey);
    int SSL_CTX_set_cipher_list(SSL_CTX & ctx, char const * str);
    void SSL_CTX_set_verify(SSL_CTX & ctx, int mode, int (*callback) (int, X509_STORE_CTX_ *));

    BIO_METHOD_ const *BIO_s_null();

    void SSL_set_bio(SSL & s, BIO & rbio, BIO & wbio);
    void BIO_set_callback_arg(BIO & b, char *arg);
	char *BIO_get_callback_arg(BIO_ const * b);
    void BIO_set_callback(BIO & b, long (*callback) (BIO_ *, int, const char *, int, long, long));
	



    int SSL_connect(SSL & ssl);
    int SSL_read(SSL & ssl, void * buf, int num);
    int SSL_write(SSL & ssl, void const * buf, int num);

    X509 SSL_get_peer_certificate(SSL & ssl);

  }


} // namespace WacomGSS
       
#endif // WacomGSS_OpenSSL_hpp
