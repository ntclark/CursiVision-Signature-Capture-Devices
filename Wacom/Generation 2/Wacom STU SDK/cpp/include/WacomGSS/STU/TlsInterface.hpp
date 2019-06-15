/// @file      WacomGSS/STU/TlsInterface.hpp
/// @copyright Copyright (c) 2016 Wacom Company Limited
/// @author    mholden
/// @date      2016-11-17
/// @brief     Definition of the WacomGSS::STU::TlsInterface class

#ifndef WacomGSS_STU_TlsInterface_hpp
#define WacomGSS_STU_TlsInterface_hpp

#include <WacomGSS/config.hpp>

#if defined(WacomGSS_WIN32)
# include <WacomGSS/STU/Win32/TlsInterface.hpp>
#elif defined(WacomGSS_Linux)
# include <WacomGSS/STU/Linux/TlsInterface.hpp>
/*#elif defined(WacomGSS_OSX)
# include <WacomGSS/STU/OSX/TlsInterface.hpp>*/
#else
# error WacomGSS: 'class TlsInterface': platform not implemented.
#endif

#if defined(WacomGSS_DOXYGEN)
namespace WacomGSS
{

  namespace STU
  {

    /// @brief Provides standard means of identifying a TLS enabled device
    struct TlsDevice
    {
    };

    /// @brief Retrieves the list of TLS-enabled devices
    std::vector<TlsDevice> getTlsDevices();


    /// @brief Implements Interface for communicating with an %STU tablet over TLS over USB.
    class TlsInterface : public Interface
    {
    public:
      /// @brief Default constructor.
      TlsInterface();

      /// @brief Constructor with shared WinusbDll resource. <b>Windows only</b>.
      ///
      /// @remark This class uses Winusb to support write(). In order to avoid multiple load/free 
      ///         library calls, you can preload the WinusbDll and share it with this object.
      TlsInterface(std::shared_ptr<Win32::WinusbDll> & winusb);
    
      /// @brief  Destructor.
      /// @remark Whilst every effort is made to reduce the possibility of exceptions, it 
      ///         is still possible that this can throw. To reduce the chances, call 
      ///         disconnect() prior to object destruction.
      ~TlsInterface() noexcept;

      /// @brief  Establishes a connection to a tablet.
      /// @param  tlsDevice     The device.
      /// @return std::error_code containing result of connection attempt. If a connection has 
      ///         been sucessfully established, error_code::value() returns zero and 
      ///         error_code::operator bool() returns true. If the connection attempt fails, 
      ///         operator bool() returns false and value() returns a non-zero error code.
      std::error_code connect(TlsDevice const & usbDevice);


      void disconnect() override;

      bool isConnected() const override;

      void get(uint8_t * data, size_t length) override;

      void set(uint8_t const * data, size_t length) override;

      bool supportsWrite() const override;
  
      void write(uint8_t const * data, size_t length) override;

      bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const override;

      std::uint16_t getProductId() const override;


      OpenSSL::X509 getPeerCertificate() override;

      bool isConnectedOOB() const override;

      void getOOB(uint8_t * data, size_t length) override;

      void setOOB(uint8_t const * data, size_t length) override;

      InterfaceQueue send(uint8_t const * data, size_t length, InterfaceQueue_tag) override;

      Report send(uint8_t const * data, size_t length) override;
    };



  } // namespace STU

} // namespace WacomGSS
#endif

#endif // WacomGSS_STU_TlsInterface_hpp
