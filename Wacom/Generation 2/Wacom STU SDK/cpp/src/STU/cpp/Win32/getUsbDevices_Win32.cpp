#include <WacomGSS/STU/getUsbDevices.hpp>
#include <WacomGSS/enumUsbDevices.hpp>
#include <memory>
#include <initguid.h>
DEFINE_GUID(GUID_DEVINTERFACE_WINUSBSIGNPAD_MI01, 0xe85d416b, 0x7c2a, 0x4a7a, 0x84, 0x0e, 0xfb, 0x49, 0x36, 0x54, 0xc9, 0x1e);

#pragma comment(linker,"/DEFAULTLIB:SETUPAPI.LIB")

//
// The top level enumeration provides:
//
// (a)  \\?\hid#vid_056a&pid_00a1#7&32ce0d06&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}
// or
// (b)  \\?\hid#vid_056a&pid_00a3&mi_00#8&1fa36a67&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}
//                                ^^^^^ = mulitple-instance device
//
// The first one (a) is fine for opening as HID (will be STU-300/STU-500).
// The second is a multiple-instance (ie bulk). We can open this for HID use, but need to open
// the bulk handle separately. To do this we need to go up and across the device tree:
//
//   USB\VID_056A&PID_00A3&MI_00\7&A99CEC9&0&0001 --> USB\VID_056A&PID_00A3&MI_01\7&A99CEC9&0&0001
//   ^                                                ^^^^^^^^^^^^^^^^^^^^^^^^^^^ = we want this for bulk
//   | go up a level
//   |
//   HID\VID_056A&PID_00A3&MI_00\7&A99CEC9&0&0001  << we start here
//
// Once we have the USB...MI_01 DevInst, we can query for the WinUSB bulk device filename and use
// that handle in WinUSB for bulk transfers.
//

namespace WacomGSS
{
  namespace STU
  {

    static std::wstring getBulkFileName(DEVINST DevInfoData_DevInst)
    {
      // Assumes this is an "MI" device
    
      using namespace Win32;

      DEVINST DevInst;

      win32api_CONFIGRET(CM_Get_Parent(&DevInst, DevInfoData_DevInst, 0));
      win32api_CONFIGRET(CM_Get_Sibling(&DevInst, DevInst, 0));

      ULONG ulSize = 0;
      win32api_CONFIGRET(CM_Get_Device_ID_Size(&ulSize, DevInst, 0));       
      
      std::unique_ptr<WCHAR[]> pszID(new WCHAR[ulSize+1]);
      win32api_CONFIGRET(CM_Get_Device_ID(DevInst, pszID.get(), ulSize+1, 0));

      ulSize = 0;
      win32api_CONFIGRET(CM_Get_Device_Interface_List_Size(&ulSize, const_cast<LPGUID>(&GUID_DEVINTERFACE_WINUSBSIGNPAD_MI01), pszID.get(), 0));
      
      std::unique_ptr<WCHAR[]> pszBulkName(new WCHAR[ulSize+1]);
      win32api_CONFIGRET(CM_Get_Device_Interface_List(const_cast<LPGUID>(&GUID_DEVINTERFACE_WINUSBSIGNPAD_MI01), pszID.get(), pszBulkName.get(), ulSize+1, 0));
      
      return pszBulkName.get();
    }



    bool isSupportedUsbDevice(std::uint16_t idVendor, std::uint16_t idProduct) noexcept
    {
      return idVendor == VendorId_Wacom &&
             (idProduct >= ProductId_min && idProduct <= ProductId_max);
    }



    std::vector<UsbDevice> getUsbDevices()
    {
      std::vector<UsbDevice> usbDevices;

      enumUsbDevices
      (
        [&usbDevices] (::WacomGSS::UsbDevice & usbDevice) -> bool
        {
          if (isSupportedUsbDevice(usbDevice.idVendor, usbDevice.idProduct))
          {
            usbDevices.push_back(STU::UsbDevice(usbDevice.idVendor, usbDevice.idProduct, usbDevice.bcdDevice, usbDevice.fileName, usbDevice.isMI ? getBulkFileName(usbDevice.devInst) : std::wstring(), usbDevice.devInst));
          }
          return true;
        }
      );

      return usbDevices;
    }



  } // namespace STU

} // namespace WacomGSS

