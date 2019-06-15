/// @file      WacomGSS/STU/getUsbDevices.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     API to get a list of all STU tablets currently attached to the system.

#ifndef WacomGSS_STU_getUsbDevices_hpp
#define WacomGSS_STU_getUsbDevices_hpp

#include <WacomGSS/config.hpp>
#include <string>
#include <vector>

namespace WacomGSS
{
  namespace STU
  {

    /// @brief USB device Vendor ID for Wacom.
    enum VendorId : std::uint16_t
    {
      VendorId_Wacom = 0x056a
    };

    /// @brief USB device Product IDs for %STU tablets.
    enum ProductId : std::uint16_t
    {
      ProductId_500  = 0x00a1,  ///< STU-500 
      ProductId_300  = 0x00a2,  ///< STU-300 
      ProductId_520A = 0x00a3,  ///< STU-520
      ProductId_430  = 0x00a4,  ///< STU-430 
      ProductId_530  = 0x00a5,  ///< STU-530   
      ProductId_430V = 0x00a6,  ///< STU-430V
      ProductId_540  = 0x00a8,  ///< STU-540
      ProductId_541  = 0x00a9   ///< STU-541
    };
    const std::uint16_t ProductId_min = 0x00a1;
    const std::uint16_t ProductId_max = 0x00af;
    
    /// @brief Checks if a device is supported.
    ///
    /// @param idVendor   Vendor ID of device.
    /// @param idProduct  Product ID of device.
    ///
    /// @return true if Vendor and Product IDs correspond to a Wacom %STU tablet.
    bool isSupportedUsbDevice(std::uint16_t idVendor, std::uint16_t idProduct) noexcept;

    /// @brief Basic attributes of a USB device.
    struct UsbDeviceBase
    {
      std::uint16_t  idVendor;  ///< Vendor ID (assigned by the USB-IF)
      std::uint16_t  idProduct; ///< Product ID (assigned by the manfacturer)
      std::uint16_t  bcdDevice; ///< Device release number in binary-coded decimal

      UsbDeviceBase(std::uint16_t vendor, std::uint16_t product, std::uint16_t device)
      :
        idVendor(vendor),
        idProduct(product),
        bcdDevice(device)
      {
      }
    };

#if defined(WacomGSS_DOXYGEN)
    /// @brief Platform-specific specialization of UsbDeviceBase.
    struct UsbDevice : UsbDeviceBase
    {
      std::wstring  fileName;       ///< Filename to use to open the device. <b>Windows only</b>
      std::wstring  bulkFileName;   ///< Filename to use to open the device for bulk transfer. <b>Windows only</b>
      std::uint32_t devInst;        ///< DEVINST handle. <b>Windows only</b>
      std::uint8_t  busNumber;      ///< bus number. <b>Linux only</b>
      std::uint8_t  deviceAddress;  ///< device address. <b>Linux only</b>

      explicit UsbDevice();

      /// @brief Constructor for UsbDevice on <b>Windows</b>.
      ///
      /// @param idVendor       USB Vendor ID.
      /// @param idProduct      USB Product ID.
      /// @param bcdDevice      USB Device release number.
      /// @param fn             Filename to use to open the device.
      /// @param bn             Filename to use to open the device for bulk transfer.
      /// @param di             DEVINST handle.
      UsbDevice(std::uint16_t idVendor, std::uint16_t idProduct, std::uint16_t bcdDevice, std::wstring fn, std::wstring bn, std::uint32_t di);

      /// @brief Constructor for UsbDevice on <b>Linux</b>.
      ///
      /// @param idVendor       USB Vendor ID.
      /// @param idProduct      USB Product ID.
      /// @param bcdDevice      USB Device release number.
      /// @param bn             bus number.
      /// @param deviceAddress  device address.
      UsbDevice(std::uint16_t idVendor, std::uint16_t idProduct, std::uint16_t bcdDevice, std::uint8_t bn, std::uint8_t da);
    };
#elif defined(WacomGSS_WIN32)
    struct UsbDevice : UsbDeviceBase
    {
      std::wstring  fileName;
      std::wstring  bulkFileName;
      std::uint32_t devInst;

      explicit UsbDevice()
      :
        UsbDeviceBase(0,0,0),
        fileName(),
        bulkFileName(),
        devInst(0)
      {
      }

      UsbDevice(std::uint16_t idVendor, std::uint16_t idProduct, std::uint16_t bcdDevice, std::wstring fn, std::wstring bn, std::uint32_t di)
      :
        UsbDeviceBase(idVendor, idProduct, bcdDevice),
        fileName(std::move(fn)),
        bulkFileName(std::move(bn)),
        devInst(di)
      {
      }
    };
#elif defined(WacomGSS_Linux)
    struct UsbDevice : UsbDeviceBase
    {
      std::uint8_t busNumber;
      std::uint8_t deviceAddress;

      explicit UsbDevice()
      :
        UsbDeviceBase(0,0,0),
        busNumber(0),
        deviceAddress(0)
      {
      }

      UsbDevice(std::uint16_t idVendor, std::uint16_t idProduct, std::uint16_t bcdDevice, std::uint8_t bn, std::uint8_t da)
      :
        UsbDeviceBase(idVendor, idProduct, bcdDevice),
        busNumber(bn),
        deviceAddress(da)
      {
      }
    };
#elif defined(WacomGSS_OSX)
        struct UsbDevice : UsbDeviceBase
        {
            explicit UsbDevice()
            :
            UsbDeviceBase(0,0,0)
            {
            }
            
            UsbDevice(std::uint16_t idVendor, std::uint16_t idProduct, std::uint16_t bcdDevice)
            :
            UsbDeviceBase(idVendor, idProduct, bcdDevice)
            {
            }
        };
#else
# error WacomGSS: 'struct UsbDevice': platform not implemented.
#endif

    /// @brief Retrieves the list of available USB devices.
    ///
    /// @return The list of available devices.
    ///
    /// @throws std::system_error on an unexpected system API call failure
    std::vector<UsbDevice> getUsbDevices();

    namespace ostream_operators
    {
      /// @brief  Formats the UsbDeviceBase as three 4-digit hex strings separated by colons:
      ///         "vvvv:pppp:dddd"
      std::ostream & operator << (std::ostream & o, UsbDeviceBase const & usbDevice);
    }


  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_getUsbDevices_hpp
