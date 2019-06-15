#include <WacomGSS/Win32/getDevicePowerState.hpp>
#include <WacomGSS/Win32/setupapi.hpp>
#define INITGUID
#include <devpkey.h>
#undef INITGUID

#include <memory>


namespace WacomGSS
{
  namespace Win32
  {

    // This could have been a one line function using CM_Get_DevNode_Property(), but Microsoft
    // decided that is "reserved for system use", so we have to convert the DEVINST back to 
    // a HDEVINFO+SP_DEVINFO_DATA pair.
    // see:
    // SetupDiGetDeviceRegistryProperty  - http://msdn.microsoft.com/en-us/library/windows/hardware/ff551967(v=vs.85).aspx
    // CM_Get_DevNode_Property           - http://msdn.microsoft.com/en-us/library/windows/hardware/hh780220(v=vs.85).aspx
    //
    // SetupDiGetDeviceProperty (Vista+) - http://msdn.microsoft.com/en-us/library/windows/hardware/ff551963(v=vs.85).aspx
    //
    ::DEVICE_POWER_STATE getDevicePowerState(::DEVINST devInst)
    {
      ULONG deviceID_size = 0; // in chars, excluding null terminator
      win32api_CONFIGRET( ::CM_Get_Device_ID_Size(&deviceID_size, devInst, 0), "CM_Get_Device_ID_Size" );

      std::unique_ptr<WCHAR[]> deviceID(new WCHAR[deviceID_size+1]); // allow to null terminate
      win32api_CONFIGRET( ::CM_Get_Device_ID(devInst, deviceID.get(), deviceID_size, 0), "CM_Get_Device_ID" );
      deviceID[deviceID_size] = 0; // null terminate string

      HDevInfo devInfo(SetupDiCreateDeviceInfoList(nullptr, nullptr));

      ::SP_DEVINFO_DATA deviceInfoData = { sizeof(::SP_DEVINFO_DATA) };
      win32api_BOOL( ::SetupDiOpenDeviceInfo(devInfo.get(), deviceID.get(), nullptr, 0, &deviceInfoData), "SetupDiOpenDeviceInfo"  );

      ::CM_POWER_DATA powerData = { 0 };
      
      //::DEVPROPTYPE propertyType = 0;
      //win32api_BOOL( ::SetupDiGetDeviceProperty(devInfo.get(), &deviceInfoData, &::DEVPKEY_Device_PowerData, &propertyType, reinterpret_cast<PBYTE>(&powerData), sizeof(powerData),nullptr, 0), "SetupDiGetDeviceProperty" );

      win32api_BOOL( ::SetupDiGetDeviceRegistryProperty(devInfo.get(), &deviceInfoData, SPDRP_DEVICE_POWER_DATA, nullptr, reinterpret_cast<PBYTE>(&powerData), sizeof(powerData), nullptr),  "SetupDiGetDeviceRegistryProperty");

      return powerData.PD_MostRecentPowerState;
    }



  }
}

