 
#include <stddef.h>

#include "SignaturePad.h"

   WNDPROC SignaturePad::defaultSwatchHandler = NULL;
   WNDPROC SignaturePad::defaultTextHandler = NULL;

   SignaturePad::SignaturePad(IUnknown *pUnkOuter) :

      connectionPointContainer(this),
      connectionPoint(this),

      pSignatureDataX(NULL),
      pSignatureDataY(NULL),

      pICursiVisionServices(NULL),

      pIGProperties(NULL),
      pIGPropertiesClient(NULL),
      pIGPropertyPageClient(NULL),

      hwndSettings(NULL),
      hwndFrameWindow(NULL),

      displayBitsPerPixel(0),

      gdiplusStartupInput(0),
      gdiplusToken(NULL),

      padWidthInInches(0.0),

      signatureTimeOutPeriod(0.0),

      sendingBitmapThreadAddress(NULL),
      sendingBitmapThreadHandle(NULL),

      penWidth(2L),
      inkColor(RGB(0,0,0)),

      scaleToPadX(1.0),
      scaleToPadY(1.0),

      maximumPenUnitsWidth(0),
      maximumPenUnitsHeight(0),

      pIConnectionPointContainer(NULL),
      pIConnectionPoint(NULL),
      dwConnectionCookie(0L),

      isActive(false),
      isLoaded(false),

      refCount(0)

   {

   memset(signatureDeviceName,0,sizeof(signatureDeviceName));
   memset(signatureDeviceModel,0,sizeof(signatureDeviceModel));
   memset(signatureDeviceProductName,0,sizeof(signatureDeviceProductName));
   memset(deviceDescription,0,sizeof(deviceDescription));

   memset(szOKFile,0,sizeof(szOKFile));
   memset(szClearFile,0,sizeof(szClearFile));

   memset(&thePad,0,sizeof(CAPS));

   memset(&originPoint,0,sizeof(POINTL));
   memset(&baselinePoint,0,sizeof(POINTL));

   initializeParameters();

   HRESULT rc = CoCreateInstance(CLSID_InnoVisioNateProperties,NULL,CLSCTX_ALL,IID_IGProperties,reinterpret_cast<void **>(&pIGProperties));

   pIGPropertiesClient = new _IGPropertiesClient(this);

   pIGProperties -> Advise(static_cast<IGPropertiesClient *>(pIGPropertiesClient));

   pIGPropertyPageClient = new _IGPropertyPageClient(this);

   pIGProperties -> AdvisePropertyPageClient(pIGPropertyPageClient,true);

   char szTemp[MAX_PATH];
   char szRootName[MAX_PATH];      

   strcpy(szRootName,szModuleName);

   char *p = strrchr(szModuleName,'\\');
   if ( ! p )
      p = strrchr(szModuleName,'/');
   if ( p ) {
      strcpy(szRootName,p + 1);
   }

   p = strrchr(szRootName,'.');
   if ( p )
      *p = '\0';

   sprintf(szTemp,"%s\\Settings\\%s.settings",szApplicationDataDirectory,szRootName);

   BSTR bstrFileName = SysAllocStringLen(NULL,MAX_PATH);
   MultiByteToWideChar(CP_ACP,0,szTemp,-1,bstrFileName,MAX_PATH);

   pIGProperties -> put_FileName(bstrFileName);

   SysFreeString(bstrFileName);

   pIGProperties -> Add(L"pad width",NULL);
   pIGProperties -> DirectAccess(L"pad width",TYPE_DOUBLE,&padWidthInInches,sizeof(RECT));

   long n = offsetof(SignaturePad,endPadParameters) - offsetof(SignaturePad,startPadParameters);

   pIGProperties -> Add(L"pad parameters",NULL);
   pIGProperties -> DirectAccess(L"pad parameters",TYPE_BINARY,&startPadParameters,n);

   pIGProperties -> Add(L"Timeout period",NULL);
   pIGProperties -> DirectAccess(L"Timeout period",TYPE_BINARY,&signatureTimeOutPeriod,sizeof(double));

   pIGProperties -> Add(L"bit depth",NULL);
   pIGProperties -> DirectAccess(L"bit depth",TYPE_BINARY,&displayBitsPerPixel,sizeof(displayBitsPerPixel));

   pIGProperties -> Add(L"pen width",NULL);
   pIGProperties -> DirectAccess(L"pen width",TYPE_BINARY,&penWidth,sizeof(penWidth));

   pIGProperties -> Add(L"origin point",NULL);
   pIGProperties -> DirectAccess(L"origin point",TYPE_BINARY,&originPoint,sizeof(POINTL));

   pIGProperties -> Add(L"ink color",NULL);
   pIGProperties -> DirectAccess(L"ink color",TYPE_BINARY,&inkColor,sizeof(inkColor));

   Connect(NULL,0);

   if ( isConnected() ) {

      thePad = GetCapabilities();

      if ( 396 == thePad.lcd_width ) {

         modelId = STU_300;

         strcpy(signatureDeviceName,"STU-300");
         strcpy(signatureDeviceProductName,"Wacom STU-300 LCD");
         strcpy(signatureDeviceModel,"(STU-300)");
         modelHasScaling = true;
         padWidthInInches = 4.25;

         baselinePoint.y = 80;

      } else if ( 800 == thePad.lcd_width ) {

         modelId = STU_520;

         strcpy(signatureDeviceName,"STU-520");
         strcpy(signatureDeviceProductName,"Wacom STU-520 LCD");
         strcpy(signatureDeviceModel,"(STU-520)");
         modelHasScaling = true;
         padWidthInInches = 7.0;

         baselinePoint.y = 0;

      } else if ( 640 == thePad.lcd_width ) {

         strcpy(signatureDeviceName,"STU-500");
         strcpy(signatureDeviceProductName,"Wacom STU-500 LCD");
         strcpy(signatureDeviceModel,"(STU-500)");
         modelHasScaling = true;
         padWidthInInches = 7.0;

         baselinePoint.y = 0;

      } else {

         modelId = STU_300;

         strcpy(signatureDeviceName,"STU-300");
         strcpy(signatureDeviceProductName,"No Pad Connected");
         strcpy(signatureDeviceModel,"(STU-300)");
         modelHasScaling = true;
         padWidthInInches = 4.25;

         baselinePoint.y = 80;

      }

   } else {
      strcpy(signatureDeviceName,"Wacom");
      strcpy(signatureDeviceProductName,"Wacom (not connected)");
      strcpy(signatureDeviceModel,"Wacom");
   }

   short bSuccess;
   pIGProperties -> LoadFile(&bSuccess);
   if ( ! bSuccess )
      pIGPropertiesClient -> InitNew();

   return;
   }


   SignaturePad::~SignaturePad() {

   ClearScreen();

   GdiplusShutdown(gdiplusToken);

   if ( pIGProperties ) {
      pIGProperties -> Save();
      pIGProperties -> Release();
   }

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);

   hotSpots.clear();

   return;
   }

   void SignaturePad::initializeParameters() {
   long n = offsetof(SignaturePad,endPadParameters) - offsetof(SignaturePad,startPadParameters);
   memset(&startPadParameters,0,n);
   return;
   }


   void SignaturePad::TabletState(long ts) {
   return;
   }


   void SignaturePad::ClearTablet() {

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);

   hotSpots.clear();

   ClearScreen();

   return;
   }


   void SignaturePad::GetRect(RECT *pRect) {
   pRect -> left = 0;
   pRect -> right = Width();
   pRect -> top = 0;
   pRect -> bottom = Height();
   return;
   }


   void SignaturePad::GetHotSpot(long hotSpotNumber,RECT *pRect) {
   pRect -> left = OKXPos;
   pRect -> top = OKYPos;
   pRect -> right = OKXPos + OKXSize;
   pRect -> bottom = OKYPos + OKYSize;
   for ( long k = 0; k < hotSpotNumber; k++ ) {
      pRect -> left -= OKXSize + 8;
      pRect -> right -= OKXSize + 8;
   }
   return;
   }

   long SignaturePad::ClearInk() {
   return E_NOTIMPL;
   }

   void SignaturePad::DisplayOk(long eventId) {

   hotSpot *ph = new hotSpot();

   ph -> rcLocation.left = OKXPos;
   ph -> rcLocation.top = OKYPos;
   ph -> rcLocation.right = OKXPos + OKXSize;
   ph -> rcLocation.bottom = OKYPos + OKYSize;
   ph -> eventId = eventId;

   hotSpots.insert(hotSpots.end(),ph);

   loadBitmapFromFile(OKXPos,OKYPos,OKXSize,OKYSize,OKXSize,OKYSize,szOKFile);
   sendBitmapHandle(hBitmapPad);

   return;
   }


   void SignaturePad::DisplayClear(long eventId) {

   hotSpot *ph = new hotSpot();

   ph -> rcLocation.left = ClearXPos;
   ph -> rcLocation.top = ClearYPos;
   ph -> rcLocation.right = ClearXPos + ClearXSize;
   ph -> rcLocation.bottom = ClearYPos + ClearYSize;
   ph -> eventId = eventId;

   hotSpots.insert(hotSpots.end(),ph);

   loadBitmapFromFile(ClearXPos,ClearYPos,ClearXSize,ClearYSize,ClearXSize,ClearYSize,szClearFile);
   sendBitmapHandle(hBitmapPad);

   return;
   }


   void SignaturePad::DisplayCancel(long eventId) {

   hotSpot *ph = new hotSpot();

   ph -> rcLocation.left = ClearXPos;
   ph -> rcLocation.top = ClearYPos;
   ph -> rcLocation.right = ClearXPos + ClearXSize;
   ph -> rcLocation.bottom = ClearYPos + ClearYSize;
   ph -> eventId = eventId;

   hotSpots.insert(hotSpots.end(),ph);

   loadBitmapFromFile(ClearXPos,ClearYPos,ClearXSize,ClearYSize,ClearXSize,ClearYSize,szClearFile);
   sendBitmapHandle(hBitmapPad);

   return;
   }


   void SignaturePad::DisplayHotSpotHandle(UINT_PTR hBitmap,HDC hdc,long eventId,long x,long y,long cx,long cy) {

   hotSpot *ph = new hotSpot();

   ph -> rcLocation.left = x;
   ph -> rcLocation.top = y;
   ph -> rcLocation.right = x + cx;
   ph -> rcLocation.bottom = y + cy;
   ph -> eventId = eventId;

   hotSpots.insert(hotSpots.end(),ph);

   return;
   }


   void SignaturePad::DisplayHotSpotFile(char *pszFile,long eventId,long x,long y,long cx,long cy) {

   hotSpot *ph = new hotSpot();

   ph -> rcLocation.left = x;
   ph -> rcLocation.top = y;
   ph -> rcLocation.right = x + cx;
   ph -> rcLocation.bottom = y + cy;
   ph -> eventId = eventId;

   hotSpots.insert(hotSpots.end(),ph);

   FILE *fX = fopen(pszFile,"rb");
   char szBitmapFile[MAX_PATH];
   strcpy(szBitmapFile,pszFile);
   if ( fX ) {
      fclose(fX);
   } else {
      char *p = strrchr(szBitmapFile,'\\');
      if ( ! p )
         p = strrchr(szBitmapFile,'/');
      if ( p ) {
         char szTemp[MAX_PATH];
         sprintf(szTemp,"%s\\%s",szUserDirectory,p + 1);
         strcpy(szBitmapFile,szTemp);
      }         
   }

   long cxMargin = (OKHotSpotXSize - OKXSize) / 2;
   long cyMargin = (OKHotSpotYSize - OKYSize) / 2;

   return;
   }


   void SignaturePad::SetSigWindow() {
//FIXME
//   pIDSigPlus -> SetSigWindow(1,SigXPos,SigYPos,SigXSize,SigYSize);
   return;
   }


   void SignaturePad::SetInkArea() {
   setHandwritingArea(InkXPos,InkYPos,InkXSize,InkYSize);
   return;
   }


   long SignaturePad::GetPixelBits(HDC hdc) {
   return displayBitsPerPixel;
   }


   void SignaturePad::PrepareForPage(RECT *) {
   ClearScreen();
   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);
   hotSpots.clear();
   clearImageBits(0,0,padBitmapPixelWidth,padBitmapPixelHeight);
   return;
   }


   void SignaturePad::SetBackgroundFile(BSTR bstrFile) {
   return;
   }


   BOOL SignaturePad::EnableTablet(void) {
   return TRUE;
   }


   BOOL SignaturePad::DisableTablet() {
   return TRUE;
   }

   long SignaturePad::MaximumX() {
   return thePad.lcd_width;
   }

   long SignaturePad::MaximumY() {
   return thePad.lcd_height;
   }

   long SignaturePad::MinimumX() {
   return 0;
   }

   long SignaturePad::MinimumY() {
   return 0;
   }

   long SignaturePad::MaximumSignableY() {
   return MaximumY() * (long)(1.0 - (double)OKYSize / (double)thePad.lcd_height);
   }

   long SignaturePad::Width() {
   return thePad.lcd_width;
   }

   long SignaturePad::Height() {
   return thePad.lcd_height;
   }

   long SignaturePad::LCDWidth() {
   return Width();
   }

   long SignaturePad::LCDHeight() {
   return Height();
   }

   double SignaturePad::PadToLCDScaleX() {
   return scaleToPadX;
   }

   double SignaturePad::PadToLCDScaleY() {
   return scaleToPadY;
   }


   long SignaturePad::FireOption(long optionNumber) {
   connectionPointContainer.fire_OptionSelected(optionNumber);
   return S_OK;
   }