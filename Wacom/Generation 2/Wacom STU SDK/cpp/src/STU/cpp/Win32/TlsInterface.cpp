#include <WacomGSS/STU/Win32/TlsInterface.hpp>
#include <WacomGSS/STU/TlsProtocol.hpp>
#include <WacomGSS/setThreadName.hpp>
#include <array>


namespace WacomGSS
{
  namespace STU
  {


    void TlsInterface::readerThread() noexcept
    {
      setThreadName("TlsInterface::readerThread");
         
      try
      {
        // Using a try/catch clause seems extremely excessive, but will be noticed by a debugger if it fails.
        try
        {
          Win32::win32api_BOOL(SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL));
        }
        catch (std::system_error &)
        {
        }

        std::vector<uint8_t> readBuffer(4096);
        size_t offset = 0;
        for (;;)
        {
          DWORD dwWait = ::WaitForSingleObjectEx(m_quitEvent.get(), 0, FALSE);

          if (dwWait == WAIT_OBJECT_0+0) 
          {
            // quit signaled
            break;
          }
          else if (dwWait != WAIT_TIMEOUT) 
          {
            Win32::throw_win32api_error(::GetLastError(), "WaitForSingleObjectEx");
          }
                    
          int len = OpenSSL::SSL_read(m_ssl, readBuffer.data()+offset, (int)(readBuffer.size()-offset));
          if (len > 0)
          {
            if (offset + len >= 6)
            {
              processReadBuffer(readBuffer.begin(), readBuffer.begin() + offset + static_cast<ptrdiff_t>(len));
              offset = 0;
            }
            else
            {
              offset += len;
            }
          }
          else
          {            
            if (len == k_bioCallback_read_exception)
            {       
              OutputDebugString(L"SSL_read ret == k_bioCallback_read_exception\r\n");
              if (m_bioCallback_read_exception)
              {
                queueException(std::move(m_bioCallback_read_exception));
              }
              break;
            }
            if (len < 0)
            {
              OutputDebugString(L"SSL_read ret < 0\r\n");
            }
            else
            {
              OutputDebugString(L"SSL_read ret == 0\r\n");
            }
          }
        }
      }
      catch (...)
      {
        queueException(std::current_exception());
      }    
    }



    void TlsInterface::stopReaderThread()
    {
      Win32::win32api_BOOL(::SetEvent(m_quitEvent.get()));
      m_readerThreadRunning.store(false);
      if (m_hWinusb)
      {
        if (!m_hWinusb.AbortPipe(m_inPipeId))
        {
          DWORD dwError = ::GetLastError();
          if (dwError != ERROR_BAD_COMMAND)
          {
            Win32::throw_win32api_error(dwError, "AbortPipe");
          }
        }        
      }
      
      if (m_readerThread.joinable())
        m_readerThread.join();
      Win32::win32api_BOOL(::ResetEvent(m_quitEvent.get()));
      queueClear();
    }



    TlsInterface::TlsInterface()
    :
      TlsInterface(std::make_shared<Win32::WinusbDll>())
    {
    }



