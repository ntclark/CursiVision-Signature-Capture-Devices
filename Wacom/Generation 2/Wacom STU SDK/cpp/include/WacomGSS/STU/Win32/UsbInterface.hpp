/// @file      WacomGSS/STU/Win32/UsbInterface.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides the class that can open an STU tablet over the HID/USB bus.

#ifndef WacomGSS_STU_Win32_UsbInterface_hpp
#define WacomGSS_STU_Win32_UsbInterface_hpp

#include <WacomGSS/compatibility/thread.hpp>
#include <WacomGSS/STU/getUsbDevices.hpp>
#include <WacomGSS/STU/Interface.hpp>
#include <WacomGSS/Win32/winusb.hpp>


namespace WacomGSS
{

  namespace STU
  {    

    class UsbInterface : public Interface
    {
      mutex mutable                     m_apiMutex;
      Win32::Handle                     m_hFile;
      Win32::Handle                     m_hBulk;
      std::shared_ptr<Win32::WinusbDll> m_WinusbDll;
      Win32::Winusb_Interface_Handle    m_hWinusb;
      std::uint8_t                      m_bulkOutPipeId;
      Win32::Handle                     m_quitEvent;
      thread                            m_readerThread;
      std::array<std::uint16_t, 256>    m_inputLengths;
      std::uint16_t                     m_maxInputLength;
      std::uint16_t                     m_idProduct;

      void cacheData();

      void startReaderThread();
      void stopReaderThread();
      void readerThread() noexcept;
      void processReadBuffer(std::vector<std::uint8_t>::const_iterator begin, std::vector<std::uint8_t>::const_iterator end);

      std::error_code connect(bool win81sleep, wchar_t const * fileName, wchar_t const * bulkFileName, bool exclusiveLock);

    public:
      /// @brief  Default constructor.
      UsbInterface();

      /// @brief  Constructor with shared WinusbDll resource.
      ///
      /// @remark This class uses Winusb to support write(). In order to avoid multiple load/free 
      ///         library calls, you can preload the WinusbDll and share it with this object.
      UsbInterface(std::shared_ptr<Win32::WinusbDll> & winusb);
     
      /// @brief  Destructor.
      ~UsbInterface() noexcept;

      /// @brief  Connects the class to a tablet.
      ///
      /// @param  fileName      The fileName to the USB HID device.
      /// @param  bulkFileName  The fileName to the USB bulk device, or a blank string.
      /// @param  exclusiveLock Specifies whether to open the device for exclusive access.
      ///
      /// @remark It is strongly recommended that you use exclusiveLock = true. Having a device accessed by multiple clients can lead to unexpected behaviour.
      std::error_code connect(std::wstring const & fileName, std::wstring const & bulkFileName, bool exclusiveLock);

      /// @brief  Connects the class to a tablet.
      ///
      /// @param  fileName      The fileName to the USB HID device.
      /// @param  bulkFileName  The fileName to the USB bulk device, or nullptr.
      /// @param  exclusiveLock Specifies whether to open the device for exclusive access.
      ///
      /// @remark It is strongly recommended that you use exclusiveLock = true. Having a device accessed by multiple clients can lead to unexpected behaviour.
      std::error_code connect(wchar_t const * fileName, wchar_t const * bulkFileName, bool exclusiveLock);

      /// @brief  Connects the class to a tablet.
      /// @param  usbDevice     The USB HID device.      
      /// @param  exclusiveLock Specifies whether to open the device for exclusive access.
      ///
      /// @remark It is strongly recommended that you use exclusiveLock = true. Having a device accessed by multiple clients can lead to unexpected behaviour.
      std::error_code connect(UsbDevice const & usbDevice, bool exclusiveLock);

      void disconnect() override;

      bool isConnected() const override;

      void get(uint8_t * data, __in size_t length) override;

      void set(uint8_t const * data, __in size_t length) override;

      bool supportsWrite() const override;
    
      void write(uint8_t const * data, __in size_t length) override;

      bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const override;

      std::uint16_t getProductId() const override;
    };
    
  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Win32_UsbInterface_hpp
