/// @file      WacomGSS/Win32/hidsdi.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper classes to hidsdi.h for exception safety

#ifndef WacomGSS_Win32_hidsdi_hpp
#define WacomGSS_Win32_hidsdi_hpp

#include <WacomGSS/Win32/ntstatus.hpp>
extern "C" 
{
#include <hidsdi.h>
}

namespace WacomGSS
{
  namespace Win32
  {


    struct PHIDP_PREPARSED_DATA_traits
    {
      static PHIDP_PREPARSED_DATA invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(PHIDP_PREPARSED_DATA value) noexcept
      {
        return value != nullptr;
      }

      static void close(PHIDP_PREPARSED_DATA value) noexcept
      {
        //BOOLEAN
        ::HidD_FreePreparsedData(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<PHIDP_PREPARSED_DATA,PHIDP_PREPARSED_DATA_traits> HidP_Preparsed_Data;


  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_hidsdi_hpp
