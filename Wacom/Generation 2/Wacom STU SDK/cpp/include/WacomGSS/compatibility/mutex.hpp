/// @file      WacomGSS/compatibility/mutex.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     maps class mutex from std or boost

#ifndef WacomGSS_mutex_hpp
#define WacomGSS_mutex_hpp

#include <WacomGSS/config.hpp>


#if defined(WacomGSS_compatibility_mutex)

#if WacomGSS_compatibility_mutex == 1 // boost

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(push)
#pragma warning(disable:4548 4365 4061 4987)
#endif

#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(pop)
#endif

namespace WacomGSS
{
  using boost::mutex;
  using boost::recursive_mutex;
  using boost::lock_guard;
  using boost::unique_lock;
}

#elif WacomGSS_compatibility_mutex == 0 // std

#pragma warning(push)
#pragma warning(disable: 4265 5039)
#include <mutex>
#pragma warning(pop)

namespace WacomGSS
{
  using std::mutex;
  using std::recursive_mutex;
  using std::lock_guard;
  using std::unique_lock;
}

#else

#error WacomGSS: macro 'WacomGSS_compatibility_mutex' has not been set to an unexpected value. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

#else

#error WacomGSS: macro 'WacomGSS_compatibility_mutex' has not been defined. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif
       
#endif // WacomGSS_mutex_hpp
