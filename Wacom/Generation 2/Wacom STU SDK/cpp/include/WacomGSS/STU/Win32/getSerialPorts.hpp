/// @file      WacomGSS/STU/Win32/getSerialPorts.hpp
/// @copyright Copyright (c) 2015 Wacom Company Limited
/// @author    mholden
/// @date      2015-11-12
/// @brief     provides a wrapper for retrieving the list of possible serial ports to connect to

#ifndef WacomGSS_STU_Win32_getSerialPorts_hpp
#define WacomGSS_STU_Win32_getSerialPorts_hpp

#include <WacomGSS/config.hpp>
#include <vector>


namespace WacomGSS
{
  namespace STU
  {

    /// @brief Provides simplified view of a serial port
    struct SerialPort
    {
      /// @brief The best guess of what type of port this is
      enum Type
      {
        Unknown,  ///@< could not determine type of port
        Physical, ///@< probably a physical port
        Virtual,  ///@< probably a virtual port
        Remote    ///@< probably a remote port that has been forwarded to this computer
      };

      Type         type; ///@< the type of port
      std::wstring name; ///@< the name to use for connection attempts
      std::wstring kernelName; ///@< the underlying name used by Windows.

      SerialPort(Type t, wchar_t const * n, wchar_t const * k) 
      : 
        type(t),
        name(n),
        kernelName(k) 
      {
      }
    };

    /// @brief Provides a list of serial ports available to connect to.
    ///
    /// Note that this cannot guarantee what is attached to these ports, only that they exist.
    std::vector<SerialPort> getSerialPorts();


  }
}

#endif
