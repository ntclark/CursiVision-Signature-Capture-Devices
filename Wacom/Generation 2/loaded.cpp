
#include "SignaturePad.h"

   long SignaturePad::loaded(char *pszDeviceName,HWND hwndClientHost,void *pvICursiVisionServices) {

   initializeParameters();

   if ( ! pTablet ) {

      std::vector<UsbDevice> theDevices = WacomGSS::STU::getUsbDevices();

      if ( theDevices.empty() ) {

         WCHAR szwMessage[1024];
         swprintf(szwMessage,L"There is no Wacom signature pad connected.\n\nPlease connect a pad or choose a different signature pad in CursiVision settings.");
         MessageBox(NULL,szwMessage,L"Error",MB_ICONEXCLAMATION);

         strcpy(signatureDeviceName,"Wacom");
         strcpy(signatureDeviceProductName,"Wacom (not connected)");
         strcpy(signatureDeviceModel,"Wacom");

      } else {

         pUsbInterface = std::make_unique<WacomGSS::STU::UsbInterface>();

         pUsbInterface -> connect(theDevices[0],true);

         pTablet = new WacomGSS::STU::Tablet();

         pTablet -> attach(std::move(pUsbInterface));

         pTabletCapability = pTablet -> getCapability();

         pTabletInformation = pTablet -> getInformation();

      }
   }

   if ( pszDeviceName )
      strcpy(signatureDeviceName,pszDeviceName);

   if ( 0 == strcmp(pTabletInformation.modelNameNullTerminated,"STU-300") ) {//396 == pCapability.screenWidth ) {

      modelId = STU_300;

      strcpy(signatureDeviceName,pTabletInformation.modelNameNullTerminated);
      strcpy(signatureDeviceProductName,"Wacom STU-300 LCD");
      strcpy(signatureDeviceModel,signatureDeviceName);
      modelHasScaling = true;
      padWidthInInches = 4.25;

      baselinePoint.y = 80;

      SetOKXSize(48);
      SetOKYSize(24);

      SetClearXSize(84);
      SetClearYSize(24);

      sprintf(szOKFile,"%s\\OKSTU300.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU300.bmp",szApplicationDataDirectory);

      displayBitsPerPixel = 1;

   } else if ( 800 == pTabletCapability.screenWidth ) {

      modelId = STU_520;

      strcpy(signatureDeviceName,pTabletInformation.modelNameNullTerminated);
      strcpy(signatureDeviceProductName,"Wacom STU-520 LCD");
      strcpy(signatureDeviceModel,signatureDeviceName);
      modelHasScaling = true;
      padWidthInInches = 7.0;

      baselinePoint.y = 0;

      SetOKXSize(76);
      SetOKYSize(40);

      SetClearXSize(104);
      SetClearYSize(40);

      sprintf(szOKFile,"%s\\OKSTU520.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU520.bmp",szApplicationDataDirectory);

      displayBitsPerPixel = 16;

   } else if ( 640 == pTabletCapability.screenWidth ) {

      modelId = STU_500;

      strcpy(signatureDeviceName,pTabletInformation.modelNameNullTerminated);
      strcpy(signatureDeviceProductName,"Wacom STU-500 LCD");
      strcpy(signatureDeviceModel,signatureDeviceName);
      modelHasScaling = true;
      padWidthInInches = 7.0;

      baselinePoint.y = 0;

      SetOKXSize(76);
      SetOKYSize(40);

      SetClearXSize(104);
      SetClearYSize(40);

      sprintf(szOKFile,"%s\\OKSTU520.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU520.bmp",szApplicationDataDirectory);

      displayBitsPerPixel = 1;

   } else if ( pTablet ) {

      modelId = STU_300;

      WCHAR szwMessage[1024];
      swprintf(szwMessage,L"There is an unrecognized Wacom signature pad connected.\n\nKnown pads include the 300, 500, and 520 pads.\nCharacteristics of the 300 will be used.");
      MessageBox(NULL,szwMessage,L"Error",MB_ICONEXCLAMATION);

      strcpy(signatureDeviceName,pTabletInformation.modelNameNullTerminated);
      strcpy(signatureDeviceProductName,"No Pad Connected");
      strcpy(signatureDeviceModel,signatureDeviceName);
      modelHasScaling = true;
      padWidthInInches = 4.25;

      baselinePoint.y = 80;

      SetOKXSize(48);
      SetOKYSize(24);

      SetClearXSize(84);
      SetClearYSize(24);

      sprintf(szOKFile,"%s\\OKSTU300.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU300.bmp",szApplicationDataDirectory);

      displayBitsPerPixel = 1;

   }

   pICursiVisionServices = reinterpret_cast<ICursiVisionServices *>(pvICursiVisionServices);

   SetOKXPos(pTabletCapability.screenWidth - OKXSize - 2);
   SetOKYPos(0);

   SetInkXSize(pTabletCapability.screenWidth);
   SetInkYSize(pTabletCapability.screenHeight);

   SetSigXSize(pTabletCapability.screenWidth);
   SetSigYSize(pTabletCapability.screenHeight);

   SetSigXPos(0);
   SetSigYPos(0);

   SetOKHotSpotXPos(OKXPos - 8);
   SetOKHotSpotXSize(OKXSize + 16);

   SetOKHotSpotYPos(OKYPos - 8);
   SetOKHotSpotYSize(OKYSize + 16);

   SetClearHotSpotXPos(ClearXPos - 8);
   SetClearHotSpotXSize(ClearXSize + 16);

   SetClearHotSpotYPos(ClearYPos - 8);
   SetClearHotSpotYSize(ClearYSize + 16);

   SetInkXPos(0);
   SetInkYPos(0);

   isLoaded = true;

   if ( pTabletCapability.tabletMaxX ) {
      scaleToPadX = (double)pTabletCapability.screenWidth / (double)pTabletCapability.tabletMaxX;
      scaleToPadY = (double)pTabletCapability.screenHeight / (double)pTabletCapability.tabletMaxY;
   }

   maximumPenUnitsWidth = (long)((double)Width() / scaleToPadX);
   maximumPenUnitsHeight = (long)((double)Height() / scaleToPadY);

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);

   hotSpots.clear();

   if ( gdiplusToken )
      GdiplusShutdown(gdiplusToken);

   GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);

   if ( ! hwndClientHost )
      return pTablet ? S_OK : S_FALSE;

   isActive = true;

   if ( hBitmapPad )
      DeleteObject(hBitmapPad);

   padBitmapPixelWidth = LCDWidth();
   padBitmapPixelHeight = LCDHeight();

   padBitmapPixelBytes = padBitmapPixelHeight * ((padBitmapPixelWidth * MONOCHROME_IMAGE_BITS + 31) & ~31) / 8 ;

   if ( IS_COLOR_PAD ) 
      padBitmapPixelBytes = padBitmapPixelHeight * ((padBitmapPixelWidth * PREFERRED_COLOR_IMAGE_BIT_DEPTH + 31) & ~31) / 8 ;

   padBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   padBitmapInfo.bmiHeader.biWidth = padBitmapPixelWidth;
   padBitmapInfo.bmiHeader.biHeight = -padBitmapPixelHeight;
   padBitmapInfo.bmiHeader.biPlanes = 1;
   padBitmapInfo.bmiHeader.biBitCount = IS_COLOR_PAD ? (WORD)PREFERRED_COLOR_IMAGE_BIT_DEPTH : (WORD)MONOCHROME_IMAGE_BITS;
   padBitmapInfo.bmiHeader.biSizeImage = padBitmapPixelHeight * ((padBitmapPixelWidth * padBitmapInfo.bmiHeader.biBitCount + 31) & ~31) / 8 ;
   padBitmapInfo.bmiHeader.biCompression = BI_RGB;

   memcpy(&temporaryBitmapInfo,&padBitmapInfo,sizeof(BITMAPINFO));

   hBitmapPad = createPadBitmap();

   return pTablet ? S_OK : S_FALSE;
   }


   long SignaturePad::unload() {

   ClearTablet();

   if ( ! isLoaded )
      return 0L;

   isLoaded = false;

   if ( ! isActive )
      return 0;

   isActive = false;

   return S_OK;
   }
