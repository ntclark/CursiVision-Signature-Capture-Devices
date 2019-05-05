
#include <windows.h>

#include <olectl.h>

#include "TabletPC.h"

   long __stdcall TabletPC::_IOleObject::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IOleObject )
      *ppv = static_cast<IOleObject *>(this); 
   else
      return pParent -> QueryInterface(riid,ppv);

   AddRef();
  
   return S_OK; 
   }
 
   unsigned long __stdcall TabletPC::_IOleObject::AddRef() {
   return 1;
   }
 
   unsigned long __stdcall TabletPC::_IOleObject::Release() {
   return 1;
   }


   STDMETHODIMP TabletPC::_IOleObject::SetClientSite(IOleClientSite *pcs) {

   pParent -> instantiateSigPlus();

   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetClientSite(IOleClientSite **pcs) {
   *pcs = pParent -> pIOleClientSite_MySite;
   pParent -> pIOleClientSite_MySite -> AddRef();
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::Advise(IAdviseSink *pAdvSink,DWORD *pdwConnection) {
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::Unadvise(DWORD dwConnection) {
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::EnumAdvise(IEnumSTATDATA **ppenum) {
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::SetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   if ( dwDrawAspect != DVASPECT_CONTENT ) return S_OK;
   SIZEL tempSizel;
   RECT rect = {0,0,0,0};
   hiMetricToPixels(pSizel,&tempSizel);
   rect.right = tempSizel.cx;
   rect.bottom = tempSizel.cy;
   SetWindowPos(pParent -> hwndSigPlusFrame,HWND_TOP,rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,0L);
   SetWindowPos(pParent -> hwndSigPlusControlHost,HWND_TOP,0,0,rect.right - rect.left,rect.bottom - rect.top,0L);
   return S_OK;
   }
 
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetExtent(DWORD dwDrawAspect,SIZEL *pSizel) {
   RECT rcParent;
   GetWindowRect(pParent -> hwndSigPlusFrame,&rcParent);
   pSizel -> cx = rcParent.right - rcParent.left;
   pSizel -> cy = rcParent.bottom - rcParent.top;
   pixelsToHiMetric(pSizel,pSizel);
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::DoVerb(LONG iVerb, LPMSG ,IOleClientSite *, LONG,HWND hwndParent,LPCRECT lprcPosRect) {
 
   switch ( iVerb ) {
 
   case OLEIVERB_PROPERTIES: {
      }
      break;
 
   case OLEIVERB_UIACTIVATE:
      break;
 
   case OLEIVERB_HIDE:
      ShowWindow(pParent -> hwndSigPlusFrame,SW_HIDE);
      break;
 
   case OLEIVERB_SHOW: 
 
      if ( ! pParent -> hwndSigPlusFrame )
         pParent -> initializeWindows(hwndParent);
 
      if ( lprcPosRect ) {
 
         SetWindowPos(pParent -> hwndSigPlusFrame,HWND_TOP,
                               lprcPosRect -> left,lprcPosRect -> top,
                               lprcPosRect -> right - lprcPosRect -> left,lprcPosRect -> bottom - lprcPosRect -> top,0L);
 
      }
 
      break;
 
    case OLEIVERB_INPLACEACTIVATE:
       pParent -> SetObjectRects(lprcPosRect,NULL);
       pParent -> pIOleInPlaceSite_MySite -> OnInPlaceActivate();
       break;
 
   default:
      break;
   }
 
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::SetHostNames(LPCOLESTR szContainerApp,LPCOLESTR olestrContainerObject) {
    return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::Close(DWORD dwOptions) {
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::SetMoniker(DWORD dwMonikerNo,IMoniker *pm) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetMoniker(DWORD dwAssign,DWORD dwMonikerNo,IMoniker **ppm) {
   if ( ! pParent -> pIOleClientSite_MySite ) 
      return E_NOTIMPL;
   return pParent -> pIOleClientSite_MySite -> GetMoniker(OLEGETMONIKER_FORCEASSIGN,OLEWHICHMK_OBJFULL,ppm);
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::InitFromData(IDataObject *pdo,BOOL fCreation,DWORD dwReserved) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetClipboardData(DWORD,IDataObject **ppdo) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::Update() {
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::IsUpToDate() {
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetUserClassID(CLSID *pclsid) {
  *pclsid = CLSID_CursiVisionSignaturePad;
   return S_OK;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetUserType(DWORD dwTypeOfType,LPOLESTR *pszTypeName) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::GetMiscStatus(DWORD dwAspect,DWORD *dwStatus) {
   return E_NOTIMPL;
   }
 
 
   STDMETHODIMP TabletPC::_IOleObject::SetColorScheme(LOGPALETTE *) {
   return S_OK;
   }