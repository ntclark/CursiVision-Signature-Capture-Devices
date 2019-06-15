/// @file      WacomGSS/Win32/ntstatus.hpp
/// @copyright Copyright (c) 2016 Wacom Company Limited
/// @author    mholden
/// @date      2016-08-22
/// @brief     provides wrapper classes to NTSTATUS for exception safety

#ifndef WacomGSS_Win32_ntstatus_hpp
#define WacomGSS_Win32_ntstatus_hpp

#include <WacomGSS/Win32/windows.hpp>

#ifndef _NTDEF_
typedef __success(return >= 0) LONG NTSTATUS;
typedef NTSTATUS *PNTSTATUS;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif


namespace WacomGSS
{
  namespace Win32
  {

    std::error_category const & NTSTATUS_error_category() noexcept;



    inline std::error_code make_NTSTATUS_error_code(NTSTATUS cr)
    {
      return std::error_code(static_cast<int>(cr), NTSTATUS_error_category());
    }



    WacomGSS_noreturn inline void throw_NTSTATUS_error(NTSTATUS cr)
    {
      throw std::system_error(make_NTSTATUS_error_code(cr));
    }



    template<typename Ty>
    WacomGSS_noreturn inline void throw_NTSTATUS_error(NTSTATUS cr, Ty _Message)
    {
      throw std::system_error(make_NTSTATUS_error_code(cr), _Message);
    }



    inline void win32api_NTSTATUS_equal(NTSTATUS test, NTSTATUS value)
    {
      if (value != test) 
      {
        throw_NTSTATUS_error(value);
      }
    }

    template<typename Ty>
    inline void win32api_NTSTATUS_equal(NTSTATUS test, NTSTATUS value, Ty _Message)
    {
      if (value != test) 
      {
        throw_NTSTATUS_error(value, _Message);
      }
    }


    inline void win32api_NTSTATUS(NTSTATUS value)
    {
      if (!NT_SUCCESS(value))
      {
        throw_NTSTATUS_error(value);
      }
    }

    template<typename Ty>
    inline void win32api_NTSTATUS(NTSTATUS value, Ty _Message)
    {
      if (!NT_SUCCESS(value))
      {
        throw_NTSTATUS_error(value, _Message);
      }
    }



  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_ntstatus_hpp
