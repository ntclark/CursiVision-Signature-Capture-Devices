/// @file      WacomGSS/Win32/windows.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper classes to windows.h for exception safety

#ifndef WacomGSS_Win32_windows_hpp
#define WacomGSS_Win32_windows_hpp

#include <WacomGSS/unique_handle.hpp>

#pragma warning(push, 3)
#pragma warning(disable: 4548 5026 5027)
#include <windows.h>
#include <system_error>
#pragma warning(pop)



//#include <process.h>
//#include <memory>

//#define WACOM_GSDT_API      WINAPI
//#define WACOM_GSDT_CALLBACK WINAPI

namespace WacomGSS
{
  namespace Win32
  {

    std::error_category const & win32api_error_category() noexcept;

    inline std::error_code make_win32api_error_code(DWORD dwError)
    {
      return std::error_code(static_cast<int>(dwError), win32api_error_category());
    }

    WacomGSS_noreturn inline void throw_win32api_error(DWORD dwError)
    {
      throw std::system_error(make_win32api_error_code(dwError));
    }

    template<typename Ty>
    WacomGSS_noreturn inline void throw_win32api_error(DWORD dwError, Ty _Message)
    {
      throw std::system_error(make_win32api_error_code(dwError), _Message);
    }


    //=========================================================================


    inline void win32api_bool(bool value)
    {
      if (!value) 
      {
        throw_win32api_error(::GetLastError());
      }
    }


    template<typename _Ty>
    inline void win32api_bool(bool value, _Ty _Message)
    {
      if (!value) 
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    }
    
    
    inline void win32api_BOOL(BOOL value)
    {
      if (!value) 
      {
        throw_win32api_error(::GetLastError());
      }
    }



    template<typename Ty>
    inline void win32api_BOOL(BOOL value, Ty _Message)
    {
      if (!value) 
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    }



    inline void win32api_BOOLEAN(BOOLEAN value)
    {
      if (!value) 
      {
        throw_win32api_error(::GetLastError());
      }
    }


    template<typename _Ty>
    inline void win32api_BOOLEAN(BOOLEAN value, _Ty _Message)
    {
      if (!value) 
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    }



    inline void win32api_DWORD(DWORD value)
    {
      if (value != ERROR_SUCCESS) 
      {
        throw_win32api_error(value);
      }
    }



    inline void win32api_LONG(LONG value)
    {
      if (value != ERROR_SUCCESS) 
      {
        throw_win32api_error(*reinterpret_cast<DWORD const *>(&value));
      }
    }

    template<typename Ty>
    inline void win32api_LONG(LONG value, Ty _Message)
    {
      if (value != ERROR_SUCCESS) 
      {
        throw_win32api_error(*reinterpret_cast<DWORD const *>(&value), _Message);
      }
    }


  } // namespace Win32

} // namespace WacomGSS


//=============================================================================


namespace WacomGSS
{
  namespace Win32
  {


    template<typename T>
    T getProcAddress(HMODULE hModule, LPCSTR lpProcName)
    {
      FARPROC fp = ::GetProcAddress(hModule, lpProcName);
      if (!fp)
      {
        throw_win32api_error(::GetLastError(), "GetProcAddress");
      }
#pragma warning(suppress:4191)
      return reinterpret_cast<T>(fp);
    }



    template<typename T>
    T getProcAddress(HMODULE hModule, LPCSTR lpProcName, std::nothrow_t) noexcept
    {
      FARPROC fp = ::GetProcAddress(hModule, lpProcName);
#pragma warning(suppress:4191)
      return reinterpret_cast<T>(fp);
    }



  } // namespace Win32
} // namespace WacomGSS



//=============================================================================



namespace WacomGSS
{
  namespace Win32
  {

    struct HANDLE_CloseHandle_traits
    {
      static HANDLE invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HANDLE value) noexcept
      {
        return value != nullptr && value != INVALID_HANDLE_VALUE;
      }

      static void close(HANDLE value) noexcept
      {
        //BOOL
        ::CloseHandle(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }

      template<typename Ty>
      static void throw_invalid_handle_exception(Ty _Message)
      {
        throw_win32api_error(::GetLastError(), _Message);
      }

    };
    typedef unique_handle<HANDLE,HANDLE_CloseHandle_traits> Handle;


    //=========================================================================


    struct HANDLE_FindClose_traits
    {
      static HANDLE invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HANDLE value) noexcept
      {
        return value != nullptr && value != INVALID_HANDLE_VALUE;
      }

