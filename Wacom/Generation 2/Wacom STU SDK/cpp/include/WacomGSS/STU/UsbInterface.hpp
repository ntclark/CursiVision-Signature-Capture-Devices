/// @file      WacomGSS/STU/UsbInterface.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Definition of the WacomGSS::STU::UsbInterface class

#ifndef WacomGSS_STU_UsbInterface_hpp
#define WacomGSS_STU_UsbInterface_hpp

#include <WacomGSS/config.hpp>

#if defined(WacomGSS_WIN32)
# include <WacomGSS/STU/Win32/UsbInterface.hpp>
#elif defined(WacomGSS_Linux)
# include <WacomGSS/STU/Linux/UsbInterface.hpp>
#elif defined(WacomGSS_OSX)
# include <WacomGSS/STU/OSX/UsbInterface.hpp>
#else
# error WacomGSS: 'class UsbInterface': platform not implemented.
#endif

#if defined(WacomGSS_DOXYGEN)
namespace WacomGSS
{

  namespace STU
  {    

    /// @brief Implements Interface for communicating with an %STU tablet over the HID/USB bus.
    class UsbInterface : public Interface
    {
    public:
      /// @brief Default constructor.
      UsbInterface();

      /// @brief Constructor with shared WinusbDll resource. <b>Windows only</b>.
      ///
      /// @remark This class uses Winusb to support write(). In order to avoid multiple load/free 
      ///         library calls, you can preload the WinusbDll and share it with this object.
      UsbInterface(std::shared_ptr<Win32::WinusbDll> & winusb);
     
      /// @brief  Destructor.
      /// @remark Whilst every effort is made to reduce the possibility of exceptions, it 
      ///         is still possible that this can throw. To reduce the chances, call 
      ///         disconnect() prior to object destruction.
      ~UsbInterface() noexcept;

      /// @brief  Establishes a connection to a tablet.
      /// @param  usbDevice     The USB HID device.      
      /// @param  exclusiveLock Specifies whether to open the device for exclusive access.
      /// @return std::error_code containing result of connection attempt. If a connection has 
      ///         been sucessfully established, error_code::value() returns zero and 
      ///         error_code::operator bool() returns true. If the connection attempt fails, 
      ///         operator bool() returns false and value() returns a non-zero error code.
      ///
      /// @remark It is strongly recommended that you use exclusiveLock = true. Having a 
      ///         device accessed by multiple clients can lead to unexpected behaviour.
      std::error_code connect(UsbDevice const & usbDevice, bool exclusiveLock);

      /// @brief  Establishes a connection to a tablet. <b>Windows only</b>.
      ///
      /// @param  fileName      The fileName to the USB HID device.
      /// @param  bulkFileName  The fileName to the USB bulk device, or a blank string.
      /// @param  exclusiveLock Specifies whether to open the device for exclusive access.
      /// @return std::error_code containing result of connection attempt. If a connection has 
      ///         been sucessfully established, error_code::value() returns zero and 
      ///         error_code::operator bool() returns true. If the connection attempt fails, 
      ///         operator bool() returns false and value() returns a non-zero error code.
      ///
      /// @remark It is strongly recommended that you use exclusiveLock = true. Having a 
      ///         device accessed by multiple clients can lead to unexpected behaviour.
      std::error_code connect(std::wstring const & fileName, std::wstring const & bulkFileName, bool exclusiveLock);

      /// @brief  Establishes a connection to a tablet. <b>Windows only</b>.
      ///
      /// @param  fileName      The fileName to the USB HID device.
      /// @param  bulkFileName  The fileName to the USB bulk device, or nullptr.
      /// @param  exclusiveLock Specifies whether to open the device for exclusive access.
      /// @return std::error_code containing result of connection attempt. If a connection has 
      ///         been sucessfully established, error_code::value() returns zero and 
      ///         error_code::operator bool() returns true. If the connection attempt fails, 
      ///         operator bool() returns false and value() returns a non-zero error code.
      ///
      /// @remark It is strongly recommended that you use exclusiveLock = true. Having a device 
      ///         accessed by multiple clients can lead to unexpected behaviour.
      std::error_code connect(wchar_t const * fileName, wchar_t const * bulkFileName, bool exclusiveLock);

      void disconnect() override;

      bool isConnected() const override;

      void get(uint8_t * data, size_t length) override;

      void set(uint8_t const * data, size_t length) override;

      bool supportsWrite() const override;
    
      void write(uint8_t const * data, size_t length) override;

      bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const override;

      std::uint16_t getProductId() const override;
    };
    
  } // namespace STU

} // namespace WacomGSS
#endif

#endif // WacomGSS_STU_UsbInterface_hpp
