/// @file      WacomGSS/Win32/SafeArray.hpp
/// @copyright Copyright (c) 2012 Wacom Company Limited
/// @author    mholden
/// @date      2012-08-28
/// @brief     provides helper classes for the Windows SAFEARRAY struct

#ifndef WacomGSS_Win32_SafeArray_hpp
#define WacomGSS_Win32_SafeArray_hpp

#include <WacomGSS/win32/com.hpp>

namespace WacomGSS
{
  namespace Win32
  {

    class SafeArrayBase
    {
      ::SAFEARRAY * m_sa;

      typedef pointer_initialize<SAFEARRAY*, SafeArrayBase> pointer;

    public:
      SafeArrayBase() noexcept
      :
        m_sa(nullptr)
      {
      }

      SafeArrayBase(SafeArrayBase && other)
      :
        m_sa(other.release())
      {
      }

      SafeArrayBase(VARTYPE vt, LONG lLbound, ULONG cElements)
      :
        m_sa(::SafeArrayCreateVector(vt, lLbound, cElements))
      {
      }

      ~SafeArrayBase() noexcept
      {
        if (m_sa)
        {
          //HRESULT
          ::SafeArrayDestroy(m_sa);
        }
      }

      ::SAFEARRAY * release() noexcept
      {
        ::SAFEARRAY * sa = m_sa;
        m_sa = nullptr;
        return sa;
      }

      ::SAFEARRAY * sa() const noexcept
      {
        return m_sa;
      }


      void reset(::SAFEARRAY * sa, nothrow_t) noexcept
      {
        if (m_sa && m_sa != sa)
        {
          auto m = m_sa;
          m_sa = nullptr;

          //HRESULT hr =
          ::SafeArrayDestroy(m);
        }
        m_sa = sa;
      }

      pointer initialize(nothrow_t) noexcept
      {
        return pointer(m_sa, *this);
      }
    };


    template<typename Type>
    inline VARTYPE getVarType()
    {
      static_assert(false, "Type not defined"); return VT_EMPTY;
    }

    template<> 
    inline VARTYPE getVarType<std::uint8_t>() { return VT_UI1; }
    
    template<> 
    inline VARTYPE getVarType<std::uint16_t>() { return VT_UI2; }



    template<class Container>
    SafeArrayBase toSafeArray(Container const & container)
    {
      SafeArrayBase safeArray(getVarType<typename Container::value_type>(), 0, static_cast<ULONG>(container.size()));

      SafeArrayAccess access(safeArray.sa());
      std::copy(container.begin(), container.end(), stdext::make_checked_array_iterator(reinterpret_cast<typename Container::value_type *>(access.data()), container.size()));

      return std::move(safeArray);
    }



    template<class Iterator>
    SafeArrayBase toSafeArray(Iterator begin, Iterator end)
    {
      SafeArrayBase safeArray(getVarType<typename Iterator::value_type>(), 0, end-begin);

      SafeArrayAccess access(safeArray.data());
      std::copy(begin, end, reinterpret_cast<typename Iterator::value_type *>(access.data()));

      return std::move(safeArray);
    }



    template<class Iterator, class Size>
    SafeArrayBase toSafeArray_n(Iterator begin, Size size)
    {
      SafeArrayBase safeArray(getVarType<typename std::iterator_traits<Iterator>::value_type>(), 0, size);

      SafeArrayAccess access(safeArray.sa());
      std::copy_n(begin, size, stdext::make_checked_array_iterator(reinterpret_cast<typename std::iterator_traits<Iterator>::value_type *>(access.data()), size));
     
      return std::move(safeArray);
    }


    class SafeArrayAccess : noncopyable
    {
      ::SAFEARRAY * m_sa;
      LPVOID        m_p;
  
    public:
      SafeArrayAccess(::SAFEARRAY * sa)
      :
        m_sa(sa),
        m_p(nullptr)
      {
        LPVOID p = nullptr;
        hresult_succeeded(::SafeArrayAccessData(m_sa, &p));
        m_p = p;
      }

      ~SafeArrayAccess() noexcept
      {
        if (m_p)
        {
          //HRESULT
          ::SafeArrayUnaccessData(m_sa);
        }
      }


      ::SAFEARRAY * sa() const noexcept
      {
        return m_sa;
      }


      LPVOID data() const noexcept
      {
        return m_p;
      }


      template<typename T> 
      T * elements() const noexcept
      {
        return reinterpret_cast<T *>(m_p);
      }
    
    };


    template<typename T>
    inline bool is1dArray(::SAFEARRAY * sa)
    {
      VARTYPE vt;
      hresult_succeeded( ::SafeArrayGetVartype(sa, &vt), "SafeArrayGetVartype" );
      return vt == getVarType<T>() && ::SafeArrayGetDim(sa) == 1 && ::SafeArrayGetElemsize(sa) == sizeof(T);
    }

    inline bool isByteArray(::SAFEARRAY * sa)
    {
      return is1dArray<std::uint8_t>(sa);
    }



  }
}

#endif
