#include <WacomGSS/STU/getTlsDevices.hpp>

#pragma warning(push, 3)
#include <iostream>

#if defined(WacomGSS_WIN32)
#include <WacomGSS/Win32/windows.hpp>
#include <WacomGSS/Win32/wstringConvert.hpp>
#endif

#pragma warning(pop)

namespace WacomGSS
{
  namespace STU
  {

    namespace ostream_operators
    {

#if defined(WacomGSS_WIN32)
      std::ostream & operator << (std::ostream & o, TlsDevice const & tlsDevice)
      {
        std::string deviceName = Win32::wstring_to_string(tlsDevice.deviceName);
        o << deviceName;
        return o;
      }
#elif defined(WacomGSS_Linux)
      std::ostream & operator << (std::ostream & o, TlsDevice const & tlsDevice)
      {
        auto flags = o.flags(std::ios_base::hex);
        auto fill = o.fill('0');

        auto width = o.width(4);
        o << tlsDevice.idVendor  << ':';
        o.width(4);
        o << tlsDevice.idProduct << ':';
        o.width(4);
        o << tlsDevice.bcdDevice;

        o.width(width);
        o.flags(flags);
        o.fill(fill);

        return o;
      }
#endif

    } // namespace ostream_operators

  } // namespace STU

} // namespace WacomGSS

