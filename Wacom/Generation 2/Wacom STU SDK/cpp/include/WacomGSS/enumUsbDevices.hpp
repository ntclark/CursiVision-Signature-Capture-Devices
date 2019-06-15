/// @file      WacomGSS/enumUsbDevices.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides platform independent enumeration of attached USB devices.

#ifndef WacomGSS_enumUsbDevices_hpp
#define WacomGSS_enumUsbDevices_hpp

#include <WacomGSS/config.hpp>

#if defined(WacomGSS_WIN32)
#include <WacomGSS/Win32/enumDevices.hpp>
#include <WacomGSS/Win32/cfgmgr32.hpp>
#endif

#pragma warning(push, 3)
#pragma warning(disable: 4265)
#include <functional>
#pragma warning(pop)
#include <cstdint>

namespace WacomGSS
{


  struct UsbDeviceBase
  {
    std::uint16_t  idVendor;  ///< Vendor ID (assigned by the USB-IF).
    std::uint16_t  idProduct; ///< Product ID (assigned by the manfacturer).
    std::uint16_t  bcdDevice; ///< Device release number in binary-coded decimal.
    
    bool           isMI;      ///< is a multi-interface device.
  };

#if defined(WacomGSS_WIN32)
  struct UsbDevice : UsbDeviceBase
  {
    std::wstring fileName;
    DEVINST      devInst;
  };
#elif defined(WacomGSS_Linux)
  struct UsbDevice : UsbDeviceBase
  {
    std::uint8_t busNumber;
    std::uint8_t deviceAddress;
  };
#elif defined(WacomGSS_OSX)
    struct UsbDevice : UsbDeviceBase
    {
        io_service_t usbService;
    };
#else
# error WacomGSS: 'struct UsbDevice': platform not implemented.
#endif

  /// @brief  Enumerates all attached USB devices.
  /// @param  A function object that is called for each USB device that is found.
  ///         The function should return true to continue processing or false to
  ///         stop. The function may safely throw exceptions.  
  void enumUsbDevices(std::function<bool (UsbDevice & usbDevice)> f);

#if defined(WacomGSS_WIN32)
  /// @brief    Pointer to function that is called if the given Win32 Hardware ID could not be parsed.
  ///           The default action is to ignore the device and continue enumeration. 
  ///           By replacing the default function it is possible to stop enumeration by
  ///           returning false, or by throwing an exception. Return true to continue 
  ///           the enumeration. This is provided purely for diagnostic purposes and
  ///           there is no expectation that this will ever be used.
  /// @warning  There is no thread protection covering the change and use of this variable.
  extern bool (* enumUsbDevices_unhandledParseHardwareId)(wchar_t const * hardwareID, wchar_t const * fileName);

  /// @brief    Pointer to function that is called if the given Win32 API failed during enumeration.
  ///           By replacing the default function it is possible to stop enumeration by
  ///           returning false, or by throwing an exception. Return true to continue 
  ///           the enumeration. This is provided purely for diagnostic purposes and
  ///           there is no expectation that this will ever be used.
  /// @param getData is set to false to indicate the first call to get the size of the data; is set to true if the data is being retrieved.
  /// @warning  There is no thread protection covering the change and use of this variable.
  extern bool (* enumUsbDevices_failedSetupDiGetDeviceRegistryProperty)(DWORD dwError, bool getData, SP_DEVINFO_DATA const & spDevinfoData, SP_DEVICE_INTERFACE_DETAIL_DATA const * pspDeviceInterfaceDetailData);
#endif

} // namespace WacomGSS

#endif // WacomGSS_enumUsbDevices_hpp
