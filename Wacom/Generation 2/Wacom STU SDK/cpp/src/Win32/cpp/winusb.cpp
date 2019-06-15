#include <WacomGSS/Win32/winusb.hpp>

namespace WacomGSS
{
  namespace Win32
  {

    WinusbDll::WinusbDll()
    :
      m_hModule(),
      m_Initialize(nullptr),
      m_Free(nullptr),
      m_ReadPipe(nullptr),
      m_WritePipe(nullptr),
      m_ControlTransfer(nullptr),
      m_GetOverlappedResult(nullptr),
      m_QueryInterfaceSettings(nullptr),
      m_QueryPipe(nullptr),
      m_ResetPipe(nullptr),
      m_AbortPipe(nullptr),
      m_FlushPipe(nullptr)
    {
    }

    bool WinusbDll::loadLibrary(wchar_t const * fileName, nothrow_t) noexcept
    {
      if (!isLoaded()) 
      {
        Win32::HModule hModule(::LoadLibrary(fileName), nothrow);
        if (hModule) 
        {
#pragma warning(disable:4191)
          m_Initialize          = getProcAddress<BOOL (WINAPI *)(HANDLE, PWINUSB_INTERFACE_HANDLE)                                   >(hModule.get(), "WinUsb_Initialize", nothrow);
          m_Free                = getProcAddress<BOOL (WINAPI *)(WINUSB_INTERFACE_HANDLE)                                            >(hModule.get(), "WinUsb_Free", nothrow);
          m_ReadPipe            = getProcAddress<BOOL (WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR, PUCHAR, ULONG, PULONG, LPOVERLAPPED)>(hModule.get(), "WinUsb_ReadPipe", nothrow);
          m_WritePipe           = getProcAddress<BOOL (WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR, PUCHAR, ULONG, PULONG, LPOVERLAPPED)>(hModule.get(), "WinUsb_WritePipe", nothrow);
          m_ControlTransfer     = getProcAddress<BOOL (WINAPI *)(WINUSB_INTERFACE_HANDLE, WINUSB_SETUP_PACKET, PUCHAR, ULONG, PULONG, LPOVERLAPPED)>(hModule.get(), "WinUsb_ControlTransfer", nothrow);
          m_GetOverlappedResult = getProcAddress<BOOL (WINAPI *)(WINUSB_INTERFACE_HANDLE, LPOVERLAPPED, LPDWORD, BOOL)               >(hModule.get(), "WinUsb_GetOverlappedResult", nothrow);
          m_QueryInterfaceSettings = getProcAddress<BOOL(WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR, PUSB_INTERFACE_DESCRIPTOR)        >(hModule.get(), "WinUsb_QueryInterfaceSettings", nothrow);
          m_QueryPipe              = getProcAddress<BOOL(WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR, UCHAR, PWINUSB_PIPE_INFORMATION)  >(hModule.get(), "WinUsb_QueryPipe", nothrow);
          m_ResetPipe              = getProcAddress<BOOL (WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR)                                  >(hModule.get(), "WinUsb_ResetPipe", nothrow);
          m_AbortPipe              = getProcAddress<BOOL(WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR)                                   >(hModule.get(), "WinUsb_AbortPipe", nothrow);
          m_FlushPipe              = getProcAddress<BOOL(WINAPI *)(WINUSB_INTERFACE_HANDLE, UCHAR)                                   >(hModule.get(), "WinUsb_FlushPipe", nothrow);          
          
#pragma warning(default:4191)

          if (m_Initialize && m_Free && m_ReadPipe && m_WritePipe && m_ControlTransfer && m_GetOverlappedResult && m_QueryInterfaceSettings && m_QueryPipe && m_ResetPipe && m_AbortPipe && m_FlushPipe)
          {
            m_hModule = std::move(hModule);
          }
          else 
          {
            m_Initialize          = nullptr;
            m_Free                = nullptr;
            m_ReadPipe            = nullptr;
            m_WritePipe           = nullptr;
            m_ControlTransfer     = nullptr;
            m_GetOverlappedResult = nullptr;            
            m_QueryInterfaceSettings = nullptr;
            m_QueryPipe = nullptr;
            m_ResetPipe = nullptr;
            m_AbortPipe = nullptr;
            m_FlushPipe = nullptr;
          }
        }
      }
      return m_hModule;
    }

    /*void WinusbDll::freeLibrary() noexcept
    {
      if (m_hModule) 
      {
        m_hModule.reset();
        m_Initialize          = 0;
        m_Free                = 0;
        m_ReadPipe           = 0;
        m_WritePipe           = 0;
        m_GetOverlappedResult = 0;
        m_QueryInterfaceSettings = 0;
        m_QueryPipe = 0;
        m_ResetPipe           = 0;
        m_AbortPipe = 0;
        m_FlushPipe = 0;
      }
    }*/

    bool WinusbDll::isLoaded() const noexcept
    {
      return m_hModule;
    }

    BOOL WinusbDll::Initialize(HANDLE DeviceHandle, PWINUSB_INTERFACE_HANDLE InterfaceHandle) const noexcept
    {
      return m_Initialize(DeviceHandle, InterfaceHandle);
    }

    BOOL WinusbDll::Free(WINUSB_INTERFACE_HANDLE InterfaceHandle) const noexcept
    {
      return m_Free(InterfaceHandle);
    }
    
    BOOL WinusbDll::ReadPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept
    {
      return m_ReadPipe(InterfaceHandle, PipeID, Buffer, BufferLength, LengthTransferred, Overlapped);
    }
    
    BOOL WinusbDll::WritePipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept
    {
      return m_WritePipe(InterfaceHandle, PipeID, Buffer, BufferLength, LengthTransferred, Overlapped);
    }

    BOOL WinusbDll::ControlTransfer(WINUSB_INTERFACE_HANDLE InterfaceHandle, WINUSB_SETUP_PACKET SetupPacket, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept
    {
      return m_ControlTransfer(InterfaceHandle, SetupPacket, Buffer, BufferLength, LengthTransferred, Overlapped);
    }

    BOOL WinusbDll::GetOverlappedResult(WINUSB_INTERFACE_HANDLE InterfaceHandle, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait) const noexcept
    {
      return m_GetOverlappedResult(InterfaceHandle, lpOverlapped, lpNumberOfBytesTransferred, bWait);
    }
        
    BOOL WinusbDll::QueryInterfaceSettings(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR AlternateSettingNumber, PUSB_INTERFACE_DESCRIPTOR pUsbAltInterfaceDescriptor) const noexcept
    {
      return m_QueryInterfaceSettings(InterfaceHandle, AlternateSettingNumber, pUsbAltInterfaceDescriptor);
    }

    BOOL WinusbDll::QueryPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR AlternateInterfaceNumber, UCHAR PipeIndex, PWINUSB_PIPE_INFORMATION pPipeInformation) const noexcept
    {
      return m_QueryPipe(InterfaceHandle, AlternateInterfaceNumber, PipeIndex, pPipeInformation);
    }

    BOOL WinusbDll::ResetPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID) const noexcept
    {
      return m_ResetPipe(InterfaceHandle, PipeID);
    }

    BOOL WinusbDll::AbortPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID) const noexcept
    {
      return m_AbortPipe(InterfaceHandle, PipeID);
    }

    BOOL WinusbDll::FlushPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID) const noexcept
    {
      return m_FlushPipe(InterfaceHandle, PipeID);
    }


  } // namespace Win32

  
} // namespace WacomGSS
