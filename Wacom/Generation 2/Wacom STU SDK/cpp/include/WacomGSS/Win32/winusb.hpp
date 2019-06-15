/// @file      WacomGSS/Win32/winusb.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper classes to winusb.h for exception safety

#ifndef WacomGSS_Win32_winusb_hpp
#define WacomGSS_Win32_winusb_hpp

#include <WacomGSS/Win32/windows.hpp>
#include <winusb.h>
#include <memory>


namespace WacomGSS
{

  namespace Win32
  {


    class WinusbDll : noncopyable
    {
      HModule m_hModule;

      BOOL (WINAPI * m_Initialize)(HANDLE DeviceHandle, PWINUSB_INTERFACE_HANDLE InterfaceHandle);
      BOOL (WINAPI * m_Free)(WINUSB_INTERFACE_HANDLE InterfaceHandle);
      BOOL (WINAPI * m_ReadPipe)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped);
      BOOL (WINAPI * m_WritePipe)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped);
      BOOL (WINAPI * m_ControlTransfer)(WINUSB_INTERFACE_HANDLE InterfaceHandle, WINUSB_SETUP_PACKET SetupPacket, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped);
      BOOL (WINAPI * m_GetOverlappedResult)(WINUSB_INTERFACE_HANDLE InterfaceHandle, LPOVERLAPPED  lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait );      
      BOOL (WINAPI * m_QueryInterfaceSettings)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR AlternateSettingNumber, PUSB_INTERFACE_DESCRIPTOR pUsbAltInterfaceDescriptor);
      BOOL (WINAPI * m_QueryPipe)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR AlternateInterfaceNumber, UCHAR PipeIndex, PWINUSB_PIPE_INFORMATION pPipeInformation);
      BOOL (WINAPI * m_ResetPipe)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID);      
      BOOL (WINAPI * m_AbortPipe)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID);
      BOOL (WINAPI * m_FlushPipe)(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID);
      
    public:
      WinusbDll();

      bool loadLibrary(wchar_t const * fileName = L"WINUSB.DLL", nothrow_t = nothrow) noexcept;
      bool isLoaded() const noexcept;

      BOOL Initialize(HANDLE DeviceHandle, PWINUSB_INTERFACE_HANDLE InterfaceHandle) const noexcept;
      BOOL Free(WINUSB_INTERFACE_HANDLE InterfaceHandle) const noexcept;
      BOOL ReadPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept;
      BOOL WritePipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept;
      BOOL ControlTransfer(WINUSB_INTERFACE_HANDLE InterfaceHandle, WINUSB_SETUP_PACKET SetupPacket, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept;
      BOOL GetOverlappedResult(WINUSB_INTERFACE_HANDLE InterfaceHandle, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait) const noexcept;
      BOOL QueryInterfaceSettings(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR AlternateSettingNumber, PUSB_INTERFACE_DESCRIPTOR pUsbAltInterfaceDescriptor) const noexcept;
      BOOL QueryPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR AlternateInterfaceNumber, UCHAR PipeIndex, PWINUSB_PIPE_INFORMATION pPipeInformation) const noexcept;
      BOOL ResetPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID) const noexcept;      
      BOOL AbortPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID) const noexcept;
      BOOL FlushPipe(WINUSB_INTERFACE_HANDLE InterfaceHandle, UCHAR PipeID) const noexcept;      
    };



    class Winusb_Interface_Handle : noncopyable
    {
      typedef WINUSB_INTERFACE_HANDLE Type;

      Type                       m_value;
      std::shared_ptr<WinusbDll> m_winusb;

      static Type invalid() noexcept
      { 
        return nullptr; 
      }
      static bool is_valid(Type value) noexcept
      {
        return value != nullptr;
      }
      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }

      bool operator == (Winusb_Interface_Handle const &) const noexcept;
      bool operator != (Winusb_Interface_Handle const &) const noexcept;
  
      void close()
      {
        if (*this)
        {
          auto value = m_value;
          m_value = invalid();
          m_winusb->Free(value);
        }
      }

      struct boolean_value { int value; }; typedef int boolean_value::* boolean_type;

      typedef Winusb_Interface_Handle Handle;
  
      class pointer : noncopyable
      {
        Type     m_value;
        Handle & m_handle;
  
      public:
#if !defined(_MSC_VER) || (_MSC_VER>=1900)
        pointer(pointer &&) noexcept = default;
#endif
        pointer(Handle & handle) noexcept
        :
          m_value(handle.get()),
          m_handle(handle)
        {
        }
  
        ~pointer()
        {
          m_handle.reset(m_value);
        }
  
        operator Type * () noexcept
        {
          return &m_value;
        }
      };
      
      class pointer_nothrow : noncopyable
      {
        Type     m_value;
        Handle & m_handle;
  
      public:
#if !defined(_MSC_VER) || (_MSC_VER>=1900)
        pointer_nothrow(pointer_nothrow &&) noexcept = default;
#endif
        pointer_nothrow(Handle & handle) noexcept
        :
          m_value(handle.get()),
          m_handle(handle)
        {
        }
  
        ~pointer_nothrow() noexcept
        {
          m_handle.reset(m_value, nothrow);
        }
  
        operator Type * () noexcept
        {
          return &m_value;
        }
      };

      Type get() const noexcept
      {
        return m_value;
      }


    public:
      Winusb_Interface_Handle(std::shared_ptr<WinusbDll> & winusb, Type value)
      :
        m_value(value),
        m_winusb(winusb)
      {
        if (!*this) 
        {
          throw_invalid_handle_exception();
        }
      }
      
      Winusb_Interface_Handle(std::shared_ptr<WinusbDll> & winusb, Type value, nothrow_t) noexcept
      :
        m_value(value),
        m_winusb(winusb)
      {
      }

      Winusb_Interface_Handle(std::shared_ptr<WinusbDll> & winusb) noexcept
      :
        m_value(invalid()),
        m_winusb(winusb)
      {
      }

      Winusb_Interface_Handle(Winusb_Interface_Handle && other) noexcept
      :
        m_value(other.release()),
        m_winusb(other.m_winusb)
      {
      }

      ~Winusb_Interface_Handle()
      {
        close();
      }

      Winusb_Interface_Handle & operator = (Winusb_Interface_Handle && other) noexcept
      {
        reset(other.release(), nothrow);
        return *this;
      }

      operator boolean_type () const noexcept
      {
        return is_valid(m_value) ? &boolean_value::value : nullptr;
      }
  
      void reset() noexcept
      {
        if (m_value != invalid()) 
        {
          close();
        }
      }
  
      void reset(Type value, nothrow_t) noexcept
      {
        if (m_value != value) 
        {
          close();
          m_value = value;
        }
      }
  
      void reset(Type value)
      {
        if (m_value != value) 
        {
          close();
          m_value = value;
        }
        if (!*this) 
        {
          throw_invalid_handle_exception();
        }        
      }
      
      Type release() noexcept
      {
        auto value = m_value;
        m_value = invalid();
        return value;
      }

      pointer initialize() noexcept
      {
        return pointer(*this);
      }
      
      pointer_nothrow initialize(nothrow_t) noexcept
      {
        return pointer_nothrow(*this);
      }


      BOOL Free() const noexcept
      {
        return m_winusb->Free(get());
      }
      
      BOOL ReadPipe(UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept
      {
        return m_winusb->ReadPipe(get(), PipeID, Buffer, BufferLength, LengthTransferred, Overlapped);
      }

      BOOL WritePipe(UCHAR PipeID, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept
      {
        return m_winusb->WritePipe(get(), PipeID, Buffer, BufferLength, LengthTransferred, Overlapped);
      }

      BOOL ControlTransfer(WINUSB_SETUP_PACKET SetupPacket, PUCHAR Buffer, ULONG BufferLength, PULONG LengthTransferred, LPOVERLAPPED Overlapped) const noexcept
      {
        return m_winusb->ControlTransfer(get(), SetupPacket, Buffer, BufferLength, LengthTransferred, Overlapped);
      }
      
      BOOL GetOverlappedResult(LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait) const noexcept
      {
        return m_winusb->GetOverlappedResult(get(), lpOverlapped, lpNumberOfBytesTransferred, bWait);
      }

      BOOL QueryInterfaceSettings(UCHAR AlternateSettingNumber, PUSB_INTERFACE_DESCRIPTOR pUsbAltInterfaceDescriptor) const noexcept
      {
        return m_winusb->QueryInterfaceSettings(get(), AlternateSettingNumber, pUsbAltInterfaceDescriptor);
      }

      BOOL QueryPipe(UCHAR AlternateInterfaceNumber, UCHAR PipeIndex, PWINUSB_PIPE_INFORMATION pPipeInformation) const noexcept
      {
        return m_winusb->QueryPipe(get(), AlternateInterfaceNumber, PipeIndex, pPipeInformation);
      }

      BOOL ResetPipe(UCHAR PipeID) const noexcept
      {
        return m_winusb->ResetPipe(get(), PipeID);
      }

      BOOL AbortPipe(UCHAR PipeID) const noexcept
      {
        return m_winusb->AbortPipe(get(), PipeID);
      }

      BOOL FlushPipe(UCHAR PipeID) const noexcept
      {
        return m_winusb->FlushPipe(get(), PipeID);
      }


    };


  } // namespace Win32
  
} // namespace WacomGSS



#endif // WacomGSS_Win32_winusb_hpp
