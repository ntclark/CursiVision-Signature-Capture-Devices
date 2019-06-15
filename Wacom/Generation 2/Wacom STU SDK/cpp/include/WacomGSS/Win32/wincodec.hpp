/// @file      WacomGSS/Win32/wincodec.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2012-01-33
/// @brief     provides wrapper for wincodec.h

#ifndef WacomGSS_Win32_wincodec_hpp
#define WacomGSS_Win32_wincodec_hpp

#include <WacomGSS/Win32/ocidl.hpp>

#define _INTSAFE_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable:4917)
#include <wincodec.h>
#pragma warning(pop)

#endif // WacomGSS_Win32_wincodec_hpp
