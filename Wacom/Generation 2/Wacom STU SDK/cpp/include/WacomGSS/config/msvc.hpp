/// @file      WacomGSS/config/msvc.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     meta configuration about the compliation platform and compiler

#ifndef WacomGSS_config_msvc_hpp
#define WacomGSS_config_msvc_hpp

#if _MSC_VER < 1600
# error WacomGSS: Compiler does not support enough of C++11 to compile this code
#elif _MSC_VER <= 1600
#include <exception>
#include <cstdint>

#pragma warning(push, 3)
#pragma warning(disable:4986)
#include <stdexcept>
#pragma warning(pop)

#define noexcept   throw()
#define constexpr  const

namespace std
{
  // The specification states that an exception_ptr should satisfy NullablePtr (18.8.5-2) VS2010 does not adhere to this.
  inline bool operator != (exception_ptr const & lhs, nullptr_t rhs)
  {
    return !operator==(lhs, rhs);
  }
}

namespace WacomGSS
{
  /// MSC06-CPP. Be aware of compiler optimization when dealing with sensitive data 
  /// https://www.securecoding.cert.org/confluence/display/cplusplus/MSC06-CPP.+Be+aware+of+compiler+optimization+when+dealing+with+sensitive+data
  void * memset_s(void * ptr, int value, size_t num);
}

#define WacomGSS_compatibility_condition_variable 1 // use boost
#define WacomGSS_compatibility_mutex              1 // use boost
#define WacomGSS_compatibility_thread             1 // use boost
#define WacomGSS_compatibility_chrono             1 // use boost
#define WacomGSS_compatibility_atomic             1 // use boost
#define WacomGSS_compatibility_optional           2 // use emulation

#define WacomGSS_compatibility_std_codecvt        0 /* does not require compatibility - use std */

#define WacomGSS_noreturn __declspec(noreturn) // [[noreturn]] attribute
#define WacomGSS_deprecated __declspec(deprecated)

#define WacomGSS_thread_local __declspec(thread)

#elif _MSC_VER <= 1800
#include <cstddef> // size_t, ptrdiff_t, nullptr_t
#include <cstdint> // uint32_t et al

#define noexcept   throw()
#define constexpr  const

namespace WacomGSS
{
    /// MSC06-CPP. Be aware of compiler optimization when dealing with sensitive data 
    /// https://www.securecoding.cert.org/confluence/display/cplusplus/MSC06-CPP.+Be+aware+of+compiler+optimization+when+dealing+with+sensitive+data
  void * memset_s(void * dest, int value, std::size_t count);
}

#define WacomGSS_compatibility_condition_variable 0 // use std
#define WacomGSS_compatibility_mutex              0 // use std
#define WacomGSS_compatibility_thread             0 // use std
#define WacomGSS_compatibility_chrono             0 // use std
#define WacomGSS_compatibility_atomic             0 // use std
#define WacomGSS_compatibility_optional           2 // use emulation

#define WacomGSS_compatibility_std_codecvt        0 /* does not require compatibility - use std */

#define WacomGSS_noreturn __declspec(noreturn) // [[noreturn]] attribute
#define WacomGSS_deprecated __declspec(deprecated)

#define WacomGSS_thread_local __declspec(thread)

#elif _MSC_VER >= 1900 && _MSC_VER < 2000   // Visual Studio 2015 & 2017, MSVC++ 14.0 & 14.1
#pragma warning(push, 3)
#include <cstddef> // size_t, ptrdiff_t, nullptr_t
#include <cstdint> // uint32_t et al
#pragma warning(pop)

#define WacomGSS_compatibility_condition_variable 0 // use std
#define WacomGSS_compatibility_mutex              0 // use std
#define WacomGSS_compatibility_thread             0 // use std
#define WacomGSS_compatibility_chrono             0 // use std
#define WacomGSS_compatibility_atomic             0 // use std
#define WacomGSS_compatibility_std_codecvt        0 // use std

#if _HAS_CXX17    // vs2017 with /std:c++17
#define WacomGSS_compatibility_optional           0 // use std
#else
#define WacomGSS_compatibility_optional           2 // use emulation
#endif

#if _MSC_FULL_VER >= 190023506
// 2015 update 1 works correctly with applying attributes to templates.
#define WacomGSS_noreturn    [[noreturn]]
#define WacomGSS_deprecated  [[deprecated]]
#else
// Although 2015 is supposed to now support attributes, it fails when they are applied to function templates.
#define WacomGSS_noreturn __declspec(noreturn)
#define WacomGSS_deprecated __declspec(deprecated)
#endif

#define WacomGSS_thread_local thread_local

namespace WacomGSS
{
    /// MSC06-CPP. Be aware of compiler optimization when dealing with sensitive data 
    /// https://www.securecoding.cert.org/confluence/display/cplusplus/MSC06-CPP.+Be+aware+of+compiler+optimization+when+dealing+with+sensitive+data
  void * memset_s(void * dest, int value, std::size_t count);
}

#else
#error WacomGSS: 'Microsoft Visual Studio' unsupported or untested compiler version
#endif


#endif // WacomGSS_config_msvc_hpp
