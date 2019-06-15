/// @file      OpenSSL.cpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides a thin layer of objects from the OpenSSL library to provide basic exception safety.

#include <WacomGSS/OpenSSL.hpp>
#if defined(WacomGSS_WIN32)
#include <WacomGSS/Win32/windows.hpp>
#endif
#pragma warning(push, 3)
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#pragma warning(pop)

namespace WacomGSS
{
  namespace OpenSSL
  {



#define WacomGSS_fr(NAME)\
      static inline :: NAME *          f2r(NAME##_ *           value) { return reinterpret_cast< :: NAME *         >(value      ); } \
      static inline :: NAME const *    f2r(NAME##_ const *     value) { return reinterpret_cast< :: NAME const *   >(value      ); } \
      static inline NAME##_ *          r2f(:: NAME *           value) { return reinterpret_cast< NAME##_ *         >(value      ); } \
      static inline NAME##_ const *    r2f(:: NAME const *     value) { return reinterpret_cast< NAME##_ const *   >(value      ); }

#define WacomGSS_o(NAME)\
      static inline :: NAME const *    o2r(NAME const &        value) { return reinterpret_cast< :: NAME const *   >(value.get()); } \
      static inline :: NAME *          o2r(NAME &              value) { return reinterpret_cast< :: NAME *         >(value.get()); }

#define WacomGSS_fro(NAME) WacomGSS_fr(NAME)  WacomGSS_o(NAME) 

    WacomGSS_fro(SSL)
      WacomGSS_fro(SSL_CTX)
      WacomGSS_fro(EVP_PKEY)
      WacomGSS_fro(X509)
      WacomGSS_fr(SSL_METHOD)
      WacomGSS_o(BIO)

    namespace Threading
    {
#if defined(WacomGSS_WIN32)
      static HANDLE *lock_cs;

      static void win32_locking_callback(int mode, int type, const char *file, int line) noexcept
      {
        UNREFERENCED_PARAMETER(line);
        UNREFERENCED_PARAMETER(file);
        if (mode & CRYPTO_LOCK) {
          WaitForSingleObject(lock_cs[type], INFINITE);
        }
        else {
          ReleaseMutex(lock_cs[type]);
        }
      }

      static void thread_setup(void)
      {
        if (lock_cs)
          return;

        int i;

        lock_cs = (HANDLE *)OPENSSL_malloc(CRYPTO_num_locks() * sizeof(HANDLE));
        for (i = 0; i < CRYPTO_num_locks(); i++) {
          lock_cs[i] = CreateMutex(NULL, FALSE, NULL);
        }

        CRYPTO_set_locking_callback(win32_locking_callback);
        /* id callback defined */
      }

      static void thread_cleanup(void)
      {
        int i;

        CRYPTO_set_locking_callback(NULL);
        for (i = 0; i < CRYPTO_num_locks(); i++)
          CloseHandle(lock_cs[i]);
        OPENSSL_free(lock_cs);
      }
#else
      static inline void thread_setup(void) 
      {
      }
#endif

    }
    

    int (SSL_library_init)()
    {
      Threading::thread_setup();

      return ::SSL_library_init();
    }



    SSL_METHOD_ const * TLSv1_2_client_method()
    {
      return r2f(::TLSv1_2_client_method());
    }



    void SSL_CTX::SSL_CTX_Deleter::operator ()(SSL_CTX_ * p)
    {
      ::SSL_CTX_free(f2r(p));
    }



    SSL_CTX::SSL_CTX(SSL_METHOD_ const * method)
      :
      m_p(r2f(::SSL_CTX_new(f2r(method))), SSL_CTX_Deleter())
    {
    }



    SSL_CTX_ * SSL_CTX::get()
    {
      return m_p.get();
    }



    SSL_CTX_ const * SSL_CTX::get() const
    {
      return m_p.get();
    }



    SSL::SSL(SSL_CTX & sslCtx)
      :
      base(r2f(::SSL_new(o2r(sslCtx)))),
      m_sslCtx(sslCtx)
    {}



    void SSL::reset(SSL_CTX & sslCtx)
    {
      base::reset(r2f(::SSL_new(o2r(sslCtx))));
      m_sslCtx.reset(sslCtx);
    }



    int SSL_CTX_use_certificate(SSL_CTX & ctx, X509 & x)
    {
      return ::SSL_CTX_use_certificate(o2r(ctx), o2r(x));
    }



    int SSL_CTX_use_PrivateKey(SSL_CTX & ctx, EVP_PKEY & pkey)
    {
      return ::SSL_CTX_use_PrivateKey(o2r(ctx), o2r(pkey));
    }



    int SSL_CTX_set_cipher_list(SSL_CTX & ctx, char const * str)
    {
      return ::SSL_CTX_set_cipher_list(o2r(ctx), str);
    }


    void SSL_CTX_set_verify(SSL_CTX & ctx, int mode, int(*callback) (int, X509_STORE_CTX_ *))
    {
#pragma warning(suppress: 5039) // pointer or reference to potentially throwing function passed to extern C function
      ::SSL_CTX_set_verify(o2r(ctx), mode, reinterpret_cast<int (*)(int, X509_STORE_CTX *)>(callback));
    }



    void SSL_SSL_free_trait::close(SSL_ * value) noexcept
    {
      ::SSL_free(f2r(value));
    }



    void SSL_SSL_free_trait::throw_invalid_handle_exception()
    {
      throw error(::ERR_get_error());
    }



    SSL::SSL()
    :
      base(),
      m_sslCtx(nullptr)
    {
    }



    void SSL_set_bio(SSL & s, BIO & rbio, BIO & wbio)
    {
      ::SSL_set_bio(o2r(s), o2r(rbio), o2r(wbio));
      // SSL object takes ownership
      rbio.release(); 
      wbio.release();
    }



    int SSL_connect(SSL & ssl)
    {
      return ::SSL_connect(o2r(ssl));
    }



    int SSL_read(SSL & ssl, void * buf, int num)
    {
      return ::SSL_read(o2r(ssl), buf, num);
    }


    
    int SSL_write(SSL & ssl, void const * buf, int num)
    {
      return ::SSL_write(o2r(ssl), buf, num);
    }

    X509 SSL_get_peer_certificate(SSL & ssl)
    {
      return X509(r2f(::SSL_get_peer_certificate(o2r(ssl))));
    }
    


  } // namespace OpenSSL
  
} // namespace WacomGSS
