
#include "TabletPC.h"

   long __stdcall TabletPC::_IOleClientSite::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      return pParent -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleClientSite == riid ) 
      *ppv = static_cast<IOleClientSite*>(this);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      return pIOleInPlaceSite -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleInPlaceFrame == riid ) 
      return pIOleInPlaceFrame -> QueryInterface(riid,ppv);
   else

      return pParent -> QueryInterface(riid,ppv);

   AddRef();

   return S_OK;
   }

   unsigned long __stdcall TabletPC::_IOleClientSite::AddRef() {
   return ++refCount;
   }

   unsigned long __stdcall TabletPC::_IOleClientSite::Release() {
   return --refCount;
   }


   HRESULT TabletPC::_IOleClientSite::SaveObject() {
   return S_OK;
   }

   
   HRESULT TabletPC::_IOleClientSite::GetMoniker(DWORD,DWORD,IMoniker**) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleClientSite::GetContainer(IOleContainer **) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleClientSite::ShowObject() {
   ShowWindow(pParent -> hwndSigPlusControlHost,SW_SHOW);
   return S_OK;
   }


   HRESULT TabletPC::_IOleClientSite::OnShowWindow(BOOL) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleClientSite::RequestNewObjectLayout() {
   return S_OK;
   }