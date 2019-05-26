
#include "SignaturePad.h"

#include <commctrl.h>

   long SignaturePad::loaded(char *pszDeviceName,HWND hwndClientHost,void *pvICursiVisionServices) {

//   if ( pszDeviceName )
//      strcpy(signatureDeviceName,pszDeviceName);

   pICursiVisionServices = reinterpret_cast<ICursiVisionServices *>(pvICursiVisionServices);

   if ( isLoaded )
      return S_OK;

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);
 
   hotSpots.clear();

   isActive = true;

   HRESULT hr = CoCreateInstance(CLSID_STFamilyDevicesManager,NULL,CLSCTX_INPROC_SERVER,IID_ISTFamilyDevicesManager,(void**)&pISTFamilyDevicesManager);

   if ( S_OK != hr ) {
      char szMessage[1024];
      LoadString(hModule,IDS_NOT_INSTALLED,szMessage,1024);
      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);
      return 0L;
   }

   long padCount = 0L;

   pISTFamilyDevicesManager -> GetNumberOfSTFamilyDevices(&padCount);

   if ( 0 == padCount ) {
      pISTFamilyDevicesManager -> Release();
      pISTFamilyDevicesManager = NULL;
      return E_FAIL;
   }

   isLoaded = true;

   hr = CoCreateInstance(CLSID_ST1500U, NULL, CLSCTX_INPROC_SERVER, IID_IST1500U,(void**)&pIST1500U);

   if ( S_OK != hr ) {
      char szMessage[1024];
      LoadString(hModule,IDS_NOT_INSTALLED,szMessage,1024);
      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);
      return 0L;
   }

   hr = CoCreateInstance(CLSID_STSignaturePoint, NULL, CLSCTX_INPROC_SERVER, IID_ISTSignaturePoint, (void **)&pISTSignaturePoint);

   if ( S_OK != hr ) {
      char szMessage[1024];
      LoadString(hModule,IDS_NOT_INSTALLED,szMessage,1024);
      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);
      return 0L;
   }

   hr = pIST1500U -> QueryInterface(IID_ISTFamilyDevice,(void **)&pISTFamilyDevice);

   BSTR bstrDevice = NULL;

   pIST1500U -> get_DeviceName(&bstrDevice);

   if ( bstrDevice ) {
      WideCharToMultiByte(CP_ACP,0,bstrDevice,-1,signatureDeviceName,sizeof(signatureDeviceName),0,0);
      SysFreeString(bstrDevice);
   }

   pISTFamilyDevicesManager -> GetSTFamilyDevice(0, &pISTFamilyDevice);

   pISTFamilyDevice -> Open(&hr);

   pISTFamilyDevice -> AddButtonSelectedEventListener();

   pISTFamilyDevice -> AddContainerSelectedEventListener();

   pISTFamilyDevice -> GetCurrentDisplay(&pISTFamilyDeviceDisplay);

   LCDRefresh(0,0,0,0,0);

   return S_OK;
   }


   long SignaturePad::unload() {

   if ( ! isLoaded )
      return 0L;

   LCDRefresh(0,0,0,0,0);

   ClearTablet();

   isLoaded = false;

   if ( ! isActive )
      return 0;

   Stop();

   HRESULT rc;

//NTC:
//TODO - I'm not yet comfortable with the synchronization with the Scriptel pad and
// the thread that has to communicate with it.
// At real issue is COM apartment threads - seems difficult to stop the pad in 
// a highly controlled way
//
//   CRITICAL_SECTION criticalSection = {0};
//   InitializeCriticalSection(&criticalSection);
//   EnterCriticalSection(&criticalSection);

   if ( pISTFamilyDevice )
      pISTFamilyDevice -> Close(&rc);

   if ( pISTFamilyDeviceDisplay )
      pISTFamilyDeviceDisplay -> Release();

   if ( pISTSignaturePoint )
      pISTSignaturePoint -> Release();

   if ( pISTFamilyDevice )
      pISTFamilyDevice -> Release();

   if ( pIST1500U )
      pIST1500U -> Release();

   if ( pISTFamilyDevicesManager )
      pISTFamilyDevicesManager -> Release();

   pISTFamilyDevicesManager = NULL;
   pIST1500U = NULL;
   pISTFamilyDeviceDisplay = NULL;
   pISTFamilyDevice = NULL;
   pISTSignaturePoint = NULL;

   //LeaveCriticalSection(&criticalSection);

   isActive = false;

   return S_OK;
   }
