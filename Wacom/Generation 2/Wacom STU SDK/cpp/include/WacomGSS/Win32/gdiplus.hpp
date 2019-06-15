/// @file      WacomGSS/Win32/gdiplus.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper classes to gdiplus.h for exception safety

#ifndef WacomGSS_Win32_gdiplus_hpp
#define WacomGSS_Win32_gdiplus_hpp

#include <WacomGSS/Win32/windows.hpp>

#include <objbase.h>
#pragma warning(push,3)
#include <gdiplus.h>
#pragma warning(pop)


namespace WacomGSS
{
  namespace Win32
  {



    std::error_category const & Gdiplus_error_category() noexcept;



    inline std::error_code make_Gdiplus_error_code(Gdiplus::Status s)
    {
      return std::error_code(static_cast<int>(s), Gdiplus_error_category());
    }



    WacomGSS_noreturn inline void throw_Gdiplus_error(Gdiplus::Status s)
    {
      throw std::system_error(make_Gdiplus_error_code(s));
    }



    template<typename Ty>
    WacomGSS_noreturn inline void throw_Gdiplus_error(Gdiplus::Status s, Ty _Message)
    {
      throw std::system_error(make_Gdiplus_error_code(s), _Message);
    }



    inline void Gdiplus_Ok(Gdiplus::Status s)
    {
      if (s != Gdiplus::Ok)
      {
        throw_Gdiplus_error(s);
      }
    }

    template<typename Ty>
    inline void Gdiplus_Ok(Gdiplus::Status s, Ty _Message)
    {
      if (s != Gdiplus::Ok)
      {
        throw_Gdiplus_error(s, _Message);
      }
    }




    /// @brief wraps Gdiplus Token value
    struct Gdiplus_StartupToken_traits
    {
      static ULONG_PTR invalid() noexcept
      {
        return 0;
      }

      static bool is_valid(ULONG_PTR value) noexcept
      {
        return value != 0;
      }

      static void close(ULONG_PTR value) noexcept
      {
        Gdiplus::GdiplusShutdown(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<ULONG_PTR,Gdiplus_StartupToken_traits> GdiplusStartupToken;



  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_gdiplus_hpp
