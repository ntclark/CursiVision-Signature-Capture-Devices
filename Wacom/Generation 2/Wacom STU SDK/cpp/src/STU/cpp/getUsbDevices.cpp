#include <WacomGSS/STU/getUsbDevices.hpp>
#include <iostream>

namespace WacomGSS
{
  namespace STU
  {

    namespace ostream_operators
    {

      std::ostream & operator << (std::ostream & o, UsbDeviceBase const & usbDevice)
      {
        auto flags = o.flags(std::ios_base::hex);
        auto fill = o.fill('0');

        auto width = o.width(4);
        o << usbDevice.idVendor  << ':';
        o.width(4);
        o << usbDevice.idProduct << ':';
        o.width(4);
        o << usbDevice.bcdDevice;

        o.width(width);
        o.flags(flags);
        o.fill(fill);

        return o;
      }


    } // namespace ostream_operators

  } // namespace STU

} // namespace WacomGSS

