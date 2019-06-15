#include <WacomGSS/Win32/enumDevices.hpp>
#include <memory>


namespace WacomGSS
{
  namespace Win32
  {


    bool (* enumDevices_failedSetupDiGetDeviceInterfaceDetail)(DWORD dwError, bool getData, SP_DEVICE_INTERFACE_DATA const &);



    void enumDevices(GUID const & guid, std::function<bool(Win32::HDevInfo &, SP_DEVICE_INTERFACE_DATA &, SP_DEVINFO_DATA &, PSP_DEVICE_INTERFACE_DETAIL_DATA)> f)
    {
      HDevInfo hDevInfo(::SetupDiGetClassDevsEx(&guid, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE, 0, 0, 0));

      for (DWORD dwIndex = 0;; ++dwIndex)
      {
        SP_DEVICE_INTERFACE_DATA DeviceInterfaceData{ sizeof(DeviceInterfaceData) };
        if (::SetupDiEnumDeviceInterfaces(hDevInfo.get(), 0, &guid, dwIndex, &DeviceInterfaceData))
        {
          DWORD dwSize = 0;
          if (::SetupDiGetDeviceInterfaceDetail(hDevInfo.get(), &DeviceInterfaceData, 0, 0, &dwSize, 0) || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
          {
            std::unique_ptr<BYTE[]>          pBuffer(new BYTE[dwSize]);
            PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetailData = reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(pBuffer.get());
            pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

            SP_DEVINFO_DATA	DevInfoData{ sizeof(SP_DEVINFO_DATA) };

            if (::SetupDiGetDeviceInterfaceDetail(hDevInfo.get(), &DeviceInterfaceData, pDeviceInterfaceDetailData, dwSize, 0, &DevInfoData))
            {            
              if (!f(hDevInfo, DeviceInterfaceData, DevInfoData, pDeviceInterfaceDetailData))
              {
                break;
              }
            }
            else
            {
              if (enumDevices_failedSetupDiGetDeviceInterfaceDetail && !enumDevices_failedSetupDiGetDeviceInterfaceDetail(::GetLastError(), true, DeviceInterfaceData))
              {
                break;
              }
            }
          }
          else
          {
            if (enumDevices_failedSetupDiGetDeviceInterfaceDetail && !enumDevices_failedSetupDiGetDeviceInterfaceDetail(::GetLastError(), false, DeviceInterfaceData))
            {
              break;
            }
          }
        }
        else
        {
          DWORD dwError = ::GetLastError();
          if (dwError != ERROR_NO_MORE_ITEMS)
          {
            throw_win32api_error(dwError, "SetupDiEnumDeviceInterfaces");
          }
          break;
        }

      } // for(;;)
    }


  } // namespace Win32

} // namespace WacomGSS
