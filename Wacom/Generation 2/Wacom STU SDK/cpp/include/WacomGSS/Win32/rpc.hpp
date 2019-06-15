/// @file      WacomGSS/Win32/rpc.hpp
/// @copyright Copyright (c) 2014 Wacom Company Limited
/// @author    mholden
/// @date      2014-10-23
/// @brief     provides wrapper classes to rpc.h for exception safety

#ifndef WacomGSS_Win32_rpc_hpp
#define WacomGSS_Win32_rpc_hpp

#include <WacomGSS/Win32/windows.hpp>

#include <rpc.h>


namespace WacomGSS
{
  namespace Win32
  {

    inline void rpc_ok(RPC_STATUS value)
    {
      if (value != RPC_S_OK)
      {
        throw_win32api_error(*reinterpret_cast<DWORD const *>(&value));
      }
    }

  } // namespace Win32

} // namespace WacomGSS


//=============================================================================



namespace WacomGSS
{
  namespace Win32
  {

  struct RPC_WSTR_RpcStringFree_traits
  {
    static RPC_WSTR invalid() noexcept
    {
      return nullptr;
    }

    static bool is_valid(RPC_WSTR value) noexcept
    {
      return value != nullptr;
    }

    static void close(RPC_WSTR value) noexcept
    {
      //RPC_STATUS
      ::RpcStringFree(&value);
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
  typedef unique_handle<RPC_WSTR,RPC_WSTR_RpcStringFree_traits> RPC_WStr;



  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_rpc_hpp
