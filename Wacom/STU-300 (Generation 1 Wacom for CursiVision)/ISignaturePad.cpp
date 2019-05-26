
#include "SignaturePad.h"


   long SignaturePad::Start() {
   onCDllCallback_ext(signaturePadCallback,this);
   BeginCapture();
   enableTimecount();
   return S_OK;
   }


   long SignaturePad::Stop() {
   EndCapture();
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
   ClearScreen();
   return;
   }


   long SignaturePad::ShowProperties() {
   IUnknown *pIUnknown;
   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndFrameWindow,pIUnknown);
   pIUnknown -> Release();
   return S_OK;
   }