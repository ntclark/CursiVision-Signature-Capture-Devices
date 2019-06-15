/// @file      WacomGSS/STU/SerialInterface.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Definition of the WacomGSS::STU::SerialInterface class

#ifndef WacomGSS_STU_SerialInterface_hpp
#define WacomGSS_STU_SerialInterface_hpp

#include <WacomGSS/config.hpp>

#if defined(WacomGSS_WIN32)
# include <WacomGSS/STU/Win32/SerialInterface.hpp>
#elif defined(WacomGSS_Linux)
# include <WacomGSS/STU/Linux/SerialInterface.hpp>
#elif defined(WacomGSS_OSX)
//# include <WacomGSS/STU/OSX/SerialInterface.hpp>
// No serial support in OSX
#else
# error WacomGSS: 'class SerialInterface': platform not implemented.
#endif

#if defined(WacomGSS_DOXYGEN)
namespace WacomGSS
{
  namespace STU
  {    
    /// @brief Implements Interface for communicating with an %STU tablet over the serial bus.
    class SerialInterface : public Interface
    {
    public:
      /// @brief Default constructor.
      SerialInterface();

      /// @brief Destructor.
      ~SerialInterface() noexcept;

      static const std::uint32_t BaudRate_STU500  = 115200u; ///< @ Connection speed for STU-500
      static const std::uint32_t BaudRate_STU430V = 128000u; ///< @ Connection speed for STU-430V

      /// @brief Establishes a connection to a tablet.
      ///
      /// @param fileName The fileName to the COM port to use device.
      /// @param baudRate The speed to connect at.
      /// @param useCrc   Specifies whether to protect sent data with a CRC.
      ///
      /// @remark It is strongly recommended that you use useCrc = true.
      std::error_code connect(std::wstring const & fileName, std::uint32_t baudRate, bool useCrc);

      /// @brief Establishes a connection to a tablet.
      ///
      /// @param fileName The fileName to the COM port to use device.
      /// @param baudRate The speed to connect at.
      /// @param useCrc   Specifies whether to protect sent data with a CRC.
      ///
      /// @remark It is strongly recommended that you use useCrc = true.
      std::error_code connect(wchar_t const * fileName, std::uint32_t baudRate, bool useCrc);
    
      void disconnect() override;

      bool isConnected() const override;

      void get(std::uint8_t * data, std::size_t length) override;

      void set(std::uint8_t const * data, std::size_t length) override;

      bool supportsWrite() const override;
     
      void write(std::uint8_t const * data, std::size_t length) override;

      bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const override;

      std::uint16_t getProductId() const override;
    };
    
  } // namespace STU

} // namespace WacomGSS
#endif

#endif // WacomGSS_STU_SerialInterface_hpp
