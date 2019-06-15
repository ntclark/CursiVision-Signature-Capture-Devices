#include <WacomGSS/STU/Win32/getSerialPorts.hpp>
#include <WacomGSS/Win32/windows.hpp>


namespace WacomGSS
{
  namespace STU
  {

    static void queryDosDevice(wchar_t const * deviceName, std::vector<wchar_t> & targetPath)
    {
      for (;;)
      {
        DWORD cch = ::QueryDosDeviceW(deviceName, targetPath.data(), targetPath.size());
        if (cch)
        {
          break;
        }

        DWORD dwError = ::GetLastError();
        if (dwError == ERROR_INSUFFICIENT_BUFFER)
        {
          auto size = targetPath.size();
          targetPath.resize(0);
          targetPath.resize(size + 4096);
        }
        else
        {
          Win32::throw_win32api_error(dwError, "QueryDosDevice");
        }
      }
    }



    std::vector<SerialPort> getSerialPorts()
    {
      std::vector<SerialPort> ret;

      std::vector<WCHAR> deviceNames(16384);
      queryDosDevice(nullptr, deviceNames);

      std::vector<WCHAR> targetPath(1024);
      for (PCWSTR deviceName = deviceNames.data(); *deviceName; deviceName += std::wcslen(deviceName) + 1)
      {
        if ((deviceName[0] == L'C' || deviceName[0] == L'c') &&
            (deviceName[1] == L'O' || deviceName[1] == L'o') &&
            (deviceName[2] == L'M' || deviceName[2] == L'm') &&
            (deviceName[3] >= L'1' && deviceName[3] <= L'9'))
        {
          queryDosDevice(deviceName, targetPath);

          SerialPort::Type type = SerialPort::Type::Unknown;

          if (std::wcsncmp(targetPath.data(), L"\\Device\\", 8) == 0)
          {
            if (std::wcsncmp(targetPath.data() + 8, L"Serial", 6) == 0)
            {
              type = SerialPort::Type::Physical;
            }
            else if (std::wcsncmp(targetPath.data() + 8, L"VCP", 3) == 0)
            {
              type = SerialPort::Type::Virtual;
            }
            else if (std::wcsncmp(targetPath.data() + 8, L"RdpDrPort", 9) == 0)
            {
              type = SerialPort::Type::Remote;
            }
          }

          if (deviceName[4] != L'\0')
          {
            // https://support.microsoft.com/en-us/kb/115831
            // HOWTO: Specify Serial Ports Larger than COM9

            std::wstring deviceName2;
            deviceName2.reserve(MAX_PATH);
            deviceName2 += L"\\\\.\\";
            deviceName2 += deviceName;
            ret.emplace_back(type, deviceName2.c_str(), targetPath.data());
          }
          else
          {
            ret.emplace_back(type, deviceName, targetPath.data());
          }
        }
      }

      return ret;
    }

  }

}

