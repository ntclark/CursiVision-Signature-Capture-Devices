
#include "SignaturePad.h"


   long SignaturePad::Start() {
   Stop();
   scriptel_open_device(pDevice);
   LCDRefresh(0,0,0,0,0);
   unsigned int threadAddr;
   hActivityMonitorThread = (HANDLE)_beginthreadex(NULL,4096,signatureActivityMonitor,(void *)this,CREATE_SUSPENDED,&threadAddr);
   ResumeThread(hActivityMonitorThread);
   return S_OK;
   }


   long SignaturePad::Stop() {
   signatureActivityStopRequested = true;
   scriptel_close_device(pDevice);
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
   scriptel_clear_screen(pDevice);
   return;
   }


   long SignaturePad::ShowProperties() {
   IUnknown *pIUnknown = NULL;
   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndFrameWindow,pIUnknown);
   pIUnknown -> Release();
   return S_OK;
   }