      static void close(HANDLE value) noexcept
      {
        //BOOL
        ::FindClose(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<HANDLE,HANDLE_FindClose_traits> FindHandle;


    //=========================================================================


    struct HMODULE_FreeLibrary_traits
    {
      static HMODULE invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HMODULE value) noexcept
      {
        return value != nullptr;
      }

      static void close(HMODULE value) noexcept
      {
        //BOOL
        ::FreeLibrary(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
      
      template<typename Ty>
      static void throw_invalid_handle_exception(Ty _Message)
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    };
    typedef unique_handle<HMODULE,HMODULE_FreeLibrary_traits> HModule;

    //=========================================================================

    struct LPVOID_UnmapViewOfFile_traits
    {
      static LPVOID invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(LPVOID value) noexcept
      {
        return value != nullptr;
      }

      static void close(LPVOID value) noexcept
      {
        //BOOL
        ::UnmapViewOfFile(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }

      template<typename Ty>
      static void throw_invalid_handle_exception(Ty _Message)
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    };
    typedef unique_handle<LPVOID, LPVOID_UnmapViewOfFile_traits> PViewOfFile;

    //==============================================================================


#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)

    struct HGLOBAL_GlobalFree_traits
    {
      static HGLOBAL invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HGLOBAL value) noexcept
      {
        return value != nullptr;
      }

      static void close(HGLOBAL value) noexcept
      {
        //HGLOBAL
        ::GlobalFree(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<HGLOBAL,HGLOBAL_GlobalFree_traits> HGlobal;


    //=========================================================================


    struct HDEVNOTIFY_UnregisterDeviceNotification_traits
    {
      static HDEVNOTIFY invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HDEVNOTIFY value) noexcept
      {
        return value != nullptr;
      }

      static void close(HDEVNOTIFY value) noexcept
      {
        //BOOL
        ::UnregisterDeviceNotification(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<HDEVNOTIFY,HDEVNOTIFY_UnregisterDeviceNotification_traits> HDevNotify;


    //=========================================================================


    struct SC_HANDLE_CloseServiceHandle_traits
    {
      static SC_HANDLE invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(SC_HANDLE value) noexcept
      {
        return value != nullptr;
      }

      static void close(SC_HANDLE value) noexcept
      {
        //BOOL
        ::CloseServiceHandle(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }
    };
    typedef unique_handle<SC_HANDLE,SC_HANDLE_CloseServiceHandle_traits> SC_Handle;


    //=========================================================================


    struct HKEY_RegCloseKey_traits
    {
      static HKEY invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HKEY value) noexcept
      {
        return value != nullptr && value != INVALID_HANDLE_VALUE;
      }

      static void close(HKEY value) noexcept
      {
        //LONG
        ::RegCloseKey(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }

      template<typename Ty>
      static void throw_invalid_handle_exception(Ty _Message)
      {
        throw_win32api_error(::GetLastError(), _Message);
      }
    };
    typedef unique_handle<HKEY,HKEY_RegCloseKey_traits> HKey;

    //=========================================================================


    class HGlobalLock : noncopyable
    {
      HGLOBAL m_hGlobal;
      PVOID   m_pvoid;
    public:
      HGlobalLock(HGLOBAL hGlobal)
        :
        m_hGlobal(hGlobal),
        m_pvoid(nullptr)
      {
        if (m_hGlobal)
        {
          m_pvoid = ::GlobalLock(m_hGlobal);
          win32api_bool(m_pvoid != nullptr, "GlobalLock");
        }
      }

      ~HGlobalLock() noexcept
      {
        if (m_hGlobal && m_pvoid)
        {
          // BOOL
          ::GlobalUnlock(m_hGlobal);
        }
      }

      void reset()
      {
        auto hGlobal = m_hGlobal;
        m_hGlobal = nullptr;
        if (m_pvoid)
        {
          m_pvoid = nullptr;
          win32api_BOOL(::GlobalUnlock(hGlobal), "GlobalUnlock");
        }
      }

      template<typename T>
      T * get() const noexcept
      {
        return reinterpret_cast<T *>(m_pvoid);
      }

    };

    struct HANDLE_DeregisterEventSource_traits
    {
      static HANDLE invalid() noexcept
      {
        return nullptr;
      }

      static bool is_valid(HANDLE value) noexcept
      {
        return value != nullptr;
      }

      static void close(HANDLE value) noexcept
      {
        //BOOL
        ::DeregisterEventSource(value);
      }

      static void throw_invalid_handle_exception()
      {
        throw_win32api_error(::GetLastError());
      }

      template<typename Ty>
      static void throw_invalid_handle_exception(Ty _Message)
      {
        throw_win32api_error(::GetLastError(), _Message);
      }

    };
    typedef unique_handle<HANDLE, HANDLE_DeregisterEventSource_traits> HEventSource;


    //==============================================================================


    class RegisterClassAtom : noncopyable
    {
      ATOM      m_atom;
      HINSTANCE m_hinstance;

    public:
      RegisterClassAtom(WNDCLASSEX const & wndClass)
        :
        m_atom(::RegisterClassEx(&wndClass)),
        m_hinstance(wndClass.hInstance)
      {
        win32api_bool(m_atom != 0, "RegisterClassEx");
      }


      RegisterClassAtom(WNDCLASSEX const & wndClass, nothrow_t) noexcept
        :
        m_atom(::RegisterClassEx(&wndClass)),
        m_hinstance(wndClass.hInstance)
      {
      }


      RegisterClassAtom(RegisterClassAtom && other) noexcept
        :
        m_atom(other.m_atom),
        m_hinstance(other.m_hinstance)
      {
        other.m_atom = 0;
      }


      ~RegisterClassAtom() noexcept
      {
        if (m_atom)
        {
          //BOOL
          ::UnregisterClass(MAKEINTRESOURCE(m_atom), m_hinstance);
        }
      }


      ATOM get() const noexcept
      {
        return m_atom;
      }
    };


#endif

    //=========================================================================


  } // namespace Win32

} // namespace WacomGSS


#endif // WacomGSS_Win32_windows_hpp
