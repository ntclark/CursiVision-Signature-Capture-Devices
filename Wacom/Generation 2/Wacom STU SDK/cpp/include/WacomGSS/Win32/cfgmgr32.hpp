/// @file      WacomGSS/Win32/cfgmgr32.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper classes to cfgmgr32.h for exception safety

#ifndef WacomGSS_Win32_cfgmgr32_hpp
#define WacomGSS_Win32_cfgmgr32_hpp

#include <WacomGSS/Win32/windows.hpp>

#include <cfgmgr32.h>


namespace WacomGSS
{
  namespace Win32
  {

    std::error_category const & CONFIGRET_error_category() noexcept;

    inline std::error_code make_CONFIGRET_error_code(CONFIGRET cr)
    {
      return std::error_code(static_cast<int>(cr), CONFIGRET_error_category());
    }

    WacomGSS_noreturn inline void throw_CONFIGRET_error(CONFIGRET cr)
    {
      throw std::system_error(make_CONFIGRET_error_code(cr));
    }

    template<typename Ty>
    WacomGSS_noreturn inline void throw_CONFIGRET_error(CONFIGRET cr, Ty _Message)
    {
      throw std::system_error(make_CONFIGRET_error_code(cr), _Message);
    }



    inline void win32api_CONFIGRET(CONFIGRET value)
    {
      if (value != CR_SUCCESS) 
      {
        throw_CONFIGRET_error(value);
      }
    }

    template<typename Ty>
    inline void win32api_CONFIGRET(CONFIGRET value, Ty _Message)
    {
      if (value != CR_SUCCESS) 
      {
        throw_CONFIGRET_error(value, _Message);
      }
    }


  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_cfgmgr32_hpp
