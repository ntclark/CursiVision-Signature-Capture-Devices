 
#include <stddef.h>

#include "SignaturePad.h"

   WNDPROC SignaturePad::defaultSwatchHandler = NULL;
   WNDPROC SignaturePad::defaultTextHandler = NULL;

   SignaturePad::SignaturePad(IUnknown *pUnkOuter) :

      hActivityMonitorThread(NULL),
      signatureActivityStopRequested(false),

      pICursiVisionServices(NULL),

      connectionPointContainer(this),
      connectionPoint(this),

      pIGProperties(NULL),
      pIGPropertiesClient(NULL),
      pIGPropertyPageClient(NULL),

      hwndSettings(NULL),
      hwndFrameWindow(NULL),

      padWidthInInches(4.25),

      signatureTimeOutPeriod(0.0),

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

      pDevice(NULL),
      lcdWidth(0L),
      lcdHeight(0L),

      modelId(ST_1500),

      refCount(0)

   {

   memset(signatureDeviceName,0,sizeof(signatureDeviceName));
   memset(signatureDeviceModel,0,sizeof(signatureDeviceModel));
   memset(signatureDeviceProductName,0,sizeof(signatureDeviceProductName));
   memset(deviceDescription,0,sizeof(deviceDescription));
   memset(modelType,0,sizeof(modelType));

   memset(&originPoint,0,sizeof(POINTL));
   memset(&baselinePoint,0,sizeof(POINTL));

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

   strcpy(signatureDeviceName,"ST-1500");
   strcpy(signatureDeviceProductName,"Scriptel ST-1500 LCD");
   strcpy(signatureDeviceModel,"(ST-15000)");

   short bSuccess;
   pIGProperties -> LoadFile(&bSuccess);
   if ( ! bSuccess )
      pIGPropertiesClient -> InitNew();

   initializeParameters();

   modelId = ST_1500;

   SetInkXSize(Width());
   SetInkYSize(Height());

   SetSigXSize(Width());
   SetSigYSize(Height());

   SetSigXPos(0);
   SetSigYPos(0);

   SetInkXPos(0);
   SetInkYPos(0);

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
   return E_NOTIMPL;
   }

   void SignaturePad::DisplayOk(long eventId) {
   return;
   }


   void SignaturePad::DisplayClear(long eventId) {
   return;
   }


   void SignaturePad::DisplayCancel(long eventId) {
   return;
   }


   void SignaturePad::DisplayHotSpotHandle(UINT_PTR hBitmap,HDC hdc,long eventId,long x,long y,long cx,long cy) {
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
   return 0;
   }


   void SignaturePad::PrepareForPage(RECT *) {
   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);
   hotSpots.clear();
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
   return lcdWidth;
   }

   long SignaturePad::MaximumY() {
   return lcdHeight;
   }

   long SignaturePad::MinimumX() {
   return 0;
   }

   long SignaturePad::MinimumY() {
   return 0;
   }

   long SignaturePad::MaximumSignableY() {
   return MaximumY();
   }

   long SignaturePad::Width() {
   return lcdWidth;
   }

   long SignaturePad::Height() {
   return lcdHeight;
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