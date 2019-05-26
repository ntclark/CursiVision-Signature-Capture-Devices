 
#include <stddef.h>

#include "SignaturePad.h"

   WNDPROC SignaturePad::defaultSwatchHandler = NULL;
   WNDPROC SignaturePad::defaultTextHandler = NULL;

   SignaturePad::SignaturePad(IUnknown *pUnkOuter) :

      pICursiVisionServices(NULL),

      connectionPointContainer(this),
      connectionPoint(this),

      pIGProperties(NULL),
      pIGPropertiesClient(NULL),
      pIGPropertyPageClient(NULL),

      hwndSettings(NULL),
      hwndFrameWindow(NULL),

      signaturePadCount(0L),
      tabletComTest(VARIANT_FALSE),

      padWidthInInches(4.25),

      signatureTimeOutPeriod(0.0),

      penWidth(1L),
      inkColor(RGB(0,0,0)),

      scaleToPadX(1.0),
      scaleToPadY(1.0),
      startPointX(0L),
      startPointY(0L),
      endPointX(0L),
      endPointY(0L),

      maximumPenUnitsWidth(0),
      maximumPenUnitsHeight(0),

      pIConnectionPointContainer(NULL),
      pIConnectionPoint(NULL),
      dwConnectionCookie(0L),

      isActive(false),
      isLoaded(false),
      penIsDown(false),

      modelId(0),

      OKXPos(0),OKYPos(0),OKXSize(0),OKYSize(0),
      ClearXPos(0),ClearYPos(0),ClearXSize(0),ClearYSize(0),
      OKHotSpotXPos(0),OKHotSpotYPos(0),OKHotSpotXSize(0),OKHotSpotYSize(0),
      ClearHotSpotXPos(0),ClearHotSpotYPos(0),ClearHotSpotXSize(0),ClearHotSpotYSize(0),

      padBitsPerPixel(0L),

      gdiplusStartupInput(NULL),
      gdiplusToken(0),

      refCount(0)

   {

   memset(signatureDeviceName,0,sizeof(signatureDeviceName));
   memset(signatureDeviceModel,0,sizeof(signatureDeviceModel));
   memset(signatureDeviceProductName,0,sizeof(signatureDeviceProductName));
   memset(deviceDescription,0,sizeof(deviceDescription));
   memset(modelType,0,sizeof(modelType));

   memset(szOKFile,0,sizeof(szOKFile));
   memset(szClearFile,0,sizeof(szClearFile));

   memset(&originPoint,0,sizeof(POINTL));
   memset(&baselinePoint,0,sizeof(POINTL));

   strcpy(signatureDeviceName,"signotec");
   strcpy(signatureDeviceProductName,"signotec");
   strcpy(signatureDeviceModel,"(signotec)");

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

   if ( szApplicationDataDirectory[0] )
      pIGProperties -> put_FileName(bstrFileName);
   else
      pIGProperties -> put_FileName(L"");

   SysFreeString(bstrFileName);

   pIGProperties -> Add(L"pad width",NULL);
   pIGProperties -> DirectAccess(L"pad width",TYPE_DOUBLE,&padWidthInInches,sizeof(RECT));

   long n = offsetof(SignaturePad,endPadParameters) - offsetof(SignaturePad,startPadParameters);

   pIGProperties -> Add(L"pad parameters",NULL);
   pIGProperties -> DirectAccess(L"pad parameters",TYPE_BINARY,&startPadParameters,n);

   pIGProperties -> Add(L"Timeout period",NULL);
   pIGProperties -> DirectAccess(L"Timeout period",TYPE_BINARY,&signatureTimeOutPeriod,sizeof(double));

   pIGProperties -> Add(L"pen width",NULL);
   pIGProperties -> DirectAccess(L"pen width",TYPE_BINARY,&penWidth,sizeof(penWidth));

   pIGProperties -> Add(L"origin point",NULL);
   pIGProperties -> DirectAccess(L"origin point",TYPE_BINARY,&originPoint,sizeof(POINTL));

   pIGProperties -> Add(L"ink color",NULL);
   pIGProperties -> DirectAccess(L"ink color",TYPE_BINARY,&inkColor,sizeof(inkColor));

   initializeParameters();

   short bSuccess;
   pIGProperties -> LoadFile(&bSuccess);
   if ( ! bSuccess )
      pIGPropertiesClient -> InitNew();

   return;
   }


   SignaturePad::~SignaturePad() {

   unload();

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

   LCDRefresh(0,0,0,0,0);

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
   memset(pRect,0,sizeof(RECT));
   return;
   }

   long SignaturePad::ClearInk() {
   STSignatureRetry();
   return S_OK;
   }

   void SignaturePad::DisplayOk(long eventId) {

   hotSpot *ph = new hotSpot();

   ph -> rcLocation.left = OKXPos;
   ph -> rcLocation.top = OKYPos;
   ph -> rcLocation.right = OKXPos + OKXSize;
   ph -> rcLocation.bottom = OKYPos + OKYSize;
   ph -> eventId = eventId;

   hotSpots.insert(hotSpots.end(),ph);

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);
   MultiByteToWideChar(CP_ACP,0,szOKFile,-1,bstrFile,MAX_PATH);

   STDisplaySetImageFromFile(OKXPos,OKYPos,bstrFile);

   SysFreeString(bstrFile);

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

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);
   MultiByteToWideChar(CP_ACP,0,szClearFile,-1,bstrFile,MAX_PATH);
   STDisplaySetImageFromFile(ClearXPos,ClearYPos,bstrFile);
   SysFreeString(bstrFile);

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

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);
   MultiByteToWideChar(CP_ACP,0,szClearFile,-1,bstrFile,MAX_PATH);
   STDisplaySetImageFromFile(ClearXPos,ClearYPos,bstrFile);
   SysFreeString(bstrFile);

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
   return;
   }


   void SignaturePad::SetSigWindow() {
   return;
   }


   void SignaturePad::SetInkArea() {
   return;
   }


   long SignaturePad::GetPixelBits(HDC hdc) {
   return padBitsPerPixel;
   }


   void SignaturePad::PrepareForPage(RECT *) {
   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);
   hotSpots.clear();
   return;
   }


   void SignaturePad::SetBackgroundFile(BSTR bstrFile) {
   STDisplaySetImageFromFile(0,0,bstrFile);
   return;
   }


   BOOL SignaturePad::EnableTablet(void) {
   return TRUE;
   }


   BOOL SignaturePad::DisableTablet() {
   return TRUE;
   }

   long SignaturePad::MaximumX() {
   return endPointX - startPointX;
   }

   long SignaturePad::MaximumY() {
   return endPointY - startPointY;
   }

   long SignaturePad::MinimumX() {
   return 0;
   }

   long SignaturePad::MinimumY() {
   return 0;
   }

   long SignaturePad::MaximumSignableY() {
   return endPointY - startPointY;
   }

   HRESULT SignaturePad::LCDWidth() {
   return STDisplayGetWidth();
   }

   HRESULT SignaturePad::LCDHeight() {
   return STDisplayGetHeight();
   }

   HRESULT SignaturePad::Width() {
   if ( 0 == signaturePadCount )
      return 0L;
   if ( 0 == endPointX ) {
      startPointX = 0;
      long k = STSignatureScaleToDisplay(startPointX);
      while ( 0 == k ) {
         startPointX++;
         k = STSignatureScaleToDisplay(startPointX);
      }
      long cx = LCDWidth();
      endPointX = startPointX;
      k = STSignatureScaleToDisplay(endPointX);
      while ( k < cx ) {
         endPointX++;
         k = STSignatureScaleToDisplay(endPointX);
      }
   }
   return endPointX - startPointX;
   }

   HRESULT SignaturePad::Height() {
   if ( 0 == signaturePadCount )
      return 0L;
   if ( 0 == endPointY ) {
      startPointY = 0;
      long k = STSignatureScaleToDisplay(startPointY);
      while ( 0 == k ) {
         startPointY++;
         k = STSignatureScaleToDisplay(startPointY);
      }
      long cy = LCDHeight();
      endPointY = startPointY;
      k = STSignatureScaleToDisplay(endPointY);
      while ( k < cy ) {
         endPointY++;
         k = STSignatureScaleToDisplay(endPointY);
      }
   }
   return endPointY - startPointY;
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