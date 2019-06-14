/*

                       Copyright (c) 2009,2010 Nathan T. Clark

*/

#include "TabletPC.h"


   long __stdcall TabletPC::_IGPropertyPageClient::QueryInterface(REFIID riid,void **ppv) {
   *ppv = NULL; 
 
   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown*>(this); 
   else

   if ( riid == IID_IDispatch )
      *ppv = this;
   else

   if ( riid == IID_IGPropertyPageClient )
      *ppv = static_cast<IGPropertyPageClient*>(this);
   else
 
      return pParent -> QueryInterface(riid,ppv);
 
   static_cast<IUnknown*>(*ppv) -> AddRef();
  
   return S_OK; 
   }
 
   unsigned long __stdcall TabletPC::_IGPropertyPageClient::AddRef() {
   return 1;
   }
 
   unsigned long __stdcall TabletPC::_IGPropertyPageClient::Release() {
   return 1;
   }


   HRESULT TabletPC::_IGPropertyPageClient::BeforeAllPropertyPages() {
   pParent -> pIGProperties -> Push();
   pParent -> pIGProperties -> Push();
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::GetPropertyPagesInfo(long* pCntPages,SAFEARRAY** thePageNames,SAFEARRAY** theHelpDirs,SAFEARRAY** pSize) {

   if ( ! pCntPages ) return E_POINTER;
   if ( ! thePageNames ) return E_POINTER;
   if ( ! theHelpDirs ) return E_POINTER;
   if ( ! pSize ) return E_POINTER;

   *pCntPages = 1;

   SAFEARRAYBOUND rgsabound[1];
   long index = 0;

   rgsabound[0].cElements = *pCntPages;
   rgsabound[0].lLbound = 1;
   SafeArrayRedim(*thePageNames,rgsabound);
   SafeArrayRedim(*theHelpDirs,rgsabound);
   rgsabound[0].cElements = 2 * *pCntPages;
   SafeArrayRedim(*pSize,rgsabound);

   BSTR bstrName = SysAllocStringLen(NULL,(DWORD)strlen(pParent -> signatureDeviceProductName) + 1);

   MultiByteToWideChar(CP_ACP,0,pParent -> signatureDeviceProductName,-1,bstrName,(DWORD)strlen(pParent -> signatureDeviceProductName) + 1);

   index = 0;
   index++; SafeArrayPutElement(*thePageNames,&index,bstrName);

   SysFreeString(bstrName);

   index = 0;
   index++; SafeArrayPutElement(*theHelpDirs,&index,NULL);

   index = 0;
   long sizeW = PROPERTY_PAGE_WIDTH,sizeH = PROPERTY_PAGE_HEIGHT;
   index++; SafeArrayPutElement(*pSize,&index,&sizeW);
   index++; SafeArrayPutElement(*pSize,&index,&sizeH);

   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::CreatePropertyPage(long pageNumber,HWND hwndParent,RECT* pRect,BOOL fModal,HWND *pHwnd) {

   if ( ! pHwnd ) return E_POINTER;

   if ( ! hwndParent ) return E_POINTER;

   if ( 0 == pageNumber ) {
      DLGTEMPLATE *dt = (DLGTEMPLATE *)LoadResource(hModule,FindResource(hModule,MAKEINTRESOURCE(IDD_PAD_PROPERTIES),RT_DIALOG));
      pParent -> hwndSettings = CreateDialogIndirectParam(hModule,dt,(HWND)hwndParent,(DLGPROC)TabletPC::settingsHandler,(ULONG_PTR)(void *)pParent);
      *pHwnd = pParent -> hwndSettings;
      return S_OK;
   }

   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::Apply() {
   SendMessage(pParent -> hwndSettings,WM_COMMAND,MAKELPARAM(IDDI_PAD_PROPERTIES_ACCEPT,0),0L);
   pParent -> pIGProperties -> Discard();
   pParent -> pIGProperties -> Push();
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::IsPageDirty(long pageNumber,BOOL* isDirty) {
   pParent -> pIGProperties -> Compare((VARIANT_BOOL*)isDirty);
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::Help(BSTR bstrHelpDir) {
   return  S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::TranslateAccelerator(long,long* pResult) {
   *pResult = S_FALSE;
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::AfterAllPropertyPages(BOOL userCanceled) {
   if ( userCanceled ) {
      pParent -> pIGProperties -> Pop();
      pParent -> pIGProperties -> Pop();
   } else {
      SendMessage(pParent -> hwndSettings,WM_COMMAND,MAKELPARAM(IDDI_PAD_PROPERTIES_ACCEPT,0),0L);
      pParent -> pIGProperties -> Discard();
      pParent -> pIGProperties -> Discard();
   }
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::DestroyPropertyPage(long index) {
   if ( pParent -> hwndSettings ) 
      DestroyWindow(pParent -> hwndSettings);
   pParent -> hwndSettings = NULL;
   pParent -> pIGProperties -> Save();
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::GetPropertySheetHeader(void *pv) {

   if ( ! pv )
      return E_POINTER;

   PROPSHEETHEADER *pHeader = reinterpret_cast<PROPSHEETHEADER *>(pv);

   pHeader -> dwFlags = PSH_PROPSHEETPAGE | PSH_NOCONTEXTHELP;
   pHeader -> hInstance = hModule;
   pHeader -> pszIcon = NULL;
   pHeader -> pszCaption = " Properties";
   pHeader -> pfnCallback = NULL;

   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::get_PropertyPageCount(long *pCount) {
   if ( ! pCount )
      return E_POINTER;
   *pCount = 1;
   return S_OK;
   }


   HRESULT TabletPC::_IGPropertyPageClient::GetPropertySheets(void *pPages) {

   PROPSHEETPAGE *pPropSheetPages = reinterpret_cast<PROPSHEETPAGE *>(pPages);

   pPropSheetPages[0].dwFlags = PSP_USETITLE;
   pPropSheetPages[0].dwSize = sizeof(PROPSHEETPAGE);
   pPropSheetPages[0].hInstance = hModule;
   pPropSheetPages[0].pszTemplate = MAKEINTRESOURCE(IDD_PAD_PROPERTIES);
   pPropSheetPages[0].pfnDlgProc = (DLGPROC)TabletPC::settingsHandler;
   pPropSheetPages[0].pszTitle = pParent -> signatureDeviceProductName;
   pPropSheetPages[0].lParam = (UINT_PTR)pParent;
   pPropSheetPages[0].pfnCallback = NULL;

   return S_OK;
   }

