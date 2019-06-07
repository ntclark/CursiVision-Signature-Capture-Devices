
#include "SignaturePad.h"


   long SignaturePad::Start() {

   if ( ! pTablet )
      return 0;

   pTablet -> setInkingMode(WacomGSS::STU::Protocol::InkingMode::InkingMode_On);

   signatureActivityStopRequested = false;

   unsigned int threadAddr;
   hActivityMonitorThread = (HANDLE)_beginthreadex(NULL,4096,signatureActivityMonitor,(void *)this,CREATE_SUSPENDED,&threadAddr);
   ResumeThread(hActivityMonitorThread);

#if 0
   pTablet -> startCapture(0);
   onCDllCallback(signaturePadCallback,this);
   StartCapture();
//
//NTC 05-08-2019: The following was used for the Wacom 520 color pad and is not available in the source code I have for the
// STUTabletCore '.dll.
//
#if 0
   enableTimecount();
#endif
#endif
   return S_OK;
   }


   long SignaturePad::Stop() {

   if ( ! pTablet )
      return 0;

   pTablet -> queueSetPredicateAll(true);
   pTablet -> queueNotifyAll();

   pTablet -> setInkingMode(WacomGSS::STU::Protocol::InkingMode::InkingMode_Off);
   signatureActivityStopRequested = true;
   while ( hActivityMonitorThread )
      Sleep(100);
   return S_OK;
   }

   double SignaturePad::SetZoomFactor(double) {
   return 0.0;
   }

   short SignaturePad::KeyPadQueryHotSpot(short) {
   return 0;
   }

   void SignaturePad::SetBackgroundHandle(OLE_HANDLE hBitmap) {
   return;
   }

   void SignaturePad::PutLCDCaptureMode(long) {
   return;
   }

   void SignaturePad::LCDRefresh(long,long,long,long,long) {
   if ( pTablet )
      pTablet -> setClearScreen();
   return;
   }


   long SignaturePad::ShowProperties() {
   IUnknown *pIUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndFrameWindow,pIUnknown);
   pIUnknown -> Release();
   return S_OK;
   }