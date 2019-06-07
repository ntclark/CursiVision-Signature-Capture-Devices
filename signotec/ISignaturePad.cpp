
#include "SignaturePad.h"


   long SignaturePad::Start() {
   penIsDown = false;
   STSignatureStop();
   STControlSetCallback(NULL,this);
   STControlSetCallback(&signatureActivityMonitor,this);
   STSignatureStart();
   return S_OK;
   }


   long SignaturePad::Stop() {
   STSensorClearHotSpots();
   STSignatureStop();
   STControlSetCallback(NULL,this);
   STDisplayErase();
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
   STDisplayErase();
   return;
   }

   void SignaturePad::OverlaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   return;
   }

   void SignaturePad::DisplaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   STDisplaySetImage(x,y,(HBITMAP)hBitmap);
   return;
   }

   void SignaturePad::DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad,BOOL isLastPage) {
   OLECHAR bstrFile[MAX_PATH];
   MultiByteToWideChar(CP_ACP,0,pszImageFile,-1,bstrFile,MAX_PATH);
   STDisplaySetImageFromFile(0,0,bstrFile);
   return;
   }

   long SignaturePad::ShowProperties() {
   IUnknown *pIUnknown = NULL;
   QueryInterface(IID_IUnknown,reinterpret_cast<void **>(&pIUnknown));
   pIGProperties -> ShowProperties(hwndFrameWindow,pIUnknown);
   pIUnknown -> Release();
   return S_OK;
   }