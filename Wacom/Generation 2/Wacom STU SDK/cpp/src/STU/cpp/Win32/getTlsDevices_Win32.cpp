#include <WacomGSS/STU/TlsInterface.hpp>
#include <WacomGSS/Win32/enumDevices.hpp>

namespace WacomGSS
{
  namespace STU
  {

    
    std::vector<TlsDevice> getTlsDevices()
    {
      GUID guidWacom { 0xffccadeb, 0xf3d0, 0x443f,{ 0xb4, 0x21, 0x83, 0xe0, 0x98, 0xca, 0xf8, 0x1b } };

      std::vector<TlsDevice> tlsDevices;

      Win32::enumDevices
      (
        guidWacom,
        [&tlsDevices] (Win32::HDevInfo &, SP_DEVICE_INTERFACE_DATA &, SP_DEVINFO_DATA &, PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetailData) -> bool
        {
          std::wstring tlsDevice{ pDeviceInterfaceDetailData->DevicePath };
          tlsDevices.emplace_back(std::move(tlsDevice));
          return true;
        }
      );

      return tlsDevices;
    }



  } // namespace STU

} // namespace WacomGSS

