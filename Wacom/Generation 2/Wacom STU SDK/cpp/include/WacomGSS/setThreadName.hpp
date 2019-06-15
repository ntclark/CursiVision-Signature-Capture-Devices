/// @file      WacomGSS/setThreadName.hpp
/// @copyright Copyright (c) 2012 Wacom Company Limited
/// @author    mholden
/// @date      2012-02-10
/// @brief     Debug diagnostic routine to set the name of the thread, as displayed in debugging.

#ifndef WacomGSS_setThreadName_hpp
#define WacomGSS_setThreadName_hpp

#include <WacomGSS/config.hpp>

#if defined(WacomGSS_POSIX) && !defined(WacomGSS_MAC_OSX)
#include <pthread.h>
#endif


namespace WacomGSS
{


#if defined(WacomGSS_WIN32)
  /// @brief Sets the thread identified by threadId with the given name.
  /// @note The Microsoft Visual Studio 2010 debugger appears to be limited to 31 characters.
  void setThreadName(uint32_t threadId, char const * threadName);
#endif

#if defined(WacomGSS_POSIX) && !defined(WacomGSS_MAC_OSX)
  /// @brief Sets the thread identified by threadId with the given name.
  void setThreadName(pthread_t thread, char const * threadName);
#endif


  /// @brief Sets the current thread with the given name.
  /// @note The Microsoft Visual Studio 2010 debugger appears to be limited to 31 characters.
  void setThreadName(char const * threadName);


} // namespace WacomGSS

#endif
