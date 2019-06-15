/// @file      WacomGSS/config.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     meta configuration about the compliation platform and compiler

#ifndef WacomGSS_config_hpp
#define WacomGSS_config_hpp

#if defined(_MSC_VER)
#include <WacomGSS/config/msvc.hpp>
#elif defined(__MACH__)
#include <WacomGSS/config/osx.hpp>
#elif defined(__clang__)
#include <WacomGSS/config/clang.hpp>
#elif defined(__GNUC__)
#include <WacomGSS/config/gcc.hpp>
#else
# error WacomGSS: Unknown compiler
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <WacomGSS/config/win32.hpp>
#elif defined(__MACH__)
#include <WacomGSS/config/osx.hpp>
#elif defined (__linux__)
#include <WacomGSS/config/Linux.hpp>
#else
# error WacomGSS: Unknown platform
#endif

#endif // WacomGSS_config_hpp
