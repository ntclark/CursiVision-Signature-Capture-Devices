/// @file      WacomGSS/Win32/com.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides wrapper class for COM objects

#ifndef WacomGSS_Win32_com_hpp
#define WacomGSS_Win32_com_hpp

#include <WacomGSS/win32/windows.hpp>
#pragma warning(push, 3)
#include <objbase.h>
#include <cguid.h>
#pragma warning(pop)

namespace WacomGSS
{
  namespace Win32
  {


    std::error_category const & HRESULT_error_category() noexcept;


    inline std::error_code make_HRESULT_error_code(HRESULT hr)
    {
      return std::error_code(static_cast<int>(hr), HRESULT_error_category());
    }


    WacomGSS_noreturn inline void throw_HRESULT_error(HRESULT hr)
    {
      throw std::system_error(make_HRESULT_error_code(hr));
    }


    template<typename Ty>
    WacomGSS_noreturn inline void throw_HRESULT_error(HRESULT hr, Ty _Message)
    {
      throw std::system_error(make_HRESULT_error_code(hr), _Message);
    }



    inline void hresult_succeeded(HRESULT value)
    {
      if (!SUCCEEDED(value))
      {
        throw_HRESULT_error(value);
      }
    }



    template<typename Ty>
    inline void hresult_succeeded(HRESULT value, Ty _Message)
    {
      if (!SUCCEEDED(value))
      {
        throw_HRESULT_error(value, _Message);
      }
    }



    class ComInitialize : noncopyable
    {
      HRESULT m_hr;

    public:
      ComInitialize(LPVOID pvReserved, DWORD dwCoInit)
      :
        m_hr(::CoInitializeEx(pvReserved, dwCoInit))
      {
        hresult_succeeded(m_hr, "CoInitializeEx");
      }


      ComInitialize(LPVOID pvReserved, DWORD dwCoInit, nothrow_t)
      :
        m_hr(::CoInitializeEx(pvReserved, dwCoInit))
      {
      }


      ComInitialize(nothrow_t)
      :
        m_hr(::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))
      {
        if (m_hr == RPC_E_CHANGED_MODE)
        {
          // Already initialized as multi-threaded. 
          // Call CoInit again to increment ref count for CoUninit in dtor
          m_hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
        }
      }


      ~ComInitialize() noexcept
      {
        if (SUCCEEDED(m_hr))
        {
          //void
          ::CoUninitialize();
        }
      }


      HRESULT hresult() const noexcept
      {
        return m_hr;
      }
    };



    enum addref_t { addref };

    enum queryinterface_t { queryinterface };


    /// @brief WacomGSS wrapper for COM pointers
    template<typename Type>
    class com_ptr
    {
      Type * m_value;

      struct boolean_value { int value; }; typedef int boolean_value::* boolean_type;

      class com_ptrptr : noncopyable
      {
        Type * * m_value;

      public:
        com_ptrptr(Type * * value) noexcept
        :
          m_value(value)
        {
        }
        
        operator Type * * () const noexcept
        {
          return m_value;
        }

        template<typename T = Type, typename std::enable_if<!std::is_same<T,IUnknown>::value, int>::type = 0>
        operator IUnknown * * () const noexcept
        {
          return reinterpret_cast<IUnknown * *>(m_value);
        }

        operator void * * () const noexcept
        {
          return reinterpret_cast<void * *>(m_value);
        }
      };

    private:
      com_ptr & operator = (Type const * other) noexcept;

    public:
      /// @brief Default constructor - intializes to null.
      com_ptr() noexcept
        :
        m_value(nullptr)
      {
      }


      /// @brief Constructor that takes ownership of a raw pointer.
      explicit com_ptr(Type * && takeOwnership) noexcept
      :
        m_value(takeOwnership)
      {
        takeOwnership = nullptr;
      }


      /// @brief Copy constructor - copies the other pointer, adding the reference count.
      explicit com_ptr(com_ptr const & other) noexcept
        :
      m_value(other.get())
      {
        if (m_value)
        {
          m_value->AddRef();
        }
      }


      /// @brief Copy constructor - copies the a raw pointer, adding the reference count.
      explicit com_ptr(Type * value, addref_t) noexcept
      :
        m_value(value)
      {
        if (m_value)
        {
          m_value->AddRef();
        }
      }