    TlsInterface::TlsInterface(std::shared_ptr<Win32::WinusbDll> winusb)
    :
      m_WinusbDll(winusb),
      m_hWinusb(m_WinusbDll),
      m_quitEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr))
    {
      m_readerThreadRunning.store(false);
    }
    



    TlsInterface::~TlsInterface() noexcept
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
    


    std::error_code TlsInterface::connect(std::wstring const & tlsDevice, ConnectOption connectOption)
    {
      return connect(tlsDevice.c_str(), connectOption);
    }



    std::error_code TlsInterface::connect(TlsDevice const & tlsDevice, ConnectOption connectOption)
    {
      return connect(tlsDevice.deviceName, connectOption);
    }



    long TlsInterface::bioCallback_read(uint8_t * buffer, long length)
    {
      long retVal;
      try
      {
        m_bioCallback_read_exception = nullptr;

        Win32::Handle hEvent(::CreateEvent(nullptr, true, FALSE, nullptr));
        OVERLAPPED o{ 0 };
        o.hEvent = hEvent.get();

        DWORD dwError;        
        BOOL b = m_hWinusb.ReadPipe(m_inPipeId, buffer, (ULONG)length, nullptr, &o);
        m_readerThreadRunning.store(true);
        if (!b)
        {
          dwError = ::GetLastError();

          if (dwError == ERROR_OPERATION_ABORTED)
          {            
            return k_bioCallback_read_exception;            
          }          
          else if (dwError == ERROR_GEN_FAILURE)
          {
            throw Interface::device_removed_error();
          }
          else if (dwError != ERROR_IO_PENDING)
          {
            Win32::throw_win32api_error(dwError, "WinUsb_ReadPipe");
          }          
        }
    
        DWORD dwBytesTransferred = 0;
        b = m_hWinusb.GetOverlappedResult(&o, &dwBytesTransferred, TRUE);
        if (!b)
        {
          dwError = ::GetLastError();
          if (dwError == ERROR_OPERATION_ABORTED)
          {
            return k_bioCallback_read_exception; // aborted from main thread
          }
          else if (dwError == ERROR_GEN_FAILURE)
          {
            throw Interface::device_removed_error();
          }
          Win32::throw_win32api_error(dwError, "WinUsb_GetOverlappedResult");
        }
        
        retVal = (long)dwBytesTransferred;
      }
      catch (...)
      {        
        retVal = k_bioCallback_read_exception;
        m_bioCallback_read_exception = std::current_exception();
      }
      return retVal;
    }



    long TlsInterface::bioCallback_write(uint8_t const * buffer, long length)
    {
      long retVal;
      try
      {
        m_bioCallback_write_exception = nullptr;

        Win32::Handle hEvent(::CreateEvent(nullptr, true, FALSE, nullptr));
        OVERLAPPED o{ 0 };
        o.hEvent = hEvent.get();

        DWORD dwError;        
        BOOL b = m_hWinusb.WritePipe(m_outPipeId, (PUCHAR)buffer, (ULONG)length, nullptr, &o);
        if (!b)
        {
          dwError = ::GetLastError();
          if (dwError == ERROR_GEN_FAILURE)
          {
            throw Interface::device_removed_error();
          }
          else if (dwError != ERROR_IO_PENDING)
          {
            Win32::throw_win32api_error(dwError, "WinUsb_WritePipe");
          }
        }
        DWORD dwBytesTransferred = 0;        
        if (!m_hWinusb.GetOverlappedResult(&o, &dwBytesTransferred, TRUE))
        {
          dwError = ::GetLastError();
          if (dwError == ERROR_GEN_FAILURE)
          {
            throw Interface::device_removed_error();
          }
          Win32::throw_win32api_error(dwError, "WinUsb_GetOverlappedResult");
        }        
        retVal = (long)dwBytesTransferred;
      }
      catch (...)
      {
        retVal = k_bioCallback_write_exception;
        m_bioCallback_write_exception = std::current_exception();
      }
      return retVal;
    }



    std::error_code TlsInterface::connect(wchar_t const * fileName, ConnectOption connectOption)
    {
      //auto devicePowerState = Win32::getDevicePowerState(usbDevice.devInst);
      //bool win81sleep = (devicePowerState != PowerDeviceD0);

      disconnect();

      std::lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

      if (!fileName) 
      {
        throw std::logic_error("nullptr passed to TlsInterface::connect");
      }

      if (!m_WinusbDll->isLoaded() && !m_WinusbDll->loadLibrary())
      {
        throw std::runtime_error("winusb not loaded");
      }

      std::error_code ec;
      try
      {
        DWORD dwShareMode = 0U; 
    
        m_hFile.reset(::CreateFileW(fileName, GENERIC_READ|GENERIC_WRITE, dwShareMode, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0));
        // successfully opened device!

        Win32::win32api_BOOL(m_WinusbDll->Initialize(m_hFile.get(), m_hWinusb.initialize(nothrow)), "WinUsb_Initialize");

        if (connectOption == ConnectOption_SSL)
        {
          connect2();

          startReaderThread();          
        }                
      }
      catch (std::system_error & ex)
      {
        ec = ex.code();

        m_hWinusb.reset();
        m_hFile.reset();
      }
      catch (...)
      {
        m_hWinusb.reset();
        m_hFile.reset();
        throw;
      }
      
      return ec;
    }



    void TlsInterface::disconnect()
    {
      std::lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
    
      stopReaderThread();
          
      m_hWinusb.reset();
      m_hFile.reset();
      m_sslConnect = 0;
    }



    bool TlsInterface::isConnectedOOB() const
    {
      return m_hFile;
    }



    void TlsInterface::getOOB(uint8_t * data, __in size_t length)
    {
      std::lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

      if (m_hFile)
      {
        // data must not be null
        // length must be greater than zero and less than 4KB, see https://msdn.microsoft.com/en-us/library/windows/hardware/ff540219(v=vs.85).aspx

        WINUSB_SETUP_PACKET setupPacket
        {
          TlsProtocolOOB::ControlTransfer_RequestType_Get, // UCHAR RequestType 
          TlsProtocolOOB::ControlTransfer_Request,    // UCHAR Request
          data[0], // USHORT Value == report id
          TlsProtocolOOB::ControlTransfer_Index,  // USHORT Index
          static_cast<USHORT>(length & 0xffffu) // USHORT Length
        };

        DWORD dwTransferred = 0;
        Win32::win32api_BOOL(m_hWinusb.ControlTransfer(setupPacket, const_cast<std::uint8_t *>(data), length, &dwTransferred, nullptr), "Winusb_ControlTransfer");

        if (dwTransferred != length)
        {
          throw std::runtime_error("Winusb_ControlTransfer - unexpected transmission loss");
        }
      }
      else
      {
        throw not_connected_error();
      }
    }



    void TlsInterface::setOOB(uint8_t const * data, __in size_t length)
    {
      std::lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      if (m_hFile)
      {
        // data must not be null
        // length must be greater than zero and less than 4KB, see https://msdn.microsoft.com/en-us/library/windows/hardware/ff540219(v=vs.85).aspx

        WINUSB_SETUP_PACKET setupPacket
        {
          TlsProtocolOOB::ControlTransfer_RequestType_Set, // UCHAR RequestType 
          TlsProtocolOOB::ControlTransfer_Request,    // UCHAR Request
          data[0], // USHORT Value == report id
          TlsProtocolOOB::ControlTransfer_Index,  // USHORT Index
          static_cast<USHORT>(length & 0xffffu) // USHORT Length
        };

        DWORD dwTransferred = 0;
        Win32::win32api_BOOL(m_hWinusb.ControlTransfer(setupPacket, const_cast<std::uint8_t *>(data), length, &dwTransferred, nullptr), "Winusb_ControlTransfer");

        if (dwTransferred != length)
        {
          throw std::runtime_error("Winusb_ControlTransfer - unexpected transmission loss");
        }
      }
      else
      {
        throw not_connected_error();
      }
    }



  } // namespace STU
  
} // namespace WacomGSS

