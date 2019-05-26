
#include "SignaturePad.h"

   // IUnknown

   long __stdcall SignaturePad::QueryInterface(REFIID riid,void **ppv) {

   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown *>(this);
   else

   if ( riid == IID_ISignaturePad )
      *ppv = static_cast<ISignaturePad *>(this);
   else

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

      return E_NOINTERFACE;

   AddRef();

   return S_OK;
   }

   unsigned long __stdcall SignaturePad::AddRef() {
   refCount++;
   return refCount;
   }

   unsigned long __stdcall SignaturePad::Release() { 
   refCount--;
   if ( ! refCount ) {
      delete this;
      return 0;
   }
   return refCount;
   }