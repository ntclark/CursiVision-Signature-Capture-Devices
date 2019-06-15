/// @file      WacomGSS/Win32/iphlpapi.hpp
/// @copyright Copyright (c) 2014 Wacom Company Limited
/// @author    mholden
/// @date      2014-10-23
/// @brief     provides wrapper classes to iphlpapi.h for exception safety

#ifndef WacomGSS_Win32_iphlpapi_hpp
#define WacomGSS_Win32_iphlpapi_hpp

#include <WacomGSS/Win32/windows.hpp>
#include <ws2tcpip.h>
#include <iphlpapi.h>


namespace WacomGSS
{
  namespace Win32
  {


  } // namespace Win32

} // namespace WacomGSS


//=============================================================================



namespace WacomGSS
{
  namespace Win32
  {

  struct PMIB_IF_TABLE2_FreeMibTable_traits
  {
    static PMIB_IF_TABLE2 invalid() noexcept
    {
      return nullptr;
    }

    static bool is_valid(PMIB_IF_TABLE2 value) noexcept
    {
      return value != nullptr;
    }

    static void close(PMIB_IF_TABLE2 value) noexcept
    {
      //void
      ::FreeMibTable(value);
    }

    static void throw_invalid_handle_exception()
    {
      throw_win32api_error(::GetLastError());
    }

    template<typename Ty>
    static void throw_invalid_handle_exception(Ty _Message)
    {
      throw_win32api_error(::GetLastError(), _Message);
    }

  };
  typedef unique_handle<PMIB_IF_TABLE2,PMIB_IF_TABLE2_FreeMibTable_traits> PMIB_If_Table2;



  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_iphlpapi_hpp
