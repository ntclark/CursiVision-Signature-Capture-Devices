
#include "TabletPC.h"

   // IUnknown

   long __stdcall TabletPC::QueryInterface(REFIID riid,void **ppv) {

   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown *>(this);
   else

   if ( riid == IID_ISignaturePad )
      *ppv = static_cast<ISignaturePad *>(this);
   else

   if ( IID_IOleObject == riid ) 
      return pIOleObject -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleClientSite == riid ) 
      return pIOleClientSite -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      return pIOleInPlaceSite -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleInPlaceFrame == riid ) 
      return pIOleInPlaceFrame -> QueryInterface(riid,ppv);
   else

#if 0
   if ( riid == IID_IGPropertyPageSupport )
      return pIGProperties -> QueryInterface(riid,ppv);
   else
#endif

   if ( riid == IID_IGPropertyPageClient )
      return pIGPropertyPageClient -> QueryInterface(riid,ppv);
   else

   if ( riid == IID_ISpecifyPropertyPages )
      return pIGProperties -> QueryInterface(riid,ppv);
   else

   if ( IID_IPersistStream == riid ) 
      return pIGProperties -> QueryInterface(riid,ppv);
   else

   if ( riid == IID_IConnectionPointContainer ) 
      *ppv = static_cast<IConnectionPointContainer *>(&connectionPointContainer);
   else

   if ( riid == IID_IConnectionPoint ) 
      *ppv = static_cast<IConnectionPoint *>(&connectionPoint);
   else

#if 0
   if ( riid == DIID__DSigPlus )
      *ppv = static_cast<_DSigPlus *>(pIDSigPlus);
   else
#endif

      return E_NOINTERFACE;

   AddRef();

   return S_OK;
   }

   unsigned long __stdcall TabletPC::AddRef() {
   refCount++;
   return refCount;
   }

   unsigned long __stdcall TabletPC::Release() { 
   refCount--;
   if ( ! refCount ) {
      delete this;
      return 0;
   }
   return refCount;
   }