      /// @brief Copy constructor - copies the other pointer, adding the reference count.
      template<class OtherType>
      com_ptr(com_ptr<OtherType> const & other, addref_t) noexcept
      :
        m_value(other.get())
      {
        if (m_value)
        {
          m_value->AddRef();
        }
      }


      /// @brief Copy constructor - queries the other pointer for this type
      template<class OtherType>
      com_ptr(com_ptr<OtherType> & other, queryinterface_t)
      :
        m_value(nullptr)
      {
        if (other)
        {
          hresult_succeeded(other->QueryInterface(__uuidof(Type), reinterpret_cast<LPVOID *>(&m_value)), "QueryInterface");
        }
      }


      /// @brief Copy constructor - queries the other pointer for this type
      com_ptr(IUnknown * other, queryinterface_t)
      :
        m_value(nullptr)
      {
        if (other)
        {
          hresult_succeeded(other->QueryInterface(__uuidof(Type), reinterpret_cast<LPVOID *>(&m_value)), "QueryInterface");
        }
      }


      /// @brief Move constructor - moves the pointer to this one; other is set to null.
      template<typename OtherType>
      com_ptr(com_ptr<OtherType> && other) noexcept
      :
        m_value(other.release())
      {
      }


      /// @brief Direct construction of an object
      com_ptr(CLSID clsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext)
      {
        hresult_succeeded(::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(Type), reinterpret_cast<LPVOID *>(&m_value)), "CoCreateInstance");
      }


      /// @brief Direct construction of an object
      com_ptr(CLSID clsid, DWORD dwClsContext)
      {
        hresult_succeeded(::CoCreateInstance(clsid, nullptr, dwClsContext, __uuidof(Type), reinterpret_cast<LPVOID *>(&m_value)), "CoCreateInstance");
      }

      /// @brief Destructor - lowers the reference count.
      ~com_ptr() noexcept
      {
        if (m_value) 
        {
          m_value->Release();
        }
      }


      /// @brief Assign operator with nullptr.
      com_ptr & operator = (nullptr_t) noexcept
      {
        Type * value = m_value;

        m_value = nullptr;

        if (value) 
        {
          value->Release();
        }

        return *this;
      }
   
      /// @brief move operator
      com_ptr & operator = (Type * && other) noexcept
      {
        Type * value = m_value;
      
        m_value = other;
        other = nullptr;

        if (value) 
        {
          value->Release();
        }
      
        return *this;
      }
      
      
      /// @brief Assign (copy).
      com_ptr & operator = (com_ptr const & other) noexcept
      {
        Type * other_value = other.get();
        Type * this_value  = m_value;

        if (other_value) 
        {
          other_value->AddRef();
        }

        m_value = other_value;

        if (this_value) 
        {
          this_value->Release();
        }

        return *this;
      }


      /// @brief Assign (copy).
      template<typename OtherType>
      com_ptr & operator = (com_ptr<OtherType> const & other) noexcept
      {
        Type * other_value = other.get();
        Type * this_value  = m_value;

        if (other_value) 
        {
          other_value->AddRef();
        }

        m_value = other_value;

        if (this_value) 
        {
          this_value->Release();
        }

        return *this;
      }


      /// @brief Assign (move).
      template<typename OtherType>
      com_ptr & operator = (com_ptr<OtherType> && other) noexcept
      {
        Type * value = m_value;

        m_value = other.release();
        
        if (value)
        {
          value->Release();
        }

        return *this;
      }


      /// @brief Assign (move).
      com_ptr & operator = (com_ptr && other) noexcept
      {
        Type * value = m_value;

        m_value = other.release();
        
        if (value)
        {
          value->Release();
        }

        return *this;
      }


      /// @brief bool equivalent - returns true if held pointer isn't null.
      operator boolean_type () const noexcept
      {
        return m_value ? &boolean_value::value : nullptr;
      }


      /// @brief access operator. Will throw if nullptr
      Type * operator -> () const
      {
        if (!*this) 
        {
          throw_HRESULT_error(E_POINTER);
        }

        return m_value;
      }


      Type * get() const noexcept
      {
        return m_value;
      }


      com_ptrptr operator & () noexcept
      {
        Type * value = m_value;
        m_value = nullptr;
        if (value)
        {
          value->Release();
        }
        return com_ptrptr(&m_value);
      }
      
