/// @file      WacomGSS/compatibility/atomic.hpp
/// @copyright Copyright (c) 2014 Wacom Company Limited
/// @author    mholden
/// @date      2014-02-11
/// @brief     maps class thread from std or boost

#ifndef WacomGSS_atomic_hpp
#define WacomGSS_atomic_hpp

#include <WacomGSS/config.hpp>


#if defined(WacomGSS_compatibility_atomic)

#if WacomGSS_compatibility_atomic == 1 // boost

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(push)
#pragma warning(disable:4061 4987)
// 4061: enumerator '' in switch of enum '' is not explicitly handled by a case label
// 4987: nonstandard extension used: 'throw (...)'
#endif

#include <boost/atomic/atomic.hpp>

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(pop)
#endif

namespace WacomGSS
{
  using boost::atomic;
}

#elif WacomGSS_compatibility_atomic == 0 // std
#include <atomic>
namespace WacomGSS
{
  using std::atomic;
}

#else

#error WacomGSS: macro 'WacomGSS_compatibility_atomic' has not been set to an unexpected value. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

#else

#error WacomGSS: macro 'WacomGSS_compatibility_atomic' has not been defined. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

       
#endif // WacomGSS_atomic_hpp
