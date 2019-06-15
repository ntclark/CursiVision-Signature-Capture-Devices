
#include <WacomGSS/STU/Win32/UsbInterface.hpp>
#include <WacomGSS/Win32/hidsdi.hpp>
#include <WacomGSS/Win32/getDevicePowerState.hpp>
#include <WacomGSS/setThreadName.hpp>
#include <array>

namespace WacomGSS
{
  namespace STU
  {
    
    
    // HID:
    // If the read buffer is not initialised with a reportId, then 
    // Microsoft Windows will always return the largest report data regardless of the
    // report type. Therefore we have to skip potentially random data in order
    // to re-synchronise with the next packet.
    void UsbInterface::processReadBuffer(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
    {
      uint16_t inputLength = 0;
      
      while (begin != end && (inputLength = m_inputLengths[*begin]) != 0 && begin+inputLength <= end)
      {
        queueReport(std::vector<uint8_t>(begin, begin+inputLength));
        begin += m_maxInputLength;
      }
    }

    
    
    // When existing from the reader thread, it is vital to ensure that overlapped IO 
    // has completed before releasing data buffers.
    // also see SerialInterface.cpp
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



    void UsbInterface::readerThread() noexcept
    {
      setThreadName("UsbInterface::readerThread");
         
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
        std::unique_ptr<OVERLAPPED> o(new OVERLAPPED);
        Win32::Handle     readEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr));

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

          ZeroMemory(readBuffer.data(), readBuffer.size());

          ZeroMemory(o.get(), sizeof(OVERLAPPED));
          o->hEvent = readEvent.get();

          BOOL bSuccess = ( 0 == ::ReadFile(m_hFile.get(), readBuffer.data(), (DWORD)readBuffer.size(), nullptr, o.get()) );
          if ( ! bSuccess )
          {
            DWORD dwError = ::GetLastError();
            if (dwError == ERROR_IO_PENDING) 
            {
              std::array<HANDLE,2> h = {{ m_quitEvent.get(), readEvent.get() }};
              dwWait = ::WaitForMultipleObjectsEx((DWORD)h.size(), h.data(), FALSE, INFINITE, FALSE);
              if (dwWait == WAIT_OBJECT_0 +0) 
              {
                // quit signaled
                cancelIo(m_hFile, o, readBuffer);
                break;
              }
              else if (dwWait != WAIT_OBJECT_0+1)
              {
                // something other than event signaled
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
            processReadBuffer(readBuffer.begin(), readBuffer.begin() + static_cast<ptrdiff_t>(dwBytes));
          }
          else 
          {
            DWORD dwError = GetLastError();
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
    
      }
      catch (...)
      {
        queueException(std::current_exception());
      }    
    }



    void UsbInterface::startReaderThread()
    {
      stopReaderThread();

      m_readerThread = std::move(thread(std::bind(std::mem_fn(&UsbInterface::readerThread), this)));
    }


    
   void UsbInterface::stopReaderThread() {
      
   Win32::win32api_BOOL(::SetEvent(m_quitEvent.get()));
      
   if ( m_readerThread.joinable() )
      m_readerThread.join();

   Win32::win32api_BOOL(::ResetEvent(m_quitEvent.get()));
    
   queueClear();
   }
    


    UsbInterface::UsbInterface()
    :
      m_WinusbDll(new Win32::WinusbDll),
      m_hWinusb(m_WinusbDll),
      m_bulkOutPipeId(0),
      m_quitEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr)),
      m_maxInputLength(0),
      m_idProduct(0)
    {
      std::fill(m_inputLengths.begin(), m_inputLengths.end(), (uint16_t)0x00u);      
    }



