/// @file      WacomGSS/Win32/getDevicePowerState.hpp
/// @copyright Copyright (c) 2014 Wacom Company Limited
/// @author    mholden
/// @date      2014-01-14
/// @brief     provides a wrapper for retrieving the power state of a device

#ifndef WacomGSS_Win32_getDevicePowerState_hpp
#define WacomGSS_Win32_getDevicePowerState_hpp

#include <WacomGSS/Win32/cfgmgr32.hpp>

namespace WacomGSS
{
  namespace Win32
  {
    ::DEVICE_POWER_STATE getDevicePowerState(::DEVINST devInst);
  }
}

#endif
