#include <WacomGSS/Win32/debug_TypeInfo.hpp>
#include <WacomGSS/Win32/dispex.hpp>
#include <locale>
#include <codecvt>
#include <iostream>


namespace WacomGSS
{
  namespace Win32
  {

    using Bstr_ostream_operator_utf8::operator<<;

    class StringifyCustomType : noncopyable
    {
      HREFTYPE const                    m_refType;
      Win32::com_ptr<ITypeInfo> const & m_pITypeInfo;

    public:
      StringifyCustomType(StringifyCustomType &&) = delete;
      StringifyCustomType(HREFTYPE refType, Win32::com_ptr<ITypeInfo> const & pITypeInfo)
      :
        m_refType(refType),
        m_pITypeInfo(pITypeInfo)
      {
      }

      void write(std::ostream & o) const
      {
        HRESULT hr;  
        Win32::com_ptr<ITypeInfo> pRefTypeInfo;
        hr = m_pITypeInfo->GetRefTypeInfo(m_refType, &pRefTypeInfo);
        if (SUCCEEDED(hr))
        {
          Win32::Bstr name;
          hr = pRefTypeInfo->GetDocumentation(-1, &name, 0, 0, 0);
          if (SUCCEEDED(hr))
          {    
            o << name;
          }
          else 
          {
            o << "??";
          }

        }
        else
        {
          o << "UnknownCustomType";
        }
      }

    };



    std::ostream & operator << (std::ostream & o, StringifyCustomType const & s)
    {
      s.write(o);
      return o;
    }



    std::ostream & operator << (std::ostream & o, class StringifyTypeDesc const & s); // forward decl
    //
    class StringifyTypeDesc : noncopyable
    {
      TYPEDESC const *                  m_pTypeDesc;
      Win32::com_ptr<ITypeInfo> const & m_pITypeInfo;

    public:
#if !defined(_MSC_VER) || (_MSC_VER>=1900)
      StringifyTypeDesc(StringifyTypeDesc &&) = default;
#endif
      StringifyTypeDesc(TYPEDESC const * pTypeDesc, Win32::com_ptr<ITypeInfo> const & pITypeInfo) noexcept
      :
        m_pTypeDesc(pTypeDesc),
        m_pITypeInfo(pITypeInfo)
      {
      }  

      void write(std::ostream & o) const
      {
        switch (m_pTypeDesc->vt)
        { 
          case VT_I2          : o << "I2"         ; return;
          case VT_I4          : o << "I4"         ; return;
          case VT_R4          : o << "R4"         ; return;
          case VT_R8          : o << "R8"         ; return;
          case VT_CY          : o << "CY"         ; return;
          case VT_DATE        : o << "DATE"       ; return;
          case VT_BSTR        : o << "BSTR"       ; return;
          case VT_DISPATCH    : o << "DISPATCH"   ; return;
          case VT_ERROR       : o << "ERROR"      ; return;
          case VT_BOOL        : o << "BOOL"       ; return;
          case VT_VARIANT     : o << "VARIANT"    ; return;
          case VT_UNKNOWN     : o << "UNKNOWN"    ; return;
          case VT_DECIMAL     : o << "DECIMAL"    ; return;
          case VT_I1          : o << "I1"         ; return;
          case VT_UI1         : o << "UI1"        ; return;
          case VT_UI2         : o << "UI2"        ; return;
          case VT_UI4         : o << "UI4"        ; return;
          case VT_I8          : o << "I8"         ; return;
          case VT_UI8         : o << "UI8"        ; return;
          case VT_INT         : o << "INT"        ; return;
          case VT_UINT        : o << "UINT"       ; return;
          case VT_INT_PTR     : o << "INT_PTR"    ; return;
          case VT_UINT_PTR    : o << "UINT_PTR"   ; return;
          case VT_VOID        : o << "VOID"       ; return;
          case VT_HRESULT     : o << "HRESULT"    ; return;
          case VT_PTR         : o << StringifyTypeDesc(m_pTypeDesc->lptdesc, m_pITypeInfo) << '*'; return;
          case VT_SAFEARRAY   : o << "SAFEARRAY(" << StringifyTypeDesc(m_pTypeDesc->lptdesc, m_pITypeInfo) << ')'; return;
          case VT_CARRAY      :
            {
              o << StringifyTypeDesc(&m_pTypeDesc->lpadesc->tdescElem, m_pITypeInfo);
              for (USHORT iDim = 0; iDim < m_pTypeDesc->lpadesc->cDims; ++iDim) 
              {
                o << '[' << m_pTypeDesc->lpadesc->rgbounds[iDim].lLbound << "..." << (m_pTypeDesc->lpadesc->rgbounds[iDim].cElements + m_pTypeDesc->lpadesc->rgbounds[iDim].lLbound - 1) << ']';
              }
            }
            return;
          case VT_USERDEFINED : o << StringifyCustomType(m_pTypeDesc->hreftype, m_pITypeInfo); return;
          case VT_LPSTR       : o << "LPSTR"      ; return;
          case VT_LPWSTR      : o << "LPWSTR"     ; return;
        }

        o << "(unexpected VT)";
      }
    };



