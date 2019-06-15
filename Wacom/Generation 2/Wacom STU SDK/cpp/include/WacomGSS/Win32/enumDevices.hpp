/// @file      WacomGSS/enumUsbDevices.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides platform independent enumeration of attached USB devices.

#ifndef WacomGSS_Win32_enumDevices_hpp
#define WacomGSS_Win32_enumDevices_hpp

#include <WacomGSS/Win32/setupapi.hpp>

#pragma warning(push, 3)
#pragma warning(disable: 4265)
#include <functional>
#pragma warning(pop)

namespace WacomGSS
{

  namespace Win32
  {

    /// @brief  Enumerates all attached devices of the specified type.
    /// @param  A function object that is called for each device that is found.
    ///         The function should return true to continue processing or false to
    ///         stop. The function may safely throw exceptions.  
    void enumDevices(GUID const & guid, std::function<bool(Win32::HDevInfo &, SP_DEVICE_INTERFACE_DATA &, SP_DEVINFO_DATA &, PSP_DEVICE_INTERFACE_DETAIL_DATA)> f);

    /// @brief    Pointer to function that is called if the given Win32 API failed during enumeration.
    ///           By replacing the default function it is possible to stop enumeration by
    ///           returning false, or by throwing an exception. Return true to continue 
    ///           the enumeration. This is provided purely for diagnostic purposes and
    ///           there is no expectation that this will ever be used.
    /// @param getData is set to false to indicate the first call to get the size of the data; is set to true if the data is being retrieved.
    /// @warning  There is no thread protection covering the change and use of this variable.
    extern bool (* enumDevices_failedSetupDiGetDeviceInterfaceDetail)(DWORD dwError, bool getData, SP_DEVICE_INTERFACE_DATA const & spDeviceInterfaceData);


  } // namespace Win32

} // namespace WacomGSS

#endif // WacomGSS_Win32_enumDevices_hpp
