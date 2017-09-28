
#include "SignaturePad.h"

#include <commctrl.h>

   long SignaturePad::loaded(char *pszDeviceName,HWND hwndClientHost,void *pvICursiVisionServices) {

   pICursiVisionServices = reinterpret_cast<ICursiVisionServices *>(pvICursiVisionServices);

   if ( isLoaded )
      return S_OK;

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);
 
   hotSpots.clear();

   isActive = true;

   signaturePadCount = STDeviceGetCount();

   if ( 0 < signaturePadCount ) 

     STDeviceOpen(0,TRUE);

   else {

      DWORD commPortNumber = 0;
      DWORD cb = 0L;
      DWORD regType = REG_DWORD;
      HKEY rootKeyHandle,keyHandle;
      DWORD disposition;

      long rc = RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\InnoVisioNate",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&rootKeyHandle,&disposition);

      rc = RegCreateKeyEx(rootKeyHandle,"signotec",0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&keyHandle,&disposition);

      cb = sizeof(DWORD);
      rc = RegQueryValueEx(keyHandle,"port#",0,&regType,(BYTE *)&commPortNumber,&cb);

      if ( 0 > commPortNumber )
         commPortNumber = 0;

      DWORD initialPortNumber = commPortNumber;

      if ( 0 < commPortNumber ) 
         commPortNumber--;

      while ( 0 == signaturePadCount ) {
         commPortNumber++;
         BSTR commPort = SysAllocStringLen(NULL,32);
         swprintf(commPort,L"%ld",commPortNumber);
         STDeviceSetComPort(commPort);
         SysFreeString(commPort);
         signaturePadCount = STDeviceGetCount();
         if ( commPortNumber > 256 ) {
            commPortNumber = -1L;
            break;
         }
      }

      if ( -1L == commPortNumber ) {
         for ( long k = initialPortNumber; k; k-- ) {
            commPortNumber = k;
            BSTR commPort = SysAllocStringLen(NULL,32);
            swprintf(commPort,L"%ld",commPortNumber);
            STDeviceSetComPort(commPort);
            SysFreeString(commPort);
            signaturePadCount = STDeviceGetCount();
            if ( signaturePadCount )
               break;
         }
      }

      if ( 0 < signaturePadCount ) {
         STDeviceOpen(0,TRUE);
         cb = sizeof(DWORD);
         rc = RegSetValueEx(keyHandle,"port#",0,REG_DWORD,(BYTE *)&commPortNumber,cb);
      }

      RegCloseKey(keyHandle);
      RegCloseKey(rootKeyHandle);

   }

   if ( 0 == signaturePadCount ) {
      tabletComTest = VARIANT_FALSE;
      return E_FAIL;
   }

   scaleToPadX = (double)LCDWidth() / (double)Width();
   scaleToPadY = (double)LCDHeight() / (double)Height();

   if ( 320 == LCDWidth() ) {

      modelId = MODEL_ID_SIGMA;

      OKXSize = 48;
      OKYSize = 24;

      ClearXSize = 84;
      ClearYSize = 24;

      sprintf(szOKFile,"%s\\OKSigma.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearSigma.bmp",szApplicationDataDirectory);

      padBitsPerPixel = 24;

   } else if ( 640 == LCDWidth() ) {

      modelId = MODEL_ID_OMEGA;

      OKXSize = 76;
      OKYSize = 40;

      ClearXSize = 104;
      ClearYSize = 40;

      sprintf(szOKFile,"%s\\OKOmega.bmp",szApplicationDataDirectory);
      sprintf(szClearFile,"%s\\ClearOmega.bmp",szApplicationDataDirectory);

      padBitsPerPixel = 24L;

   }

   OKXPos = LCDWidth() - OKXSize - 2;
   OKYPos = 0;

   InkXSize = Width();
   InkYSize = Height();

   SigXSize = Width();
   SigYSize = Height();

   SigXPos = 0;
   SigYPos = 0;

   InkXPos = 0;
   InkYPos = 0;

   OKHotSpotXPos = OKXPos - 8;
   OKHotSpotXSize = OKXSize + 16;

   OKHotSpotYPos = OKYPos - 8;
   OKHotSpotYSize = OKYSize + 16;

   ClearHotSpotXPos = ClearXPos - 8;
   ClearHotSpotXSize = ClearXSize + 16;

   ClearHotSpotYPos = ClearYPos - 8;
   ClearHotSpotYSize = ClearYSize + 16;

   maximumPenUnitsWidth = (long)((double)Width() / scaleToPadX);
   maximumPenUnitsHeight = (long)((double)Height() / scaleToPadY);

   LCDRefresh(0,0,0,0,0);

   eventsAllowed = true;

   if ( gdiplusToken )
      GdiplusShutdown(gdiplusToken);

   GdiplusStartup(&gdiplusToken,&gdiplusStartupInput,NULL);
   
   if ( 0 == countEncoders ) {

      UINT arraySize;

      GetImageEncodersSize(&countEncoders,&arraySize);
   
      pEncoders = (ImageCodecInfo *)new BYTE[arraySize];
   
      GetImageEncoders(countEncoders,arraySize,pEncoders);
      
      for ( long k = 0; k < countEncoders; k++ ) {
#if 0
         if ( preferredImageMimeType )  {
            if ( 0 == wcsicmp(pEncoders[k].MimeType,preferredImageMimeType) ) 
               pTheEncoder = &pEncoders[k];
#if 0
            if ( 0 == wcsicmp(preferredImageMimeType,L"image/jpeg") ) {
            Bitmap *fms = new Bitmap(1,1);
            UINT encoderParameterSize = fms -> GetEncoderParameterListSize(&pTheEncoder -> Clsid);
            if ( encoderParameterSize ) {
               pEncoderParameters = (EncoderParameters *)new BYTE[encoderParameterSize];
               fms -> GetEncoderParameterList(&pTheEncoder -> Clsid,encoderParameterSize,pEncoderParameters);
               for ( long j = 0; j < pEncoderParameters -> Count; j++ ) {
                  if ( EncoderQuality /*EncoderColorDepth*/ == pEncoderParameters -> Parameter[j].Guid ) {
                     encoderQuality = 50L;
                     //pEncoderParameters -> Parameter[j].Value = &encoderQuality;
                  }
               }
            }
            delete fms;
            }
#endif
         } else {
#endif
         if ( 0 == wcsicmp(pEncoders[k].MimeType,L"image/bmp") ) 
            pTheEncoder = &pEncoders[k];
//         }
   
         wcscpy(encoderMimeTypes[k],pEncoders[k].MimeType);

      }

   }

   return S_OK;
   }


   long SignaturePad::unload() {

   if ( ! isLoaded )
      return 0L;

   LCDRefresh(0,0,0,0,0);

   ClearTablet();

   isLoaded = false;

   if ( ! isActive )
      return 0;

   Stop();

   HRESULT rc;

   isActive = false;

   if ( signaturePadCount )
      STDeviceClose(0);

   if ( gdiplusToken )
      GdiplusShutdown(gdiplusToken);

   return S_OK;
   }
