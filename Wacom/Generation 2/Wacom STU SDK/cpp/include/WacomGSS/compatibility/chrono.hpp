/// @file      WacomGSS/compatibility/chrono.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     maps chrono from std or boost

#ifndef WacomGSS_chrono_hpp
#define WacomGSS_chrono_hpp

#include <WacomGSS/config.hpp>


#if defined(WacomGSS_compatibility_chrono)

#if WacomGSS_compatibility_chrono == 1 // boost

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(push)
#pragma warning(disable:4365 4987)
#endif

#include <boost/chrono.hpp>

#if defined(_MSC_VER) && _MSC_VER <= 1600
#pragma warning(pop)
#endif

namespace WacomGSS
{

  namespace chrono
  {
    using namespace boost::chrono;
  }

}

#elif WacomGSS_compatibility_chrono == 0 // std

#pragma warning(push)
#pragma warning(disable:4265)
#include <chrono>
#pragma warning(pop)


namespace WacomGSS
{
  namespace chrono = std::chrono;  
}

#else

#error WacomGSS: macro 'WacomGSS_compatibility_chrono' has not been set to an unexpected value. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

#else

#error WacomGSS: macro 'WacomGSS_compatibility_chrono' has not been defined. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif


#endif // WacomGSS_chrono_hpp
