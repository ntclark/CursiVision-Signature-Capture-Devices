
#include <windows.h>

#include <tchar.h>
#include <olectl.h>

#include "TabletPC.h"


   STDMETHODIMP TabletPC::GetWindow(HWND *pHwnd) {
   *pHwnd = hwndSigPlusFrame;
   return S_OK;
   }
 
   STDMETHODIMP TabletPC::ContextSensitiveHelp(BOOL) {
   return E_NOTIMPL;
   }
 
   STDMETHODIMP TabletPC::InPlaceActivate() {
   return S_OK;
   }

   STDMETHODIMP TabletPC::InPlaceDeactivate() {
   return S_OK;
   }
 
   STDMETHODIMP TabletPC::UIDeactivate() {
   return S_OK;
   }
 
   STDMETHODIMP TabletPC::SetObjectRects(LPCRECT pRectPos,LPCRECT pRectClip) {
//NTC   Resize(pRectPos -> right - pRectPos -> left,pRectPos -> bottom - pRectPos -> top);
   return S_OK;
   }
 
   STDMETHODIMP TabletPC::ReactivateAndUndo() {
   return S_OK;
   }