/// @file      OpenSSL_Loader.cpp
/// @copyright Copyright (c) 2017 Wacom Company Limited
/// @author    mholden
/// @date      2017-07-11
/// @brief     checks to see if the OpenSSL libraries has been loaded


#include <WacomGSS/OpenSSL.hpp>
#include <WacomGSS/Win32/windows.hpp>
#include <delayimp.h>



namespace WacomGSS
{
  namespace OpenSSL
  {


    LoaderError::LoaderError()
    :
      std::runtime_error("OpenSSL operations are unavailable as the libraries cannot be loaded (not present?)")
    {
    }

    static bool loadOpenSSL()
    {
      bool ret = false;
#if 0
      __try
      {       
		if (SUCCEEDED(__HrLoadAllImportsForDll("libcrypto-1_1.dll")) && SUCCEEDED(__HrLoadAllImportsForDll("libssl-1_1.dll")))
        {
          ret = true;
        }
      }
      __except (GetExceptionCode() == 0xc06d007e ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
      {
      }
#endif
      return ret;
    }


    Loader::Loader()
    {
      if (!isLoaded())
      {
        throw LoaderError();
      }
    }



    bool Loader::isLoaded() noexcept
    {
      return loadOpenSSL();
    }



  }
}
