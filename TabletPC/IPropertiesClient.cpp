/*

                       Copyright (c) 2009 Nathan T. Clark

*/

#include "TabletPC.h"


   long __stdcall TabletPC::_IGPropertiesClient::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown*>(this); 
   else

   if ( riid == IID_IDispatch )
      *ppv = this;
   else

   if ( riid == IID_IGPropertyPageClient )
      *ppv = static_cast<IGPropertiesClient *>(this);
   else
 
      return pParent -> QueryInterface(riid,ppv);
 
   static_cast<IUnknown*>(*ppv) -> AddRef();
  
   return S_OK; 
   }
 
   unsigned long __stdcall TabletPC::_IGPropertiesClient::AddRef() {
   return 1;
   }
 
   unsigned long __stdcall TabletPC::_IGPropertiesClient::Release() {
   return 1;
   }


   HRESULT TabletPC::_IGPropertiesClient::SavePrep() {
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertiesClient::InitNew() {
   return Loaded();
   }


   HRESULT TabletPC::_IGPropertiesClient::Loaded() {
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertiesClient::Saved() {
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertiesClient::IsDirty() {
   return S_FALSE;
   }

   HRESULT TabletPC::_IGPropertiesClient::GetClassID(BYTE *pCLSID) {
   return E_NOTIMPL;
   }
