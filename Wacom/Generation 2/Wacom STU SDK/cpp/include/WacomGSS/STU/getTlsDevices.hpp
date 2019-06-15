/// @file      WacomGSS/STU/getTlsDevices.hpp
/// @copyright Copyright (c) 2017 Wacom Company Limited
/// @author    mholden
/// @date      2017-05-12
/// @brief     API to get a list of all STU tablets currently attached to the system.

#ifndef WacomGSS_STU_getTlsDevices_hpp
#define WacomGSS_STU_getTlsDevices_hpp

#include <WacomGSS/config.hpp>

#include <string>
#include <vector>


namespace WacomGSS
{
  namespace STU
  {



#if defined(WacomGSS_DOXYGEN)
    /// @brief Platform-specific container to describe a TlsDevice..
    struct TlsDevice : UsbDeviceBase
    {
    };

#elif defined(WacomGSS_WIN32)
    struct TlsDevice
    {
      std::wstring deviceName;

      TlsDevice() = default;
      TlsDevice(std::wstring && name) : deviceName(name) {}
    };
#elif defined(WacomGSS_Linux)
    struct TlsDevice
    {
      std::uint16_t  idVendor;  ///< Vendor ID (assigned by the USB-IF)
      std::uint16_t  idProduct; ///< Product ID (assigned by the manfacturer)
      std::uint16_t  bcdDevice; ///< Device release number in binary-coded decimal
      std::uint8_t busNumber;
      std::uint8_t deviceAddress;

      explicit TlsDevice()
      :
        idVendor(0),
        idProduct(0),
        bcdDevice(0),
        busNumber(0),
        deviceAddress(0)
      {
      }

      TlsDevice(std::uint16_t vendor, std::uint16_t product, std::uint16_t device, std::uint8_t bn, std::uint8_t da)
      :
        idVendor(vendor),
        idProduct(product),
        bcdDevice(device),
        busNumber(bn),
        deviceAddress(da)
      {
      }
    };
#else
# error WacomGSS: 'struct TlsDevice': platform not implemented.
#endif
    

    std::vector<TlsDevice> getTlsDevices();



    namespace ostream_operators
    {
      std::ostream & operator << (std::ostream & o, TlsDevice const & tlsDevice);
    }


  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_getTlsDevices_hpp
