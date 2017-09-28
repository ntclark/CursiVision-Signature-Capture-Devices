
#include "TabletPC.h"

#include <msinkaut_i.c>

   long TabletPC::HostHandle(HWND hwnd) {
   hwndClient = hwnd;
   return S_OK;
   }


   long TabletPC::Load(char *pszDeviceName,HWND hwndNewClient,void *pvICursiVisionServices) {

   pICursiVisionServices = reinterpret_cast<ICursiVisionServices *>(pvICursiVisionServices);

   if ( ! hwndNewClient )
      return 0L;

   hwndClient = hwndNewClient;

   IInkTablet *pIInkTablet = NULL;

   if ( ! pIInkCollector ) {

      HRESULT rc = CoCreateInstance(CLSID_InkCollector,NULL,CLSCTX_ALL,IID_IInkCollector,reinterpret_cast<void **>(&pIInkCollector));

      if ( ! ( S_OK == rc ) ) {
         MessageBox(NULL,"The Windows compatible TabletPC OS Components are not installed on this machine.","Error",MB_ICONEXCLAMATION | MB_TOPMOST);
         exit(0);
      }

      pIInkCollectorEvents = new IInkCollectorEvents(this);

      IUnknown *pIUnknown = NULL;

      pIInkCollectorEvents -> QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));

      pIInkCollector -> QueryInterface(IID_IConnectionPointContainer,reinterpret_cast<void**>(&pIConnectionPointContainer));

      if ( pIConnectionPointContainer ) {
         HRESULT hr = pIConnectionPointContainer -> FindConnectionPoint(DIID__IInkCollectorEvents,&pIConnectionPoint);
         if ( pIConnectionPoint )
            hr = pIConnectionPoint -> Advise(pIUnknown,&dwConnectionCookie);
      }

      IInkTablets *pIInkTablets = NULL;

      rc = CoCreateInstance(CLSID_InkTablets,NULL,CLSCTX_ALL,IID_IInkTablets,reinterpret_cast<void **>(&pIInkTablets));

#if 0
#ifdef EVALUATION_BUILD

      IUnknown *pIEnumVARIANT_Unknown = NULL;

      pIInkTablets -> get__NewEnum(&pIEnumVARIANT_Unknown);

      if ( pIEnumVARIANT_Unknown ) {

         ULONG countRetrieved = 1L;

         do {
            VARIANT pTablet;
            IEnumVARIANT *pIEnumVARIANT = NULL;
            if ( S_OK == pIEnumVARIANT_Unknown -> QueryInterface(IID_IEnumVARIANT,reinterpret_cast<void **>(&pIEnumVARIANT)) ) {

               pIEnumVARIANT -> Next(1,&pTablet,&countRetrieved);

               if ( countRetrieved ) {
                  IInkTablet2 *pThisTablet = NULL;
                  IDispatch *pIDispatch = pTablet.pdispVal;
                  if ( S_OK == pIDispatch -> QueryInterface(IID_IInkTablet2,reinterpret_cast<void **>(&pThisTablet)) ) {
                     enum TabletDeviceKind deviceKind;
                     pThisTablet -> get_DeviceKind(&deviceKind);
                     pThisTablet -> Release();
                     if ( TDK_Mouse == deviceKind ) {
                        pIDispatch -> QueryInterface(IID_IInkTablet,reinterpret_cast<void **>(&pIInkTablet));
                        break;
                     }
                  }
               }

               pIEnumVARIANT -> Release();

            }

         } while ( countRetrieved );

      }

#else

      pIInkTablets -> get_DefaultTablet(&pIInkTablet);

#endif
#else

      pIInkTablets -> get_DefaultTablet(&pIInkTablet);

#endif


      pIInkTablets -> Release();

   }

   pIInkCollector -> put_hWnd((LONG_PTR)hwndClient);

   hdcClient = GetDC(hwndClient);

   pIInkCollector -> put_Enabled(VARIANT_FALSE);

   pIInkCollector -> SetAllTabletsMode(VARIANT_FALSE);
   
   pIInkCollector -> SetEventInterest(ICEI_AllEvents,VARIANT_FALSE);

   pIInkCollector -> put_AutoRedraw(VARIANT_FALSE);

   pIInkCollector -> put_DynamicRendering(VARIANT_FALSE);

   pIInkCollector -> get_Renderer(&pIInkRenderer);

   return S_OK;
   }


   long TabletPC::Start() {

   if ( ! pIInkCollector )
      return E_UNEXPECTED;

   IInkRectangle *pIInkRectangle = NULL;

   HRESULT rc = CoCreateInstance(CLSID_InkRectangle,NULL,CLSCTX_ALL,IID_IInkRectangle,reinterpret_cast<void **>(&pIInkRectangle));

   RECT rcClient;
   GetWindowRect(hwndClient,&rcClient);

   pIInkRectangle -> SetRectangle(rcClient.top,rcClient.left,rcClient.bottom,rcClient.right);

   pIInkCollector -> SetWindowInputRectangle(pIInkRectangle);

   pIInkRectangle -> Release();
      
   pIInkCollector -> SetEventInterest(ICEI_AllEvents,VARIANT_TRUE);

   pIInkCollector -> put_CollectionMode(ICM_InkOnly);

   pIInkCollector -> put_Enabled(VARIANT_TRUE);

   return S_OK;
   }


   long TabletPC::Unload() {
   return S_OK;
   }

   long TabletPC::Stop() {
   if ( ! pIInkCollector )
      return E_UNEXPECTED;
   pIInkCollector -> put_Enabled(VARIANT_FALSE);
   pIInkCollector -> SetEventInterest(ICEI_AllEvents,VARIANT_FALSE);
   ReleaseDC(hwndClient,hdcClient);
   return S_OK;
   }

   void TabletPC::PrepareForPage(RECT *pRect) {
   if ( ! pRect )
      return;
   memcpy(&rcClient,pRect,sizeof(RECT));
   return;
   }

   long TabletPC::MaximumX() {
   RECT rcClient;
   GetWindowRect(hwndClient,&rcClient);
   return rcClient.right - rcClient.left;
   }

   long TabletPC::MaximumY() {
   RECT rcClient;
   GetWindowRect(hwndClient,&rcClient);
   return rcClient.bottom - rcClient.top;
   }

   long TabletPC::MinimumX() {
   return 0;
   }

   long TabletPC::MinimumY() {
   return 0;
   }

   long TabletPC::MaximumSignableY() {
   return MaximumY();
   }

   long TabletPC::FireOption(long optionNumber) {
   connectionPointContainer.fire_OptionSelected(optionNumber);
   return S_OK;
   }


   long TabletPC::ShowProperties() {
   IUnknown *pIUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndClient,pIUnknown);
   pIUnknown -> Release();
   return S_OK;
   }