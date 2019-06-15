#include <WacomGSS/enumUsbDevices.hpp>
#include <memory>


namespace WacomGSS
{
  static bool parseHexStrict(wchar_t c, WORD shift, WORD & value) noexcept
  {
    if (c) 
    {
      if (c >= L'0' && c <= L'9') { value |= WORD(c - '0'     )<<shift; return true; }
      if (c >= L'A' && c <= L'F') { value |= WORD(c - 'A' + 10)<<shift; return true; }
      if (c >= L'a' && c <= L'f') { value |= WORD(c - 'a' + 10)<<shift; return true; }

      // On Windows 7, it has been seen that a value of ":101" has been returned, instead of "A101".
      if (c >= L':' && c <= L'?') { value |= WORD(c - ':' + 10)<<shift; return true; }
    }
    return false;
  }
  


  static bool parseHexStrict(wchar_t const * p, WORD & value) noexcept
  {
    value = 0;        
    return p && parseHexStrict(p[0], 12, value) && parseHexStrict(p[1], 8, value) && parseHexStrict(p[2], 4, value) && parseHexStrict(p[3], 0, value);
  }



  static bool parseHardwareId(wchar_t const * pszHardwareId, WORD & vendorId, WORD & productId, WORD & versionId, bool & isMI) noexcept
  {
    // Expected string format of pszHardwareId:
    // 
    //           1         2         3 
    // 012345678901234567890123456789012345
    //
    // HID\VID_056A&PID_00A1&REV_0100
    // HID\VID_056A&PID_00A3&REV_0102&MI_00
    //

    vendorId  = 0x0000;
    productId = 0x0000;
    versionId = 0x0000;
    isMI      = false;

    if (pszHardwareId) 
    {
      int l = ::lstrlenW(pszHardwareId);
      if (l && l >= 29) 
      {
        static const WCHAR szHidVid[] = L"HID\\VID_";
        static const int   cchHidVid  = (sizeof(szHidVid)/sizeof(szHidVid[0]))-1;

        static const WCHAR szPid[] = L"&PID_";
        static const int   cchPid  = (sizeof(szPid)/sizeof(szPid[0]))-1;

        static const WCHAR szRev[] = L"&REV_";
        static const int   cchRev  = (sizeof(szRev)/sizeof(szRev[0]))-1;

        static const WCHAR szMi[] = L"&MI_";
        static const int   cchMi = (sizeof(szMi)/sizeof(szMi[0]))-1;

        if (::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, pszHardwareId, cchHidVid, szHidVid, cchHidVid) == CSTR_EQUAL) 
        {         
          pszHardwareId += cchHidVid;
          if (parseHexStrict(pszHardwareId, vendorId)) 
          {
            pszHardwareId += 4;
            if (::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, pszHardwareId, cchPid, szPid, cchPid) == CSTR_EQUAL) 
            {         
              pszHardwareId += cchPid;
              if (parseHexStrict(pszHardwareId, productId)) 
              {
                pszHardwareId += 4;
                if (::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, pszHardwareId, cchRev, szRev, cchRev) == CSTR_EQUAL) 
                {         
                  pszHardwareId += cchRev;
                  if (parseHexStrict(pszHardwareId, versionId)) 
                  {
                    pszHardwareId += 4;
                  
                    isMI = (::CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, pszHardwareId, cchMi, szMi, cchMi) == CSTR_EQUAL);

                    return true;
                  }
                }
              }
            }
          }
        }
      }
    }
    return false;
  }
  

  bool (* enumUsbDevices_unhandledParseHardwareId)(wchar_t const * hardwareID, wchar_t const * fileName);
  bool (* enumUsbDevices_failedSetupDiGetDeviceRegistryProperty)(DWORD dwError, bool getData, SP_DEVINFO_DATA const &, SP_DEVICE_INTERFACE_DETAIL_DATA const *);
  

  
  void enumUsbDevices(std::function<bool(UsbDevice & usbDevice)> f)
  {
    // Hard code guid as per https://msdn.microsoft.com/en-us/library/windows/hardware/ff545860(v=vs.85).aspx
    // This saves DDK and linking to HID.LIB
    GUID guidHid{ 0x4D1E55B2, 0xF16F, 0x11CF,{ 0x88,0xCB, 0x00,0x11,0x11,0x00,0x00,0x30 } }; // = GUID_DEVINTERFACE_HID = {4D1E55B2-F16F-11CF-88CB-001111000030}
                                                                                             //HidD_GetHidGuid(&guidHid);

    Win32::enumDevices(guidHid, [&f](Win32::HDevInfo & hDevInfo, SP_DEVICE_INTERFACE_DATA &, SP_DEVINFO_DATA & DevInfoData, PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetailData)->bool
    {
      DWORD dwSize = 0;
      if (::SetupDiGetDeviceRegistryProperty(hDevInfo.get(), &DevInfoData, SPDRP_HARDWAREID, 0, 0, 0, &dwSize) || GetLastError() == ERROR_INSUFFICIENT_BUFFER)
      {
        // Although dwSize is returned in bytes, we are allocating in WCHAR due to an error in Windows 2000,
        // referred to at: http://support.microsoft.com/kb/259695
        std::unique_ptr<WCHAR[]> pHardwareId(new WCHAR[dwSize]);
        dwSize *= sizeof(WCHAR);

        if (::SetupDiGetDeviceRegistryProperty(hDevInfo.get(), &DevInfoData, SPDRP_HARDWAREID, 0, reinterpret_cast<PBYTE>(pHardwareId.get()), dwSize, 0))
        {
          UsbDevice usbDevice;
          if (parseHardwareId(pHardwareId.get(), usbDevice.idVendor, usbDevice.idProduct, usbDevice.bcdDevice, usbDevice.isMI))
          {
//
//NTC: 05-11-2019: Again the assholes assume everything is unicode
//
#if 1
            usbDevice.fileName = pDeviceInterfaceDetailData->DevicePath;
#else
            WCHAR szTemp[MAX_PATH];
            MultiByteToWideChar(CP_ACP,0,pDeviceInterfaceDetailData->DevicePath,-1,szTemp,MAX_PATH);
            usbDevice.fileName = szTemp;
#endif
            usbDevice.devInst = DevInfoData.DevInst;

            if (!f(usbDevice))
            {
              return false;
            }
          }
          else
          {
//
//NTC: It would seem these assholes assumed unicode implementations are all that might exist.
//
#if 1
               return false;
#else
            if (enumUsbDevices_unhandledParseHardwareId && !enumUsbDevices_unhandledParseHardwareId(pHardwareId.get(), pDeviceInterfaceDetailData->DevicePath))
            {
              return false;
            }
#endif
          }
        }
        else
        {
          if (enumUsbDevices_failedSetupDiGetDeviceRegistryProperty && !enumUsbDevices_failedSetupDiGetDeviceRegistryProperty(::GetLastError(), true, DevInfoData, pDeviceInterfaceDetailData))
          {
            return false;
          }
        }
      }
      else
      {
        if (enumUsbDevices_failedSetupDiGetDeviceRegistryProperty && !enumUsbDevices_failedSetupDiGetDeviceRegistryProperty(::GetLastError(), false, DevInfoData, pDeviceInterfaceDetailData))
        {
          return false;
        }
      }

      return true;
    } );

  }

} // namespace WacomGSS

