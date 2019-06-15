#include <WacomGSS/STU/SerialInterface.hpp>
#include <WacomGSS/STU/SerialProtocol.hpp>
#include <WacomGSS/STU/Protocol.hpp>
#include <WacomGSS/STU/InterfaceQueue.hpp>
#include <WacomGSS/STU/getUsbDevices.hpp>
#include <WacomGSS/setThreadName.hpp>

#include <array>
#include <sstream>
#include <iostream>
#include <iomanip>



namespace WacomGSS
{
  namespace STU
  {    


    SerialInterface::SerialInterface()
    :
      m_useCrc(true),
      m_timeout(3000),
      m_quitEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr))
    {
    }



    SerialInterface::~SerialInterface() noexcept
    {
      // see http://www.gotw.ca/gotw/047.htm
      try
      {
        disconnect();
      }
      catch (...)
      {
      }
    }



    void SerialInterface::startReaderThread()
    {
      stopReaderThread();

      m_readerThread = std::move(thread(std::bind(std::mem_fn(&SerialInterface::readerThread), this)));
    }


    
    void SerialInterface::stopReaderThread()
    {
      Win32::win32api_BOOL(::SetEvent(m_quitEvent.get()));
      if (m_readerThread.joinable())
        m_readerThread.join();
      Win32::win32api_BOOL(::ResetEvent(m_quitEvent.get()));
      
      queueClear();
    }
    
    
    std::error_code SerialInterface::connect(std::wstring const & fileName, std::uint32_t baudRate, bool useCrc)
    {
      return connect(fileName.c_str(), baudRate, useCrc);
    }



    static std::vector<uint8_t>::const_iterator getReportStart(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
    {
      while (begin != end && !SerialProtocol::isStartReport(*begin)) 
      {
        ++begin;
      }
      return begin;
    }

    void SerialInterface::processReadBuffer(std::vector<uint8_t> & pReadBuffer)
    {
      // See if the buffer contains at least one report start flag
      auto start = getReportStart(pReadBuffer.begin(), pReadBuffer.end());

      if (start != pReadBuffer.end()) 
      {
        do
        {
          // get the next byte (if available)
          auto data = start + 1;
          if (data != pReadBuffer.end()) 
          {
            // the buffer contains at least the header, we can decode what the header says the report size should be.

            int decodedEncodedDataLength = SerialProtocol::decodeEncodedDataLength(*start, *data);

            // search for the next start of the next report. If data has been corrupted, this may be before, 
            // at or after the reported length

            auto next = getReportStart(data, pReadBuffer.end());

            ++data;
            if (data != pReadBuffer.end()) 
            {
              // next may be the start of the next Report, or may equal end()
            
              if (next-data >= decodedEncodedDataLength) 
              {
                // a new report does not start before the reported length of the current Report.
                Report decodedReport;
                SerialProtocol::decodeData(data, next, decodedReport);

                if (!SerialProtocol::decodeHasCrc(*start) || SerialProtocol::checkCrcAndRemove(decodedReport))
                {
                  queueReport(std::move(decodedReport));
                }
                else 
                {
                  // CRC ERROR
                  // should really queue a per-report throw here.
                }
                
                // set the start of the next report. This maybe at end().
                start = next;
              }
              else if (next != pReadBuffer.end()) 
              {
                // a new Report was found, set the start to it.
                start = next;
              }
              else
              {
                //  we did not find the start of a new report.
                // start == end()
                break;
              }
            }
            else 
            {
              // the packet start is at the end of the buffer and we haven't read the rest of the header yet.
              break;
            }
          }
          else
          {
            // got header bytes but no data yet
            break;
          }
        }
        while (start != pReadBuffer.end());

        // remove everything from the start of the buffer upto the start of the last 
        pReadBuffer.erase(pReadBuffer.begin(), start);
      }
      else 
      {
        // drop all data
        pReadBuffer.clear();
      }
    }

    // When existing from the reader thread, it is vital to ensure that overlapped IO 
    // has completed before releasing data buffers.
    // also see UsbInterface.cpp
    static void cancelIo(Win32::Handle & hFile, std::unique_ptr<OVERLAPPED> & o, std::vector<uint8_t> & readBuffer) noexcept
    {
      if (hFile)
      {
        BOOL success = ::CancelIo(hFile.get());
        if (success)
        {
          DWORD dwBytes = 0;
          success = ::GetOverlappedResult(hFile.get(), o.get(), &dwBytes, TRUE);
          if (!success)
          {
            DWORD dwError = ::GetLastError();
            if (dwError != ERROR_SUCCESS && dwError != ERROR_OPERATION_ABORTED)
            {
              // worst case - unknown error getting result of cancelled IO.
              // deliberately leak the associated memory to avoid data corruption.
              new std::vector<uint8_t>(std::move(readBuffer));
              o.release();
            }
          }
        }
        else
        {
          DWORD dwError = ::GetLastError();
          if (dwError != ERROR_NOT_FOUND)
          {
            // worst case - unknown error trying to cancel IO.
            // deliberately leak the associated memory to avoid data corruption.
            new std::vector<uint8_t>(std::move(readBuffer));
            o.release();
          }
        }
      }
    }

    void SerialInterface::readerThread() noexcept
    {
      setThreadName("SerialInterface::readerThread");
      try
      {
        std::vector<uint8_t> readBuffer;
        Win32::Handle hReadEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr));

        Win32::win32api_BOOL(::SetCommMask(m_hFile.get(), EV_RXCHAR|EV_ERR));

        Win32::win32api_BOOL(::EscapeCommFunction(m_hFile.get(), SETRTS));

        std::unique_ptr<OVERLAPPED> o(new OVERLAPPED);
        for (;;)
        {
          ZeroMemory(o.get(), sizeof(OVERLAPPED));
          o->hEvent = hReadEvent.get();

          DWORD dwEvtMask;

          if (::WaitCommEvent(m_hFile.get(), &dwEvtMask, o.get())) 
          {            
            DWORD dwWait = ::WaitForSingleObjectEx(m_quitEvent.get(), 0, FALSE);
            if (dwWait == WAIT_OBJECT_0+0) 
            {
              // quit signaled
              cancelIo(m_hFile, o, readBuffer);
              break;
            }
            else if (dwWait != WAIT_TIMEOUT) 
            {
              DWORD dwError = ::GetLastError();
              cancelIo(m_hFile, o, readBuffer);
              Win32::throw_win32api_error(dwError, "WaitForSingleObjectEx");
            }
          } 
          else
          {
            dwEvtMask = EV_ERR;

            DWORD dwError = ::GetLastError();
            if (dwError == ERROR_IO_PENDING) 
            {
              HANDLE h[2] = { m_quitEvent.get(), hReadEvent.get() };
              DWORD dwWait = ::WaitForMultipleObjectsEx(2, h, FALSE, INFINITE, FALSE);
              if (dwWait == WAIT_OBJECT_0+0) 
              {
                // quit signaled
                cancelIo(m_hFile, o, readBuffer);
                break;
              }
              else if (dwWait == WAIT_OBJECT_0+1) 
              {
                DWORD dwBytes;
                BOOL bSuccess = ::GetOverlappedResult(m_hFile.get(), o.get(), &dwBytes, TRUE);                
                if (bSuccess)                 
                {
                  dwEvtMask = EV_RXCHAR;
                }
                else
                {
                  Win32::throw_win32api_error(::GetLastError(), "GetOverlappedResult");
                }
              } 
              else 
              {
                dwError = ::GetLastError();
                cancelIo(m_hFile, o, readBuffer);
                Win32::throw_win32api_error(dwError, "WaitForMultipleObjectsEx");
              }
            }
            else 
            {
              cancelIo(m_hFile, o, readBuffer);
              Win32::throw_win32api_error(dwError, "WaitCommEvent");
            }
          }

          if ((dwEvtMask & EV_ERR) != 0) 
          {
            Win32::win32api_BOOL(::PurgeComm(m_hFile.get(), PURGE_RXABORT|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_TXCLEAR));
          } 

          COMSTAT comStat;
          DWORD   dwError;
          Win32::win32api_BOOL(::ClearCommError(m_hFile.get(), &dwError, &comStat));

          if (dwError == 0 && (dwEvtMask & EV_RXCHAR) != 0)
          {
            if (comStat.cbInQue != 0)
            {
              auto size = readBuffer.size();
              readBuffer.resize(size + comStat.cbInQue, 0x00);

              ZeroMemory(o.get(), sizeof(OVERLAPPED));
              o->hEvent = hReadEvent.get();

              BOOL bSuccess = ::ReadFile(m_hFile.get(), &readBuffer[size], comStat.cbInQue, nullptr, o.get());
              if (!bSuccess)
              {
                dwError = ::GetLastError();
                if (dwError == ERROR_IO_PENDING)
                {
                  HANDLE h[2] = { m_quitEvent.get(), hReadEvent.get() };
                  DWORD dwWait = ::WaitForMultipleObjectsEx(2, h, FALSE, INFINITE, FALSE);
                  if (dwWait == WAIT_OBJECT_0 + 0)
                  {
                    // quit signaled
                    cancelIo(m_hFile, o, readBuffer);
                    break;
                  }
                  else if (dwWait != WAIT_OBJECT_0 + 1)
                  {
                    dwError = ::GetLastError();
                    cancelIo(m_hFile, o, readBuffer);
                    Win32::throw_win32api_error(dwError);
                  }
                }
                else if (dwError == ERROR_DEVICE_NOT_CONNECTED)
                {
                  throw Interface::device_removed_error();
                }
                else
                {
                  Win32::throw_win32api_error(dwError, "ReadFile");
                }
              }

              DWORD dwBytes;
              bSuccess = ::GetOverlappedResult(m_hFile.get(), o.get(), &dwBytes, TRUE);
              if (bSuccess)
              {
                readBuffer.resize(size + dwBytes, 0x00);
                if (dwBytes)
                {
                  processReadBuffer(readBuffer);
                }
              }
              else
              {
                dwError = GetLastError();
                if (dwError == ERROR_DEVICE_NOT_CONNECTED)
                {
                  throw Interface::device_removed_error();
                }
                else
                {
                  Win32::throw_win32api_error(dwError, "GetOverlappedResult");
                }
              }
            }
            else
            {
              // RXCHAR but zero bytes read
            }
          }
          else 
          {
            //DMP(L"  !!!ClearCommError unexepected values")
            
          }

        } // for (;;)
    
        Win32::win32api_BOOL(::EscapeCommFunction(m_hFile.get(), CLRRTS));
        Win32::win32api_BOOL(::SetCommMask(m_hFile.get(), 0));
        Win32::win32api_BOOL(::PurgeComm(m_hFile.get(), PURGE_RXABORT|PURGE_RXCLEAR));
      }
      catch (...)
      {
        queueException(std::current_exception());
      }    
    }
    

    void SerialInterface::send(std::vector<uint8_t> const & encodedReport)
    {
      if (encodedReport.size() > 0xffffu)
        throw std::logic_error("too much data");

      Win32::Handle hEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr));

      OVERLAPPED o = { 0 };
      o.hEvent = hEvent.get();

      BOOL bSuccess = ::WriteFile(m_hFile.get(), &encodedReport[0], static_cast<DWORD>(encodedReport.size()), 0, &o);
      if (!bSuccess) 
      {
        DWORD dwError = ::GetLastError();
        if (dwError == ERROR_IO_PENDING)
        {
          DWORD dwWait = ::WaitForSingleObjectEx(hEvent.get(), static_cast<DWORD>(m_timeout.count()), FALSE);
          if (dwWait == WAIT_OBJECT_0 +0)
          {
            // success
          }
          else if (dwWait == WAIT_TIMEOUT)
          {
            throw io_error();
          }
          else  
          {
            dwError = ::GetLastError();
            Win32::throw_win32api_error(dwError, "WaitForSingleObjectEx");
          }
        }
        else 
        {
          Win32::throw_win32api_error(dwError,"WriteFile");
        }
      }

      DWORD dwBytes;
      bSuccess = ::GetOverlappedResult(m_hFile.get(), &o, &dwBytes, FALSE);
      if (bSuccess) 
      {
        if (dwBytes != encodedReport.size()) 
        {
          throw io_error();
        }
      }
      else 
      {
        DWORD dwError = ::GetLastError();
        Win32::throw_win32api_error(dwError, "GetOverlappedResult");
      }
    }



    static void safeCopyReport(Report const & report, std::uint8_t * outputData, std::size_t outputLength)
    {
      auto size = report.size();
      if (size > outputLength)
      {
        throw std::logic_error("buffer size too small");
      }
      else if (size < outputLength)
      {      
        std::fill_n(stdext::make_unchecked_array_iterator(outputData+size), outputLength-size, uint8_t(0));
      }      
      std::copy(report.begin(), report.end(), stdext::make_unchecked_array_iterator(outputData));
    }



    void SerialInterface::get_or_set(uint8_t const * inputData, size_t inputLength, uint8_t * outputData, size_t outputLength)
    {
      lock_guard<mutex> lock(m_apiMutex);
      
      std::vector<uint8_t> encodedReport = SerialProtocol::encodeReport(inputData, inputData+inputLength, m_useCrc);
            
      auto absTime = chrono::steady_clock::now() + m_timeout;

      auto interfaceQueue = Interface::interfaceQueue();        

      send(encodedReport);
      
      for (;;)
      {
        Report report;
        if (interfaceQueue.wait_until_getReport(report, absTime))
        {
          if (!report.empty() && report[0] == outputData[0])
          {
            safeCopyReport(report, outputData, outputLength);
            break;
          }
        }
        else
        {
          throw timeout_error();
        }          
      }
    }



    std::error_code SerialInterface::connect(wchar_t const * fileName, std::uint32_t baudRate, bool useCrc)
    {
      disconnect();

      lock_guard<mutex> lock(m_apiMutex);

      std::error_code ec;
      try
      {
        m_hFile.reset(::CreateFileW(fileName, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0));
        m_useCrc = useCrc;
        
        DCB dcb = { 0 };
#if 0
COMMCONFIG cc;
DWORD size = sizeof(COMMCONFIG);
::ZeroMemory(&cc, size);
Win32::win32api_BOOL(::GetCommConfig(m_hFile.get(), &cc, &size));
dcb = cc.dcb;
#endif

        dcb.DCBlength         = sizeof(DCB);
        dcb.BaudRate          = baudRate;
        dcb.fBinary           = 1;
      //dcb.fParity           = 0;
      //dcb.fOutxCtsFlow      = 0;
      //dcb.fOutxDsrFlow      = 0;
        dcb.fDtrControl       = DTR_CONTROL_DISABLE;
      //dcb.fDsrSensitivity   = 0;
      //dcb.fTXContinueOnXoff = 0;
      //dcb.fOutX             = 0;
      //dcb.fInX              = 0;
      //dcb.fErrorChar        = 0;
      //dcb.fNull             = 0;
        dcb.fRtsControl       = RTS_CONTROL_DISABLE;
        dcb.fAbortOnError     = 1;
      //dcb.XonLim            = 0;
      //dcb.XoffLim           = 0;
        dcb.ByteSize          = 8;
        dcb.Parity            = NOPARITY;
        dcb.StopBits          = ONESTOPBIT;
      //dcb.XonChar           = 0;
      //dcb.XoffChar          = 0;
      //dcb.ErrorChar         = 0;
      //dcb.EofChar           = 0;
      //dcb.EvtChar           = 0;      
        Win32::win32api_BOOL(::SetCommState(m_hFile.get(), &dcb) );

        COMMTIMEOUTS commTimeouts = { 0 };
      //commTimeouts.ReadIntervalTimeout         = 0;
      //commTimeouts.ReadTotalTimeoutMultiplier  = 0;
      //commTimeouts.ReadTotalTimeoutConstant    = 0;
      //commTimeouts.WriteTotalTimeoutMultiplier = 0;
      //commTimeouts.WriteTotalTimeoutConstant   = 0;
        Win32::win32api_BOOL( ::SetCommTimeouts(m_hFile.get(), &commTimeouts) );        
          
        startReaderThread();            
      }
      catch (std::system_error const & ex)
      {
        ec = ex.code();
      }

      return ec;
    }
  


    void SerialInterface::disconnect()
    {
      lock_guard<mutex> lock(m_apiMutex);

      stopReaderThread();

      m_hFile.reset();
    }



    bool SerialInterface::isConnected() const
    {
      return m_hFile;
    }



    void SerialInterface::get(uint8_t * data, size_t length)
    {
      std::array<uint8_t, 2> cmd = {{ SerialProtocol::ReportId_GetReport, data[0] }};

      get_or_set(cmd.data(), cmd.size(), data, length);
    }




    void SerialInterface::set(uint8_t const * data, size_t length)
    {
      std::array<uint8_t, 2> cmd = {{ SerialProtocol::ReportId_SetResult, 0xff }};

      get_or_set(data, length, cmd.data(), cmd.size());

      if (cmd[1] != SerialProtocol::Error_None)
        throw set_error(cmd[1]);
    }



    bool SerialInterface::supportsWrite() const
    {
      return false;
    }



    void SerialInterface::write(std::uint8_t const *, std::size_t)
    {
      throw write_not_supported_error();
    }


    bool SerialInterface::getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const
    {
      try
      {
        Protocol protocol(const_cast<SerialInterface &>(*this));
        auto reportSizeCollection = protocol.getReportSizeCollection();

        std::copy(reportSizeCollection.begin(), reportSizeCollection.end(), reportCountLengths.begin());
        return true;
      }
      catch (runtime_error &)
      {
        // Taken from STU-500 firmware 1.04
        static const std::array<std::uint16_t, 256> data =
        { {
            0, 0, 0, 5, 2, 0, 0, 0, 17, 17, 5, 0, 0, 0, 0, 0,
            0, 0, 0, 17, 17, 5, 2, 0, 0, 0, 17, 3, 0, 0, 0, 0,
            2, 2, 5, 0, 0, 2, 256, 2, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            5, 0, 5, 17, 0, 17, 64, 2, 2, 17, 3, 12, 2, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          } };
        std::copy(data.begin(), data.end(), reportCountLengths.begin());
        return true;
      }
    }
    
    std::uint16_t SerialInterface::getProductId() const
    {
      try
      {
        Protocol protocol(const_cast<SerialInterface &>(*this));
        
        auto hidInformation = protocol.getHidInformation();

        return hidInformation.idProduct;
      }
      catch (runtime_error &)
      {
        // Only the STU-500 supports serial interface.
        return ProductId_500;
      }
    }


    
  } // namespace STU
  
} // namespace WacomGSS

