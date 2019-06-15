/// @file      WacomGSS/Win32/dwrite.hpp
/// @copyright Copyright (c) 2014 Wacom Company Limited
/// @author    mholden
/// @date      2014-02-28
/// @brief     provides wrapper for dwrite.h

#ifndef WacomGSS_Win32_dwrite_hpp
#define WacomGSS_Win32_dwrite_hpp

#include <WacomGSS/config.hpp>

// C4263: 'function' : member function does not override any base class virtual member function
// C4264: 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden

#pragma warning(push)
#pragma warning(disable: 4263 4264)
#include <dwrite.h>
#pragma warning(pop)

#endif // WacomGSS_Win32_dwrite_hpp
