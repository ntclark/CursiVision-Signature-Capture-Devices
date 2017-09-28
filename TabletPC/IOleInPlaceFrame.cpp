
#include "TabletPC.h"

   long __stdcall TabletPC::_IOleInPlaceFrame::QueryInterface(REFIID riid,void **ppv) {

   if ( IID_IUnknown == riid ) 
      return pParent -> QueryInterface(riid,ppv);
   else

   if ( IID_IOleInPlaceFrame == riid ) 
      *ppv = static_cast<IOleInPlaceFrame*>(this);
   else

      return E_NOINTERFACE;

   AddRef();

   return S_OK;
   }
   unsigned long __stdcall TabletPC::_IOleInPlaceFrame::AddRef() {
   return ++refCount;
   }
   unsigned long __stdcall TabletPC::_IOleInPlaceFrame::Release() {
   return --refCount;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::GetWindow(HWND *gwh) {
   *gwh = pParent -> hwndSigPlusControlHost;
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::ContextSensitiveHelp(BOOL) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::GetBorder(RECT *) {
   return INPLACE_E_NOTOOLSPACE;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::RequestBorderSpace(LPCBORDERWIDTHS) {
   return INPLACE_E_NOTOOLSPACE;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::SetBorderSpace(LPCBORDERWIDTHS) {
   return OLE_E_INVALIDRECT ;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::SetActiveObject(IOleInPlaceActiveObject *,LPCOLESTR) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::InsertMenus(HMENU,LPOLEMENUGROUPWIDTHS) {
   return E_UNEXPECTED;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::RemoveMenus(HMENU) {
   return E_UNEXPECTED;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::SetMenu(HMENU,HOLEMENU,HWND) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::SetStatusText(LPCOLESTR) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::EnableModeless(BOOL) {
   return S_OK;
   }


   HRESULT TabletPC::_IOleInPlaceFrame::TranslateAccelerator(MSG *,WORD) {
   return S_FALSE;
   }