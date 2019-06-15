/// @file      WacomGSS/STU/Win32/TlsInterface.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides the class that can open an STU tablet over the HID/USB bus.

#ifndef WacomGSS_STU_Win32_TlsInterface_hpp
#define WacomGSS_STU_Win32_TlsInterface_hpp

#include <WacomGSS/STU/TlsInterfaceImpl.hpp>
#include <WacomGSS/STU/getTlsDevices.hpp>
#include <WacomGSS/Win32/winusb.hpp>


namespace WacomGSS
{

  namespace STU
  {    

   
    class TlsInterface : public TlsInterfaceImpl
    {
      std::shared_ptr<Win32::WinusbDll> m_WinusbDll;
      Win32::Handle                     m_hFile;
      Win32::Winusb_Interface_Handle    m_hWinusb;
      Win32::Handle                     m_quitEvent;

      void stopReaderThread() override;
      void readerThread() noexcept override;
      long bioCallback_read(uint8_t * buffer, long length) override;
      long bioCallback_write(uint8_t const * buffer, long length) override;


    public:
      /// @brief  Default constructor.
      TlsInterface();

      /// @brief  Constructor with shared WinusbDll resource.
      ///
      /// @remark This class uses Winusb to support write(). In order to avoid multiple load/free 
      ///         library calls, you can preload the WinusbDll and share it with this object.
      TlsInterface(std::shared_ptr<Win32::WinusbDll> winusb);
     
      ~TlsInterface() noexcept;

      /// @brief  Connects the class to a tablet.
      ///
      /// @param  fileName      The fileName to the USB HID device.
      std::error_code connect(std::wstring const & fileName, ConnectOption option);

      /// @brief  Connects the class to a tablet.
      ///
      /// @param  fileName      The fileName to the USB HID device.
      std::error_code connect(wchar_t const * fileName, ConnectOption option);

      std::error_code connect(TlsDevice const & tlsDevice, ConnectOption option);
      

      void disconnect() override;

      //bool isConnected() const override;                  
      //void get(uint8_t * data, size_t length) override;
      //void set(uint8_t const * data, size_t length) override;            
      //bool supportsWrite() const override;    
      //void write(uint8_t const * data, __in size_t length) override;
      //bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const override;
      //std::uint16_t getProductId() const override;
      
      // new for TLS 

      //OpenSSL::X509 getPeerCertificate() override;

      bool isConnectedOOB() const override;
      void setOOB(uint8_t const * data, size_t length) override;
      void getOOB(uint8_t * data, size_t length) override;
      
      //Report send(uint8_t const * data, size_t length) override;            
      //InterfaceQueue send(uint8_t const * data, size_t length, InterfaceQueue_tag) override;
    };
      
    
  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Win32_TlsInterface_hpp
