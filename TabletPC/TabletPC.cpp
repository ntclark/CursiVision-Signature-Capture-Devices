 
#include <stddef.h>

#include "TabletPC.h"

   char TabletPC::szBadConfigurationOption[] = {"none"};
   WNDPROC TabletPC::defaultSwatchHandler = NULL;
   WNDPROC TabletPC::defaultTextHandler = NULL;

   HBITMAP ConvertBitmap(HDC hdcSource,HBITMAP hBitmap,long bitCount);

   TabletPC::TabletPC(IUnknown *pUnkOuter) :

      connectionPointContainer(this),
      connectionPoint(this),

      pSignatureDataX(NULL),
      pSignatureDataY(NULL),

      pICursiVisionServices(NULL),

      pIGProperties(NULL),
      pIGPropertiesClient(NULL),
      pIGPropertyPageClient(NULL),

      pIOleClientSite_MySite(NULL),
      pIOleInPlaceSite_MySite(NULL),

      hwndSigPlusFrame(NULL),
      hwndSigPlusControlHost(NULL),
      hwndSettings(NULL),
      hwndBitmapDepth(NULL),

      hwndClient(NULL),
      hwndFrameWindow(NULL),
      hdcClient(NULL),

      padWidthInInches(0.0),
      signatureTimeOutPeriod(2.0),

      penWidth(2L),
      inkColor(RGB(0,0,0)),

      scaleToPadX(0.0),
      scaleToPadY(0.0),
      padWidthTopazUnits(0.0),
      padHeightTopazUnits(0.0),

      pIConnectionPointContainer(NULL),
      pIConnectionPoint(NULL),
      dwConnectionCookie(0L),

      pIOleInPlaceFrame(NULL),
      pIOleInPlaceSite(NULL),
      pIOleClientSite(NULL),

      pIInkCollector(NULL),
      pIInkRenderer(NULL),
      pIInkCollectorEvents(NULL),

      pIOleObject(NULL),

      refCount(0)

   {

   memset(&rectDisplay,0,sizeof(RECT));
   memset(signatureDeviceName,0,sizeof(signatureDeviceName));
   memset(signatureDeviceModel,0,sizeof(signatureDeviceModel));
   memset(signatureDeviceProductName,0,sizeof(signatureDeviceProductName));
   memset(modelType,0,sizeof(modelType));
   memset(szConfigurationFile,0,sizeof(szConfigurationFile));
   memset(deviceDescription,0,sizeof(deviceDescription));

   memset(&rcClient,0,sizeof(RECT));

   pIOleObject = new _IOleObject(this);

   initializeParameters();

   strcpy(signatureDeviceName,"TabletPC");
   strcpy(signatureDeviceProductName,"Windows Tablet PC OS");

   modelId = 1;
   padWidthInInches = 8.5;
   padHeightInInches = 11.0;

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

   long n = offsetof(TabletPC,endPadParameters) - offsetof(TabletPC,startPadParameters);

   pIGProperties -> Add(L"pad parameters",NULL);
   pIGProperties -> DirectAccess(L"pad parameters",TYPE_BINARY,&startPadParameters,n);

   pIGProperties -> Add(L"Timeout period",NULL);
   pIGProperties -> DirectAccess(L"Timeout period",TYPE_BINARY,&signatureTimeOutPeriod,sizeof(double));

   pIGProperties -> Add(L"pen width",NULL);
   pIGProperties -> DirectAccess(L"pen width",TYPE_BINARY,&penWidth,sizeof(penWidth));

   pIGProperties -> Add(L"ink color",NULL);
   pIGProperties -> DirectAccess(L"ink color",TYPE_BINARY,&inkColor,sizeof(inkColor));

   short bSuccess;
   pIGProperties -> LoadFile(&bSuccess);
   if ( ! bSuccess )
      pIGPropertiesClient -> InitNew();
   
   return;
   }


   TabletPC::~TabletPC() {

   if ( hwndSigPlusFrame )
      DestroyWindow(hwndSigPlusFrame);

   if ( hwndSigPlusControlHost)
      DestroyWindow(hwndSigPlusControlHost);

   if ( pIGProperties ) {
      pIGProperties -> Save();
      pIGProperties -> Release();
   }

   return;
   }

   
   void TabletPC::initializeParameters() {
   long n = offsetof(TabletPC,endPadParameters) - offsetof(TabletPC,startPadParameters);
   memset(&startPadParameters,0,n);
   return;
   }


   long TabletPC::initializeWindows(HWND hwndParent) {

   if ( hwndSigPlusFrame ) {
      SetParent(hwndSigPlusFrame,hwndParent);
      return 0;
   }

   WNDCLASS gClass;
   
   memset(&gClass,0,sizeof(WNDCLASS));
   gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
   gClass.lpfnWndProc = TabletPC::frameHandler;
   gClass.cbClsExtra = 32;
   gClass.cbWndExtra = 32;
   gClass.hInstance = hModule;
   gClass.hIcon = NULL;
   gClass.hCursor = NULL;
   gClass.hbrBackground = 0;
   gClass.lpszMenuName = NULL;
   gClass.lpszClassName = "TabletPC";
  
   RegisterClass(&gClass);

   hwndSigPlusFrame = CreateWindowEx(0L,"TabletPC","",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndParent,NULL/*hMenu*/,NULL,(void *)this);

   hwndSigPlusControlHost = CreateWindowEx(0L,"STATIC","",WS_CHILD | WS_VISIBLE,0,0,0,0,hwndSigPlusFrame,NULL,NULL,NULL);

   return 0;
   }


   void TabletPC::instantiateSigPlus() {
   return;
   }


   void TabletPC::TabletState(long ts) {
   return;
   }


   void TabletPC::ClearTablet() {
   return;
   }


   void TabletPC::GetRect(RECT *pRect) {
   if( ! pRect )
      return;
   GetWindowRect(hwndClient,pRect);
   return;
   }


   void TabletPC::GetHotSpot(long hotSpotNumber,RECT *pRect) {
   if ( pRect )
      memset(pRect,0,sizeof(RECT));
   return;
   }

   long TabletPC::ClearInk() {
   return E_NOTIMPL;
   }

   BOOL TabletPC::Configure() {
   return TRUE;
   }


   void TabletPC::SetPadParameters() {
   return;
   }


   void TabletPC::DisplayOk(long eventId) {
   return;
   }


   void TabletPC::DisplayClear(long eventId) {
   return;
   }


   void TabletPC::DisplayCancel(long eventId) {
   return;
   }


   void TabletPC::DisplayHotSpotHandle(HBITMAP hBitmap,HDC hdc,long eventId,long x,long y,long cx,long cy) {
   return;
   }


   void TabletPC::DisplayHotSpotFile(char *pszFile,long eventId,long x,long y,long cx,long cy) {
   return;
   }


   void TabletPC::SetSigWindow() {
   return;
   }


   void TabletPC::SetInkArea() {
   return;
   }


   long TabletPC::GetPixelBits(HDC hdc) {
   return GetDeviceCaps(hdc,BITSPIXEL);
   }


   void TabletPC::DisplaySignatureBitmapHandle(HBITMAP hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   return;
   }


   void TabletPC::OverlaySignatureBitmapHandle(HBITMAP hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   return;
   }


   void TabletPC::DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad,BOOL isLastPage) {
   return;
   }


   void TabletPC::SetBackgroundFile(BSTR bstrFile) {
   return;
   }


   BOOL TabletPC::EnableTablet(void) {
   return FALSE;
   }


   BOOL TabletPC::DisableTablet() {
   return FALSE;
   }


   void TabletPC::SaveBitmapFile(HDC hdcSource,HBITMAP hBitmap,long bitCount,char *pszFileName) {

   BITMAP bitMap;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   bitMap.bmBitsPixel = (short)bitCount;

   long colorCount = bitMap.bmPlanes * bitMap.bmBitsPixel;

   WORD colorTableSize = sizeof(RGBQUAD) * ( 1 << (bitMap.bmPlanes * bitMap.bmBitsPixel ) );

   long fileHeaderSize = sizeof(BITMAPFILEHEADER);
   long infoHeaderSize = sizeof(BITMAPINFOHEADER);
   long desiredSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   long entireSize = fileHeaderSize + infoHeaderSize + colorTableSize + desiredSizeImage;

   if ( 24 == bitCount )
      entireSize -= colorTableSize;

   BYTE *pBuffer = new BYTE[entireSize];

   memset(pBuffer,0,entireSize * sizeof(BYTE));

   BITMAPFILEHEADER *pBitmapFileHeader = (BITMAPFILEHEADER *)pBuffer;

   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)(pBuffer + sizeof(BITMAPFILEHEADER));

   BITMAPINFOHEADER *pBitmapInfoHeader = &(pBitmapInfo -> bmiHeader);
   
   pBitmapInfoHeader -> biSize = sizeof(BITMAPINFOHEADER); 
   pBitmapInfoHeader -> biWidth = bitMap.bmWidth;
   pBitmapInfoHeader -> biHeight = bitMap.bmHeight;
   pBitmapInfoHeader -> biPlanes = bitMap.bmPlanes; 
   pBitmapInfoHeader -> biBitCount = bitMap.bmBitsPixel;
   pBitmapInfoHeader -> biCompression = BI_RGB;
   pBitmapInfoHeader -> biClrUsed = 0;
   pBitmapInfoHeader -> biSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   pBitmapInfoHeader -> biClrImportant = 0;

   BYTE *pBits = (BYTE *)pBitmapFileHeader + fileHeaderSize + infoHeaderSize + colorTableSize;

   memset(pBits,0,pBitmapInfoHeader -> biSizeImage);

   long k = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,pBitmapInfo,DIB_RGB_COLORS);

   pBitmapFileHeader -> bfType = 0x4d42;

   pBitmapFileHeader -> bfSize = entireSize;

   pBitmapFileHeader -> bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize;

   sprintf(pszFileName,"%s.bmp",_tempnam(szApplicationDataDirectory,NULL));

   FILE *fBitmap = fopen(pszFileName,"wb");

   fwrite(pBitmapFileHeader,entireSize,1,fBitmap);

   fclose(fBitmap);

   delete [] pBuffer;

   return;
   }


   void TabletPC::SaveJPEGFile(HDC hdcSource,HBITMAP hBitmap,long bitCount,char *pszFileName) {

   BITMAP bitMap;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   bitMap.bmBitsPixel = (short)bitCount;

   long colorCount = bitMap.bmPlanes * bitMap.bmBitsPixel;

   WORD colorTableSize = sizeof(RGBQUAD) * ( 1 << (bitMap.bmPlanes * bitMap.bmBitsPixel ) );

   long fileHeaderSize = sizeof(BITMAPFILEHEADER);
   long infoHeaderSize = sizeof(BITMAPV5HEADER);
   long desiredSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   long entireSize = fileHeaderSize + infoHeaderSize + colorTableSize + desiredSizeImage;

   if ( 24 == bitCount )
      entireSize -= colorTableSize;

   BYTE *pBuffer = new BYTE[entireSize];

   memset(pBuffer,0,entireSize * sizeof(BYTE));

   BITMAPFILEHEADER *pBitmapFileHeader = (BITMAPFILEHEADER *)pBuffer;

   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)(pBuffer + sizeof(BITMAPFILEHEADER));

   BITMAPV5HEADER *pBitmapInfoHeader = (BITMAPV5HEADER *)&(pBitmapInfo -> bmiHeader);
   
   pBitmapInfoHeader -> bV5Size = sizeof(BITMAPV5HEADER); 
   pBitmapInfoHeader -> bV5Width = bitMap.bmWidth;
   pBitmapInfoHeader -> bV5Height = bitMap.bmHeight;
   pBitmapInfoHeader -> bV5Planes = bitMap.bmPlanes; 
   pBitmapInfoHeader -> bV5BitCount = bitMap.bmBitsPixel;
   pBitmapInfoHeader -> bV5Compression = BI_JPEG;
   pBitmapInfoHeader -> bV5ClrUsed = 0;
   pBitmapInfoHeader -> bV5SizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   pBitmapInfoHeader -> bV5ClrImportant = 0;

   BYTE *pBits = (BYTE *)pBitmapFileHeader + fileHeaderSize + infoHeaderSize + colorTableSize;

   memset(pBits,0,pBitmapInfoHeader -> bV5SizeImage);

   long k = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,NULL,pBitmapInfo,DIB_RGB_COLORS);
   k = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,pBitmapInfo,DIB_RGB_COLORS);

   pBitmapFileHeader -> bfType = 0x4d42;

   pBitmapFileHeader -> bfSize = entireSize;

   pBitmapFileHeader -> bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV5HEADER) + colorTableSize;

   sprintf(pszFileName,"%s.jpg",_tempnam(szApplicationDataDirectory,NULL));

   FILE *fBitmap = fopen(pszFileName,"wb");

   fwrite(pBitmapFileHeader,entireSize,1,fBitmap);

   fclose(fBitmap);

   delete [] pBuffer;

   return;
   }

   HBITMAP ConvertBitmap(HDC hdcSource,HBITMAP hBitmap,long bitCount) {

   BITMAP bitMap;
   BITMAPINFO bitMapInfo;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   memset(&bitMapInfo,0,sizeof(bitMapInfo));

   bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitMapInfo.bmiHeader.biWidth = bitMap.bmWidth;
   bitMapInfo.bmiHeader.biHeight = bitMap.bmHeight;
   bitMapInfo.bmiHeader.biPlanes = bitMap.bmPlanes;
   bitMapInfo.bmiHeader.biBitCount = (unsigned short)bitCount;
   bitMapInfo.bmiHeader.biCompression= BI_RGB;

   BYTE *pBits = NULL;

   HBITMAP hNewBitmap = CreateDIBSection(hdcSource,&bitMapInfo,DIB_RGB_COLORS,reinterpret_cast<void **>(&pBits),NULL,0L);

   long rc = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,&bitMapInfo,DIB_RGB_COLORS);

   return hNewBitmap;
   }


   long TabletPC::WriteParameters(char *pszFileName) {
   return 0;
   }