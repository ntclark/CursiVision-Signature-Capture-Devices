
#include "SignaturePad.h"

#include <commctrl.h>

   long SignaturePad::loaded(char *pszDeviceName,HWND hwndClientHost,void *pvICursiVisionServices) {

   if ( pszDeviceName )
      strcpy(signatureDeviceName,pszDeviceName);

   pICursiVisionServices = reinterpret_cast<ICursiVisionServices *>(pvICursiVisionServices);

   initializeParameters();

   displayBitsPerPixel = 16;

   if ( 396 == thePad.lcd_width ) {

      modelId = STU_300;

      SetOKXSize(48);
      SetOKYSize(24);

      SetClearXSize(84);
      SetClearYSize(24);

      sprintf(szOKFile,"%s\\OKSTU300.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU300.bmp",szApplicationDataDirectory);

   } else if ( 640 == thePad.lcd_width ) {

      modelId = STU_500;

      SetOKXSize(76);
      SetOKYSize(40);

      SetClearXSize(104);
      SetClearYSize(40);

      sprintf(szOKFile,"%s\\OKSTU520.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU520.bmp",szApplicationDataDirectory);

   } else if ( 800 == thePad.lcd_width ) {

      modelId = STU_520;

      SetOKXSize(76);
      SetOKYSize(40);

      SetClearXSize(104);
      SetClearYSize(40);

      sprintf(szOKFile,"%s\\OKSTU520.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSTU520.bmp",szApplicationDataDirectory);

   }

   SetOKXPos(thePad.lcd_width - OKXSize - 2);
   SetOKYPos(0);

   SetInkXSize(thePad.lcd_width);
   SetInkYSize(thePad.lcd_height);

   SetSigXSize(thePad.lcd_width);
   SetSigYSize(thePad.lcd_height);

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

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);

   hotSpots.clear();

   isLoaded = true;

   scaleToPadX = (double)thePad.lcd_width / (double)thePad.max_x;
   scaleToPadY = (double)thePad.lcd_height / (double)thePad.max_y;

   maximumPenUnitsWidth = (long)((double)Width() / scaleToPadX);
   maximumPenUnitsHeight = (long)((double)Height() / scaleToPadY);

   if ( gdiplusToken )
      GdiplusShutdown(gdiplusToken);

   GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);

   if ( ! hwndClientHost )
      return S_OK;

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

   setInkingMode(1);

   return S_OK;
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
