/// @file      WacomGSS/Win32/dispex.hpp
/// @copyright Copyright (c) 2013 Wacom Company Limited
/// @author    mholden
/// @date      2013-05-22
/// @brief     provides wrapper for d2d1.h

#ifndef WacomGSS_Win32_dispex_hpp
#define WacomGSS_Win32_dispex_hpp

#include <WacomGSS/Win32/com.hpp>
#pragma warning(push)
#pragma warning(disable:4917 4265)
#include <dispex.h>
#include <functional>
#pragma warning(pop)

namespace WacomGSS
{
  namespace Win32
  {

    struct TypeInfoTypeTrait_TYPEATTR
    {
      typedef TYPEATTR value_type;
      
      static HRESULT get(Win32::com_ptr<ITypeInfo> const & pITypeInfo, value_type * * pTypeAttr)
      {
        return pITypeInfo->GetTypeAttr(pTypeAttr);
      }
      
      static void release(Win32::com_ptr<ITypeInfo> const & pITypeInfo, value_type * pTypeAttr)
      {
        pITypeInfo->ReleaseTypeAttr(pTypeAttr);
      }
    };

    template<class TypeTrait>
    class TypeInfoType : noncopyable
    {
      typedef typename TypeTrait::value_type value_type;
      Win32::com_ptr<ITypeInfo> m_pITypeInfo;
      value_type * m_pType;

      struct boolean_value { int value; }; typedef int boolean_value::* boolean_type;

    protected:
      TypeInfoType(Win32::com_ptr<ITypeInfo> const & pITypeInfo, std::function<HRESULT (Win32::com_ptr<ITypeInfo> const &, value_type * * pType)> get)
      :
        m_pITypeInfo(pITypeInfo),
        m_pType(nullptr)
      {
        hresult_succeeded(get(m_pITypeInfo, &m_pType));
      }

    public:     
      TypeInfoType(Win32::com_ptr<ITypeInfo> const & pITypeInfo)
      :
        m_pITypeInfo(pITypeInfo),
        m_pType(nullptr)
      {
        hresult_succeeded(TypeTrait::get(m_pITypeInfo, &m_pType));
      }

      ~TypeInfoType() noexcept
      {
        if (m_pITypeInfo && m_pType)
        {          
          TypeTrait::release(m_pITypeInfo, m_pType);
        }
      }

      value_type * operator ->() const
      {
        if (!m_pType) 
        {
          throw_HRESULT_error(E_POINTER);
        }
        return m_pType;
      }

      operator boolean_type () const noexcept
      {
        return m_pType ? &boolean_value::value : nullptr;
      }

    };    
    typedef TypeInfoType<TypeInfoTypeTrait_TYPEATTR> TypeAttr;



    struct TypeInfoTypeTrait_FUNCDESC
    {
      typedef FUNCDESC value_type;
      static void release(Win32::com_ptr<ITypeInfo> const & pITypeInfo, value_type * pFuncDesc)
      {
        pITypeInfo->ReleaseFuncDesc(pFuncDesc);
      }
    };

    class FuncDesc : public TypeInfoType<TypeInfoTypeTrait_FUNCDESC>
    {
      typedef TypeInfoType<TypeInfoTypeTrait_FUNCDESC> base;

    public:
      FuncDesc(FuncDesc &&) = delete;
      FuncDesc(Win32::com_ptr<ITypeInfo> const & pITypeInfo, UINT index)
      :
        base(pITypeInfo, [index] (Win32::com_ptr<ITypeInfo> const & pITypeInfo, FUNCDESC * * pValue) -> HRESULT { return pITypeInfo->GetFuncDesc(index, pValue); } )
      {
      }
    };


  }
}

#endif // WacomGSS_Win32_dispex_hpp