    UsbInterface::UsbInterface(std::shared_ptr<Win32::WinusbDll> & winusb)
    :
      m_WinusbDll(winusb),
      m_hWinusb(m_WinusbDll),
      m_quitEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr)),
      m_maxInputLength(0),
      m_idProduct(0)
    {
      std::fill(m_inputLengths.begin(), m_inputLengths.end(), (uint16_t)0);
    }
    


    void UsbInterface::cacheData()
    {
      m_maxInputLength = 0;
      std::fill(m_inputLengths.begin(), m_inputLengths.end(), (uint16_t)0);
      m_idProduct = 0;

      try
      {
        HIDD_ATTRIBUTES attributes = { sizeof(HIDD_ATTRIBUTES) };
        Win32::win32api_BOOLEAN(HidD_GetAttributes(m_hFile.get(), &attributes), "HidD_GetAttributes");
        if (attributes.VendorID == VendorId_Wacom)
        {
          m_idProduct = attributes.ProductID;
        }

        Win32::HidP_Preparsed_Data pPreparsedData;
        Win32::win32api_BOOLEAN( HidD_GetPreparsedData(m_hFile.get(), pPreparsedData.initialize(nothrow)));        

        HIDP_CAPS Caps = {0};
        Win32::win32api_NTSTATUS_equal(HIDP_STATUS_SUCCESS, HidP_GetCaps(pPreparsedData.get(), &Caps));
        
        if (Caps.NumberInputValueCaps)
        {
          std::vector<HIDP_VALUE_CAPS> pValueCaps(Caps.NumberInputValueCaps);
          Win32::win32api_NTSTATUS_equal(HIDP_STATUS_SUCCESS, HidP_GetValueCaps(HidP_Input, pValueCaps.data(), &Caps.NumberInputValueCaps, pPreparsedData.get()) );

          for (USHORT i = 0; i < Caps.NumberInputValueCaps; ++i)
          {
            // 1 is added to include the ReportId as well.
            m_inputLengths[ pValueCaps[i].ReportID ] = pValueCaps[i].ReportCount + 1u;
          }

          m_maxInputLength = *std::max_element(m_inputLengths.begin(), m_inputLengths.end());
        }
      }
      catch (...)
      {
        m_maxInputLength = 0;
        std::fill(m_inputLengths.begin(), m_inputLengths.end(), (uint16_t)0);
        m_idProduct = 0;
        throw;
      }
    }



    UsbInterface::~UsbInterface() noexcept
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
    


    std::error_code UsbInterface::connect(UsbDevice const & usbDevice, bool exclusiveLock)
    {
      auto devicePowerState = Win32::getDevicePowerState(usbDevice.devInst);
      bool win81sleep = (devicePowerState != PowerDeviceD0);
      return connect(win81sleep, usbDevice.fileName.c_str(), usbDevice.bulkFileName.empty() ? nullptr : usbDevice.bulkFileName.c_str(), exclusiveLock);
    }


          
    std::error_code UsbInterface::connect(std::wstring const & fileName, std::wstring const & bulkFileName, bool exclusiveLock)
    {
      return connect(fileName.c_str(), bulkFileName.c_str(), exclusiveLock);
    }



    static bool is_win81_or_higher()
    {      
      // Initialize the OSVERSIONINFOEX structure.

      OSVERSIONINFOEX osvi = { sizeof(OSVERSIONINFOEX) };
      osvi.dwMajorVersion = 6;
      osvi.dwMinorVersion = 2;
      
      // Initialize the condition mask.
      
      DWORDLONG conditionMask = 0;
      VER_SET_CONDITION( conditionMask, VER_MAJORVERSION,     VER_GREATER_EQUAL );
      VER_SET_CONDITION( conditionMask, VER_MINORVERSION,     VER_GREATER_EQUAL );
      VER_SET_CONDITION( conditionMask, VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL );
      VER_SET_CONDITION( conditionMask, VER_SERVICEPACKMINOR, VER_GREATER_EQUAL );

      // Perform the test.

      BOOL b = VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR | VER_SERVICEPACKMINOR, conditionMask);
      return b != FALSE;
    }



    std::error_code UsbInterface::connect(wchar_t const * fileName, wchar_t const * bulkFileName, bool exclusiveLock)
    {
      // A DEVINST handle is required to query the device power. If this has not been supplied we have to
      // guess whether to enforce a delay or not. D3 power state only happens on Windows 8.1 when no driver is
      // bound to the device. This discounts the 430/530 that auto-bind. The 520 if the bulk driver is load is
      // also exempt.
      bool win81sleep = (!bulkFileName || !bulkFileName[0]) && is_win81_or_higher();

      return connect(win81sleep, fileName, bulkFileName, exclusiveLock);
    }


    static bool getBulkOutPipeId(Win32::Winusb_Interface_Handle & hWinusb, UCHAR & bulkOutPipeId)
    {
      bool success = false;

      bulkOutPipeId = 0;
      //USHORT bulkOutMaxPacketSize = 0;

      USB_INTERFACE_DESCRIPTOR usbInterfaceDescriptor { 0 };
      if (hWinusb.QueryInterfaceSettings(0, &usbInterfaceDescriptor))
      {
        for (unsigned i = 0; i < usbInterfaceDescriptor.bNumEndpoints; ++i)
        {
          WINUSB_PIPE_INFORMATION pipeInformation { (USBD_PIPE_TYPE)0 };
          if (!hWinusb.QueryPipe(0, static_cast<UCHAR>(i & 0xFF), &pipeInformation))
          {
            break;
          }

          if (pipeInformation.PipeType == UsbdPipeTypeBulk)
          {
            if (USB_ENDPOINT_DIRECTION_OUT(pipeInformation.PipeId))
            {
              bulkOutPipeId = pipeInformation.PipeId;
              //bulkOutMaxPacketSize = pipeInformation.MaximumPacketSize;
              success = true;
            }
            break;
          }
        } //for
      }

      return success;
    }


    std::error_code UsbInterface::connect(bool win81sleep, wchar_t const * fileName, wchar_t const * bulkFileName, bool exclusiveLock)
    {
      disconnect();

      lock_guard<mutex> lock(m_apiMutex);
    
      std::error_code ec;
      try

      {
        DWORD dwShareMode = exclusiveLock ? 0U : (FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE); 
    
        m_hFile.reset(::CreateFileW(fileName, GENERIC_READ|GENERIC_WRITE, dwShareMode, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0));
        // successfully opened device!

        cacheData();
        
        if (bulkFileName && *bulkFileName) 
        {
          m_hBulk.reset(::CreateFileW(bulkFileName, GENERIC_READ|GENERIC_WRITE, dwShareMode, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0), nothrow);
          if (m_hBulk) 
          {
            if (m_WinusbDll->isLoaded() || m_WinusbDll->loadLibrary()) 
            {
              if (m_WinusbDll->Initialize(m_hBulk.get(), m_hWinusb.initialize(nothrow))) 
              {
                if (getBulkOutPipeId(m_hWinusb, m_bulkOutPipeId))
                {
                  // successfully opened bulk!
                }
                else
                {
                  m_hBulk.reset();
                }
              }
              else 
              {
                m_hBulk.reset();
              }
            }
            else 
            {
              m_hBulk.reset();
            }
          }
        }          

        if (win81sleep)
        {
          // We have not had the minimum amount of delay confirmed by the firmware team to ensure
          // the tablets will be completely transitioned to D0 power. Ad hoc testing suggests that
          // at least 750ms will be required for the STU-300
          ::Sleep(1000);
        }

        startReaderThread();        
      }
      catch (std::system_error & ex)
      {
        ec = ex.code();

        m_hFile.reset();
      }
      catch (...)
      {
        m_hFile.reset();
        throw;
      }
      
      return ec;
    }



    void UsbInterface::disconnect()
    {
      lock_guard<mutex> lock(m_apiMutex);
    
      stopReaderThread();
          
      m_hWinusb.reset();
      m_hBulk.reset();
      m_hFile.reset();
      std::fill(m_inputLengths.begin(), m_inputLengths.end(), (uint16_t)0);
      m_maxInputLength = 0;
      m_idProduct = 0;
      m_bulkOutPipeId = 0;
    }



    bool UsbInterface::isConnected() const
    {
      return m_hFile;
    }



    void UsbInterface::get(uint8_t * /*out*/ data, size_t length)
    {

      lock_guard<mutex> lock(m_apiMutex);
      if (m_hFile) 
      {

        BOOLEAN b = HidD_GetFeature(m_hFile.get(), data, (ULONG)length);
        if (!b) 
        { 
          DWORD dwError = GetLastError();
          if (dwError == ERROR_DEVICE_NOT_CONNECTED) 
          {
            throw device_removed_error();
          }
          else 
          {
            Win32::throw_win32api_error(dwError, "HidD_GetFeature");
          }
        }
      }
      else 
      {
        throw not_connected_error();
      }
    }



    void UsbInterface::set(uint8_t const * data, size_t length)
    {
      lock_guard<mutex> lock(m_apiMutex);
      if (m_hFile) 
      {
        BOOLEAN b = HidD_SetFeature(m_hFile.get(), const_cast<uint8_t *>(data), (ULONG)length);
        if (!b) 
        { 
          DWORD dwError = GetLastError();
          if (dwError == ERROR_DEVICE_NOT_CONNECTED) 
          {
            throw device_removed_error();
          }
          else 
          {
            Win32::throw_win32api_error(dwError, "HidD_SetFeature");
          }
        }
      }
      else 
      {
        throw not_connected_error();
      }
    }



    bool UsbInterface::supportsWrite() const 
    {
      return m_hWinusb;
    }
  
    
    
    void UsbInterface::write(uint8_t const * data, size_t length) 
    {
      if (data && length) 
      {
        lock_guard<mutex> lock(m_apiMutex);

        if (m_hFile) 
        { 
          if (m_hWinusb) 
          {
            Win32::Handle hEvent(::CreateEventW(nullptr, TRUE, FALSE, nullptr));
        
            OVERLAPPED o = {0};
            o.hEvent = hEvent.get();
            
            BOOL  bSuccess = m_hWinusb.WritePipe(m_bulkOutPipeId, const_cast<PUCHAR>(data), (ULONG)length, nullptr, &o);
            if (!bSuccess)
            {
              DWORD dwError = ::GetLastError();
              if (dwError == ERROR_IO_PENDING) 
              {                
                DWORD dwWait = ::WaitForSingleObjectEx(o.hEvent, INFINITE, FALSE);
                if (dwWait != WAIT_OBJECT_0)
                {
                  // something other than event signaled
                  dwError = ::GetLastError();

                  Win32::throw_win32api_error(dwError);
                }
              }
              else if (dwError == ERROR_DEVICE_NOT_CONNECTED) 
              {
                throw Interface::device_removed_error();
              }
              else 
              {
                /*bSuccess =*/ m_hWinusb.ResetPipe(1);

                Win32::throw_win32api_error(dwError, "WinUsb_WritePipe");
              }
            }        

            DWORD dwBytesWritten = 0;
            bSuccess = m_hWinusb.GetOverlappedResult(&o, &dwBytesWritten, TRUE);            
            if (bSuccess)
            {
              if (dwBytesWritten != length)
                throw io_error();
            }
            else
            {              
              DWORD dwError = GetLastError();
              if (dwError == ERROR_DEVICE_NOT_CONNECTED) 
              {
                throw device_removed_error();
              }
              else 
              {
                /*bSuccess = */ m_hWinusb.ResetPipe(1);
                Win32::throw_win32api_error(dwError, "GetOverlappedResult");
              }
            }
          }        
          else
          {
            throw write_not_supported_error();
          }
        }
        else 
        {
          throw not_connected_error();
        }
      }
      //else 
      //{
      //  // no data to write, do nothing
      //}                
    }



    bool UsbInterface::getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const
    {
      std::fill(reportCountLengths.begin(), reportCountLengths.end(), (uint16_t)0);

      lock_guard<mutex> lock(m_apiMutex);

      if (m_hFile) 
      { 
        Win32::HidP_Preparsed_Data pPreparsedData;
        Win32::win32api_BOOLEAN( HidD_GetPreparsedData(m_hFile.get(), pPreparsedData.initialize(nothrow)) );
        
        HIDP_CAPS Caps = {0};
        Win32::win32api_NTSTATUS_equal(HIDP_STATUS_SUCCESS, HidP_GetCaps(pPreparsedData.get(), &Caps) );
        
        if (Caps.NumberFeatureValueCaps)
        {
          std::vector<HIDP_VALUE_CAPS> pValueCaps(Caps.NumberFeatureValueCaps);
          Win32::win32api_NTSTATUS_equal(HIDP_STATUS_SUCCESS, HidP_GetValueCaps(HidP_Feature, pValueCaps.data(), &Caps.NumberFeatureValueCaps, pPreparsedData.get()) );

          for (USHORT i = 0; i < Caps.NumberFeatureValueCaps; ++i)
          {
            reportCountLengths[ pValueCaps[i].ReportID ] = pValueCaps[i].ReportCount + 1u;
          }
        }
      }
      else 
      {
        throw not_connected_error();
      }

      return true;
    }



    std::uint16_t UsbInterface::getProductId() const
    {
      lock_guard<mutex> lock(m_apiMutex);

      if (m_hFile) 
      {
        return m_idProduct;
      }
      else 
      {
        throw not_connected_error();
      }
    }


  } // namespace STU
  
} // namespace WacomGSS