    std::ostream & operator << (std::ostream & o, StringifyTypeDesc const & s)
    {
      s.write(o);
      return o;
    }


    void debug_TypeInfo(IDispatch * pIDispatch, std::ostream & o)
    {
      HRESULT hr;

      Win32::com_ptr<ITypeInfo> pITypeInfo;  
      Win32::hresult_succeeded(pIDispatch->GetTypeInfo(0, 0, &pITypeInfo));
  
      Win32::TypeAttr pTypeAttr(pITypeInfo);

      Win32::Bstr bstrInterfaceName;
      hr = pITypeInfo->GetDocumentation(-1, &bstrInterfaceName, 0, 0, 0);

      if (SUCCEEDED(hr))
      {
        o << "Interface " << bstrInterfaceName << ':' << std::endl;        
      }
      else
      {
        o << "Unknown default interface:"<<std::endl;
      }  

      for (UINT iFunc = 0; iFunc < pTypeAttr->cFuncs; ++iFunc) 
      {
        Win32::FuncDesc pFuncDesc(pITypeInfo, iFunc);
        
        if (pFuncDesc->wFuncFlags)
        {
          o << '[';
          bool first = true;
          unsigned f = pFuncDesc->wFuncFlags;
          unsigned c;
          if ((f & (c=FUNCFLAG_FRESTRICTED       )) != 0) {                            first = false; f &= ~c; o << "restricted"      ; }
          if ((f & (c=FUNCFLAG_FSOURCE           )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "source"          ; }
          if ((f & (c=FUNCFLAG_FBINDABLE         )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "bindable"        ; }
          if ((f & (c=FUNCFLAG_FREQUESTEDIT      )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "requestedit"     ; }
          if ((f & (c=FUNCFLAG_FDISPLAYBIND      )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "displaybind"     ; }
          if ((f & (c=FUNCFLAG_FDEFAULTBIND      )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "defaultbind"     ; }
          if ((f & (c=FUNCFLAG_FHIDDEN           )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "hidden"          ; }
          if ((f & (c=FUNCFLAG_FUSESGETLASTERROR )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "usesgetlasterror"; }
          if ((f & (c=FUNCFLAG_FDEFAULTCOLLELEM  )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "defaultcollelem" ; }
          if ((f & (c=FUNCFLAG_FUIDEFAULT        )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "uidefault"       ; }
          if ((f & (c=FUNCFLAG_FNONBROWSABLE     )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "nonbrowsable"    ; }
          if ((f & (c=FUNCFLAG_FREPLACEABLE      )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "replaceable"     ; }
          if ((f & (c=FUNCFLAG_FIMMEDIATEBIND    )) != 0) { if (!first) o << ','; else first = false; f &= ~c; o << "immediatebind"   ; }  
          if (f) o << "0x" << std::hex << f << std::dec;
          o << ']';        
        }

        Win32::Bstr methodName;
        hr = pITypeInfo->GetDocumentation(pFuncDesc->memid, &methodName, 0, 0, 0);
        if (SUCCEEDED(hr))
        {
          o << '\t' << StringifyTypeDesc(&pFuncDesc->elemdescFunc.tdesc, pITypeInfo) << ' ' << methodName << '(';
            
          for (SHORT iParam = 0; iParam < pFuncDesc->cParams; ++iParam) 
          {
            o << StringifyTypeDesc(&pFuncDesc->lprgelemdescParam[iParam].tdesc, pITypeInfo);
            if (iParam < pFuncDesc->cParams - 1)
              o << ", ";
          }
          o << ')';

          switch (pFuncDesc->invkind) 
          {
            case INVOKE_FUNC           : o << " func"       ; break;
            case INVOKE_PROPERTYGET    : o << " propget"    ; break;
            case INVOKE_PROPERTYPUT    : o << " propput"    ; break;
            case INVOKE_PROPERTYPUTREF : o << " propputref" ; break;
          }
          o << std::endl;
        }
        else
        {
          o << "Error In Name" << std::endl; 
        }
        
      }//for

    }



  } // namespace Win32
} // namespace WacomGSS
