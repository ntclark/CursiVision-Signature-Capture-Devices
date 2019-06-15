/// @file      WacomGSS/Win32/getFileVersion.hpp
/// @copyright Copyright (c) 2013 Wacom Company Limited
/// @author    mholden
/// @date      2013-07-04
/// @brief     provides a wrapper for retrieving the file version from the version resource.

#ifndef WacomGSS_Win32_getFileVersion_hpp
#define WacomGSS_Win32_getFileVersion_hpp

#include <WacomGSS/config.hpp>

namespace WacomGSS
{
  namespace Win32
  {
    std::uint64_t getFileVersion(wchar_t const * fileName);
  }
}

#endif
