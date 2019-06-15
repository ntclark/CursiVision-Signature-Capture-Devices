/// @file      OpenSSL.cpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides a thin layer of objects from the OpenSSL library to provide basic exception safety.

#include <WacomGSS/OpenSSL.hpp>

#pragma warning(push)
#pragma warning(disable: 4996 5039)

#include <applink.c>
#include <openssl/x509.h>
#include <openssl/pem.h>
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

//    WacomGSS_fro(SSL)
//      WacomGSS_fro(SSL_CTX)
//      WacomGSS_fro(EVP_PKEY)
      WacomGSS_fro(X509)
//      WacomGSS_fr(SSL_METHOD)
//      WacomGSS_o(BIO)


      int PEM_write_X509(FILE * fp, X509 & x509)
      {
        return ::PEM_write_X509(fp, o2r(x509));
      }
  
      int i2d_X509_fp(FILE * fp, X509 & x509)
      {
        return ::i2d_X509_fp(fp, o2r(x509));
      }
  
  
    } // namespace OpenSSL
    
  } // namespace WacomGSS
