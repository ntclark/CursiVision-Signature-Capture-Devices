/// @file      WacomGSS/Win32/setupapi.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper classes to setupapi.h for exception safety

#ifndef WacomGSS_Win32_setupapi_hpp
#define WacomGSS_Win32_setupapi_hpp

#include <WacomGSS/Win32/windows.hpp>

#pragma warning(push, 3)
#include <setupapi.h>
#pragma warning(pop)


namespace WacomGSS
{
  namespace Win32
  {
    

    struct HDEVINFO_traits
    {
      static HDEVINFO invalid() noexcept
      {
        return INVALID_HANDLE_VALUE;
      }

      static bool is_valid(HDEVINFO value) noexcept
      {
        return value != INVALID_HANDLE_VALUE;
      }

      static void close(HDEVINFO value) noexcept
      {
        //BOOL
        ::SetupDiDestroyDeviceInfoList(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<HDEVINFO,HDEVINFO_traits> HDevInfo;


  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_setupapi_hpp
