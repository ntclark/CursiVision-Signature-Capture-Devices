#include <WacomGSS/Win32/getFileVersion.hpp>
#include <WacomGSS/Win32/windows.hpp>
#include <memory>

namespace WacomGSS
{
  namespace Win32
  {


    std::uint64_t getFileVersion(wchar_t const * fileName)
    {
      // Note this does not guard against the race condition of the file changing between requesting the size and retrieving the data.

      DWORD size = ::GetFileVersionInfoSizeW(fileName, nullptr);
      win32api_bool(size != 0, "GetFileVersionInfoSize");

      std::unique_ptr<BYTE[]> data(new BYTE[size]);

      win32api_BOOL(::GetFileVersionInfoW(fileName, 0, size, data.get()), "GetFileVersionInfoSize");

      VS_FIXEDFILEINFO * vsFFI = nullptr;
      UINT len = 0;
      win32api_BOOL(::VerQueryValueW(data.get(), L"\\", reinterpret_cast<LPVOID *>(&vsFFI), &len), "VerQueryValueW(\"\\\")");

      return static_cast<std::uint64_t>(vsFFI->dwFileVersionMS) << 32 | vsFFI->dwFileVersionLS;
    }


  }
}

