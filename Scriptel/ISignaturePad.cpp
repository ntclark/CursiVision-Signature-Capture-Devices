
#include "SignaturePad.h"


   long SignaturePad::Start() {
   Stop();
   LCDRefresh(0,0,0,0,0);
   unsigned int threadAddr;
   hActivityMonitorThread = (HANDLE)_beginthreadex(NULL,4096,signatureActivityMonitor,(void *)this,CREATE_SUSPENDED,&threadAddr);
   ResumeThread(hActivityMonitorThread);
   return S_OK;
   }


   long SignaturePad::Stop() {
   signatureActivityStopRequested = true;
   while ( hActivityMonitorThread )
      Sleep(100);
   //signatureActivityStopRequested = false;
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
   long rc = 0L;
   if ( pISTFamilyDeviceDisplay )
      pISTFamilyDeviceDisplay -> ClearDisplay(&rc);
   return;
   }


   long SignaturePad::ShowProperties() {
   IUnknown *pIUnknown = NULL;
   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndFrameWindow,pIUnknown);
   pIUnknown -> Release();
   return S_OK;
   }