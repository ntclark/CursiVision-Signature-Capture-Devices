/// @file      WacomGSS/STU/SerialInterface.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides the class that can open an STU tablet over the serial bus.

#ifndef WacomGSS_STU_Win32_SerialInterface_hpp
#define WacomGSS_STU_Win32_SerialInterface_hpp

#include <WacomGSS/STU/Interface.hpp>
#include <WacomGSS/Win32/windows.hpp>
#include <WacomGSS/compatibility/thread.hpp>
#include <string>


namespace WacomGSS
{

  namespace STU
  {    


    class SerialInterface : public Interface
    {
      mutex                 m_apiMutex;
      bool                  m_useCrc;
      chrono::milliseconds  m_timeout;
      Win32::Handle         m_hFile;
      
      Win32::Handle         m_quitEvent;
      thread                m_readerThread;

      void startReaderThread();
      void stopReaderThread();

      void get_or_set(uint8_t const * inputData, size_t inputLength, uint8_t * outputData, size_t outputLength);
      void send(std::vector<uint8_t> const & encodedReport);

      void readerThread() noexcept;
      void processReadBuffer(std::vector<uint8_t> & readBuffer);
      void processDecodedReport(Report && decodedReport);

    public:
      SerialInterface();

      ~SerialInterface() noexcept;

      static const std::uint32_t BaudRate_STU500 = 115200u;
      static const std::uint32_t BaudRate_STU430 = 128000u;

      std::error_code connect(std::wstring const & fileName, std::uint32_t baudRate, bool useCrc);

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

#endif // WacomGSS_STU_Win32_SerialInterface_hpp
