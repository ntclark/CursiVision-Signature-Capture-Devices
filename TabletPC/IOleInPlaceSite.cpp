
#include "TabletPC.h"

   long __stdcall TabletPC::_IOleInPlaceSite::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      return pParent -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleInPlaceSite == riid ) 
      *ppv = static_cast<IOleInPlaceSite*>(this);
   else

      return E_NOINTERFACE;

   AddRef();

   return S_OK;
   }
   unsigned long __stdcall TabletPC::_IOleInPlaceSite::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall TabletPC::_IOleInPlaceSite::Release() {
   return --refCount;
   }


   HRESULT TabletPC::_IOleInPlaceSite::GetWindow(HWND *gwh) {
   *gwh = hwndHost;
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::ContextSensitiveHelp(BOOL fEnterMode) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::CanInPlaceActivate() {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::OnInPlaceActivate() {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::OnUIActivate() {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::GetWindowContext(IOleInPlaceFrame **ppIOleInPlaceFrame,
                                                               IOleInPlaceUIWindow **ppIOleInPlaceUIWindow,
                                                               RECT *pRectPosition,RECT *pRectClip,
                                                               OLEINPLACEFRAMEINFO *pOleInPlaceFrameInfo) {
   *ppIOleInPlaceFrame = pIOleInPlaceFrame;
   *ppIOleInPlaceUIWindow = static_cast<IOleInPlaceUIWindow *>(pIOleInPlaceFrame);

   RECT rcDisplay;
   GetWindowRect(hwndHost,&rcDisplay);

   rcDisplay.right = rcDisplay.right - rcDisplay.left;
   rcDisplay.bottom = rcDisplay.bottom - rcDisplay.top;
   rcDisplay.left = 0;
   rcDisplay.top = 0;

   memcpy(pRectPosition,&rcDisplay,sizeof(RECT));
   memcpy(pRectClip,pRectPosition,sizeof(RECT));

   memset(pOleInPlaceFrameInfo,0,sizeof(OLEINPLACEFRAMEINFO));
   pOleInPlaceFrameInfo -> cb = sizeof(OLEINPLACEFRAMEINFO);
   pOleInPlaceFrameInfo -> fMDIApp = FALSE;
   pOleInPlaceFrameInfo -> hwndFrame = hwndHost;
   pOleInPlaceFrameInfo -> haccel = NULL;
   pOleInPlaceFrameInfo -> cAccelEntries = NULL;   

   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::Scroll(SIZE) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::OnUIDeactivate(BOOL) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::OnInPlaceDeactivate() {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::DiscardUndoState() {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::DeactivateAndUndo() {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceSite::OnPosRectChange(const RECT*) {
   return S_OK;
   }
