
#include "SignaturePad.h"

   WNDPROC SignaturePad::defaultSwatchHandler = NULL;
   WNDPROC SignaturePad::defaultTextHandler = NULL;

   Tablet *SignaturePad::pTablet = NULL;

   std::unique_ptr<UsbInterface> SignaturePad::pUsbInterface;

   Protocol::Capability SignaturePad::pTabletCapability{0};
   Protocol::Information SignaturePad::pTabletInformation{0};
   
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

      hActivityMonitorThread(NULL),
      signatureActivityStopRequested(false),

      refCount(0)

   {

   memset(signatureDeviceName,0,sizeof(signatureDeviceName));
   memset(signatureDeviceModel,0,sizeof(signatureDeviceModel));
   memset(signatureDeviceProductName,0,sizeof(signatureDeviceProductName));
   memset(deviceDescription,0,sizeof(deviceDescription));

   memset(szOKFile,0,sizeof(szOKFile));
   memset(szClearFile,0,sizeof(szClearFile));

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

   if ( p )
      strcpy(szRootName,p + 1);

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

   short bSuccess;
   pIGProperties -> LoadFile(&bSuccess);
   if ( ! bSuccess )
      pIGPropertiesClient -> InitNew();

   return;
   }


   SignaturePad::~SignaturePad() {

   GdiplusShutdown(gdiplusToken);

   if ( pTablet ) {
      Stop();
      pTablet -> setClearScreen();

//
//NTC: 05-26-2019: Deleting the tablet causes the "render thread" in USbInterface to be stopped via
// method stopRenderThread.
// Which totally hangs completely and 100%
// I do not have the desire or time to rewwrite the absolute fucking pile of shit which is the Wacom
// STU API which they have not even remotely attempted to debug.
// It IS the PRIME example of how sloppy unprofessionally written software is not worth the paper it
// might be printed on - absolute fucking crap
//
#if 0
      delete pTablet;
      pTablet = NULL;
#endif
   }

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

   if ( pTablet )
      pTablet -> setClearScreen();

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
//
//NTC: 05-08-2019: This function was specific to the 520 model, don't have the source for it's implementation
//
#if 0
   setHandwritingArea(InkXPos,InkYPos,InkXSize,InkYSize);
#endif
   return;
   }


   long SignaturePad::GetPixelBits(HDC hdc) {
   return displayBitsPerPixel;
   }


   void SignaturePad::PrepareForPage(RECT *) {
   if ( pTablet )
      pTablet -> setClearScreen();
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
   if ( ! pTablet ) 
      return 0;
   return pTabletCapability.screenWidth;
   }

   long SignaturePad::MaximumY() {
   if ( ! pTablet )
      return 0;
   return pTabletCapability.screenHeight;
   }

   long SignaturePad::MinimumX() {
   return 0;
   }

   long SignaturePad::MinimumY() {
   return 0;
   }

   long SignaturePad::MaximumSignableY() {
   return MaximumY() * (long)(1.0 - (double)OKYSize / (double)pTabletCapability.screenHeight);
   }

   long SignaturePad::Width() {
   if ( ! pTablet )
      return 0;
   return pTabletCapability.screenWidth;
   }

   long SignaturePad::Height() {
   if ( ! pTablet )
      return 0;
   return pTabletCapability.screenHeight;
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