      Type * release() noexcept
      {
        Type * value = m_value;
        m_value = nullptr;
        return value;
      }



      /// @brief creates an instance of the specified class
      HRESULT createInstance(CLSID clsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, nothrow_t) noexcept
      {
        if (m_value)
        {
          Type * value = m_value;
          m_value = nullptr;
          value->Release();
        }

        return ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(Type), reinterpret_cast<LPVOID *>(&m_value));
      }


      /// @brief creates an instance of the specified class
      void createInstance(CLSID clsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext)
      {
        hresult_succeeded(createInstance(clsid, pUnkOuter, dwClsContext, nothrow), "CoCreateInstance");
      }


      /// @brief creates an instance of the specified class
      HRESULT createInstance(CLSID clsid, DWORD dwClsContext, nothrow_t) noexcept
      {
        return createInstance(clsid, nullptr, dwClsContext, nothrow);
      }

      /// @brief creates an instance of the specified class
      void createInstance(CLSID clsid, DWORD dwClsContext)
      {
        createInstance(clsid, nullptr, dwClsContext);
      }


      /// @brief queries other for an instance of this interface
      template<typename OtherType>
      HRESULT queryInterface(com_ptr<OtherType> & other, nothrow_t) noexcept
      {
        if (m_value)
        {
          Type * value = m_value;
          m_value = nullptr;
          value->Release();
        }
        return other->QueryInterface(__uuidof(Type), reinterpret_cast<LPVOID *>(&m_value));
      }


      /// @brief queries other for an instance of this interface
      HRESULT queryInterface(IUnknown * other, nothrow_t) noexcept
      {
        if (m_value)
        {
          Type * value = m_value;
          m_value = nullptr;
          value->Release();
        }

        if (!other)
          return E_POINTER;

        return other->QueryInterface(__uuidof(Type), reinterpret_cast<LPVOID *>(&m_value));
      }


      /// @brief queries other for an instance of this interface
      template<typename OtherType>
      void queryInterface(com_ptr<OtherType> & other)
      {
        hresult_succeeded(queryInterface<OtherType>(other, nothrow), "QueryInterface");
      }


      /// @brief queries other for an instance of this interface
      void queryInterface(IUnknown * other)
      {
        hresult_succeeded(queryInterface(other, nothrow), "QueryInterface");
      }


    };



    class Bstr
    {
      BSTR m_bstr;

      struct boolean_value { int value; }; typedef int boolean_value::* boolean_type;

      class Bstr_ptr : noncopyable
      {
        BSTR * m_value;

      public:
        Bstr_ptr(BSTR * value) noexcept
        :
          m_value(value) 
        {
        }

        operator BSTR * () const noexcept
        {
          return m_value;
        }        
      };

    public:
      Bstr() noexcept
      :
        m_bstr(nullptr)
      {
      }

      Bstr(BSTR && bstr) noexcept
      :
        m_bstr(bstr)
      {
        bstr = nullptr;
      }
      
      Bstr(Bstr && other) noexcept
      :
        m_bstr(other.release())
      {
      }


      Bstr(wchar_t const * s) noexcept
      :
        m_bstr(::SysAllocString(s))
      {
      }

      
      Bstr(wchar_t const * s, UINT len) noexcept
      :
        m_bstr(::SysAllocStringLen(s, len))
      {
      }


      // Does not perform any ANSI-to-Unicode translation.
      Bstr(UINT len, char const * s) noexcept
      :
        m_bstr(::SysAllocStringByteLen(s, len))
      {
      }


      Bstr(Bstr const & other) noexcept
      :
        m_bstr(nullptr)
      {
        if (other.m_bstr)
        {
          UINT len = ::SysStringLen(other.m_bstr);
          m_bstr = ::SysAllocStringLen(other.m_bstr, len);
        }
      }

      /// @brief bool equivilent - returns true if held pointer isn't null.
      operator boolean_type () const noexcept
      {
        return m_bstr ? &boolean_value::value : nullptr;
      }


      void reset() noexcept
      {
        ::SysFreeString(release());
      }


      Bstr & operator = (Bstr const & other) noexcept
      {
        if (other.m_bstr != m_bstr)
        {
          reset();
          if (other.m_bstr)
          {
            UINT len = ::SysStringLen(other.m_bstr);
            m_bstr = ::SysAllocStringLen(other.m_bstr, len);
          } 
        }
        return *this;
      }


      Bstr & operator = (nullptr_t) noexcept
      {
        reset();
        return *this;
      }


      Bstr & operator = (Bstr && other) noexcept
      {
        if (other.m_bstr != m_bstr)
        {
          attach(std::move(other.m_bstr));
        }
        return *this;
      }


      BSTR get() const noexcept
      {
        return m_bstr;
      }


      UINT len() const noexcept
      {
        return ::SysStringLen(m_bstr);
      }


      void attach(BSTR && bstr) noexcept
      {
        reset();
        std::swap(m_bstr, bstr);
      }


      BSTR release() noexcept
      {
        BSTR bstr = m_bstr;
        m_bstr = nullptr;
        return bstr;
      }


      Bstr_ptr operator & () 
      {
        reset();
        return &m_bstr;
      }
    };



    namespace Bstr_ostream_operator_utf8
    {
      std::ostream & operator << (std::ostream & o, Win32::Bstr const & bstr);
    }



    struct ExcepInfo : noncopyable, ::EXCEPINFO
    {
      ExcepInfo()
      {
        wCode             = 0;
        wReserved         = 0;
        bstrSource        = nullptr;
        bstrDescription   = nullptr;
        bstrHelpFile      = nullptr;
        dwHelpContext     = 0;
        pvReserved        = nullptr;
        pfnDeferredFillIn = nullptr;
        scode             = 0;
      }

      ~ExcepInfo() noexcept
      {
        ::SysFreeString(bstrSource);
        ::SysFreeString(bstrDescription);
        ::SysFreeString(bstrHelpFile);
      }

      HRESULT deferredFillIn() noexcept
      {
        HRESULT hr;
        if (pfnDeferredFillIn)
        {
          hr = pfnDeferredFillIn(this);
          pfnDeferredFillIn = nullptr;
        }
        else
        {
          hr = S_OK;
        }
        return hr;
      }

    };



    template<typename T>
    inline void initializeRetVal(T * * pRetVal)
    {
      *pRetVal = nullptr;
    }



    template<typename T>
    inline void initializeRetVal(T * pRetVal)
    {
      *pRetVal = 0;
    }



    template<>
    inline void initializeRetVal(VARIANT * pRetVal)
    {
      pRetVal->vt = VT_EMPTY;
    }

    template<>
    inline void initializeRetVal(PROPVARIANT * pRetVal)
    {
      pRetVal->vt = VT_EMPTY;
    }

    
    template<>
    inline void initializeRetVal(PROPERTYKEY * pRetVal)
    {
      pRetVal->fmtid = GUID_NULL;
      pRetVal->pid = 0;
    }



    template<>
    inline void initializeRetVal(VARIANT_BOOL * pRetVal)
    {
      *pRetVal = VARIANT_FALSE;
    }

    template<>
    inline void initializeRetVal(UINT * pRetVal)
    {
      *pRetVal = 0;
    }

    template<>
    inline void initializeRetVal(EXCEPINFO * pRetVal)
    {
      ZeroMemory(pRetVal, sizeof(EXCEPINFO));
    }



  } // namespace Win32

} // namespace WacomGSS


#define WacomGSS_initializeRetVal(pRetVal)        \
  {                                               \
    if (!pRetVal)                                 \
      return E_POINTER;                           \
    ::WacomGSS::Win32::initializeRetVal(pRetVal); \
  }

#define WacomGSS_initializeRetVal2(ret1,ret2)     \
  {                                               \
    if (ret1)                                     \
      ::WacomGSS::Win32::initializeRetVal(ret1);  \
    if (ret2)                                     \
      ::WacomGSS::Win32::initializeRetVal(ret2);  \
    if (!ret1 || !ret2)                           \
      return E_POINTER;                           \
  }

#define WacomGSS_initializeRetVal3(ret1,ret2,ret3)\
  {                                               \
    if (ret1)                                     \
      ::WacomGSS::Win32::initializeRetVal(ret1);  \
    if (ret2)                                     \
      ::WacomGSS::Win32::initializeRetVal(ret2);  \
    if (ret3)                                     \
      ::WacomGSS::Win32::initializeRetVal(ret3);  \
    if (!ret1 || !ret2 || !ret3)                  \
      return E_POINTER;                           \
  }

#endif // WacomGSS_Win32_com_hpp
