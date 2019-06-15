/// @file      WacomGSS/Win32/debug_TypeInfo.hpp
/// @copyright Copyright (c) 2013 Wacom Company Limited
/// @author    mholden
/// @date      2013-05-24
/// @brief     decodes TypeInfo for displaying

#ifndef WacomGSS_Win32_debug_TypeInfo_hpp
#define WacomGSS_Win32_debug_TypeInfo_hpp

#include <WacomGSS/win32/com.hpp>
#include <iosfwd>

namespace WacomGSS
{
  namespace Win32
  {
    void debug_TypeInfo(IDispatch * pIDispatch, std::ostream & o);
  }
}

#endif
