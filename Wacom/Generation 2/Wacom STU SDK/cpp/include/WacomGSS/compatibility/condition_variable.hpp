/// @file      WacomGSS/compatibility/condition_variable.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     maps class condition_variable from std or boost

#ifndef WacomGSS_condition_variable_hpp
#define WacomGSS_condition_variable_hpp

#include <WacomGSS/config.hpp>


#if defined(WacomGSS_compatibility_condition_variable)

#if WacomGSS_compatibility_condition_variable == 1 // boost

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(push)
#pragma warning(disable:4548 4365 4061 4987)
#endif

#include <boost/thread/condition_variable.hpp>

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(pop)
#endif

namespace WacomGSS
{
  using boost::condition_variable;
  using boost::cv_status;
}

#elif WacomGSS_compatibility_condition_variable == 0 // std
#if defined(_MSC_VER) //&& _MSC_VER <= 1900
#pragma warning(push)
#pragma warning(disable:4265 4548 4623 5039)
#endif

#include <condition_variable>

#if defined(_MSC_VER) //&& _MSC_VER <= 1900
#pragma warning(pop)
#endif

namespace WacomGSS
{
  using std::condition_variable;
#if defined(_MSC_VER) && _MSC_VER <= 1700
  using namespace std::cv_status;
#else
  using std::cv_status;
#endif
}

#else

#error WacomGSS: macro 'WacomGSS_compatibility_condition_variable' has not been set to an unexpected value. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

#else

#error WacomGSS: macro 'WacomGSS_compatibility_condition_variable' has not been defined. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif
       
#endif // WacomGSS_condition_variable_hpp
