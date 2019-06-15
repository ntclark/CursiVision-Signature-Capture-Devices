/// @file      delayLoadWGSS.hpp
/// @copyright Copyright(c) 2018 Wacom Co., Ltd.
/// @author    kparkes
/// @date      2018-11-14
/// @brief     Delay load hook function for loading from %CommonFiles%\WacomGSS
///            ONLY INCLUDE ONCE

#pragma once

#include <WacomGSS/Win32/windows.hpp>

#pragma warning(push, 3)
#pragma warning(disable: 4191 5039)

#include <delayimp.h>
#pragma warning(pop)

namespace WacomGSS {

  namespace Win32 {

    FARPROC WINAPI dliHook(unsigned dliNotify, PDelayLoadInfo pdli);

  }
}

decltype(__pfnDliNotifyHook2) __pfnDliNotifyHook2 = WacomGSS::Win32::dliHook;


