/// @file      WacomGSS/compatibility/thread.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     maps class thread from std or boost

#ifndef WacomGSS_thread_hpp
#define WacomGSS_thread_hpp

#include <WacomGSS/config.hpp>


#if defined(WacomGSS_compatibility_thread)

#if WacomGSS_compatibility_thread == 1 // boost

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(push)
#pragma warning(disable:4548 4365 4061 4987)
#endif

#include <boost/thread/thread.hpp>

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(pop)
#endif

namespace WacomGSS
{
  using boost::thread;
  namespace this_thread
  {
    using namespace boost::this_thread;
  }
}

#elif WacomGSS_compatibility_thread == 0 // std
#if defined(_MSC_VER) && _MSC_VER <= 1800
#pragma warning(push)
#pragma warning(disable:4265)
#endif

#pragma warning(push)
#pragma warning(disable: 4265 5039)
#include <thread>
#pragma warning(pop)

#if defined(_MSC_VER) && _MSC_VER <= 1800
#pragma warning(pop)
#endif

namespace WacomGSS
{
  using std::thread;
  namespace this_thread = std::this_thread;
}

#else

#error WacomGSS: macro 'WacomGSS_compatibility_thread' has not been set to an unexpected value. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

#else

#error WacomGSS: macro 'WacomGSS_compatibility_thread' has not been defined. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

       
#endif // WacomGSS_thread_hpp
