/// @file      WacomGSS/Win32/winhttp.hpp
/// @copyright Copyright (c) 2012 Wacom Company Limited
/// @author    mholden
/// @date      2012-11-07
/// @brief     provides wrapper for winhttp.h

#ifndef WacomGSS_Win32_winhttp_hpp
#define WacomGSS_Win32_winhttp_hpp

#include <WacomGSS/unique_handle.hpp>

#include <winhttp.h>

namespace WacomGSS
{
  namespace Win32
  {
  
    struct HINTERNET_WinHttpCloseHandle_traits
    {
      static HINTERNET invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HINTERNET value) noexcept
      {
        return value != nullptr;
      }

      static void close(HINTERNET value) noexcept
      {
        //BOOL
        ::WinHttpCloseHandle(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }

      template<typename _Ty>
      static void throw_invalid_handle_exception(_Ty _Message)
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    };
    typedef unique_handle<HINTERNET,HINTERNET_WinHttpCloseHandle_traits> HInternet;

  }
}

#endif // WacomGSS_Win32_winhttp_hpp
