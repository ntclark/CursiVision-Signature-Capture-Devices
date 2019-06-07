
#include "SignaturePad.h"

   long SignaturePad::loaded(char *pszDeviceName,HWND hwndClientHost,void *pvICursiVisionServices) {

//   if ( pszDeviceName )
//      strcpy(signatureDeviceName,pszDeviceName);

   pICursiVisionServices = reinterpret_cast<ICursiVisionServices *>(pvICursiVisionServices);

   if ( isLoaded )
      return S_OK;

   for ( std::list<hotSpot *>::iterator it = hotSpots.begin(); it != hotSpots.end(); it++ )
      delete (*it);
 
   hotSpots.clear();

   isActive = true;

   scriptel_list_devices(&pDevice);

   if ( NULL == pDevice ) {
      char szMessage[1024];
      LoadString(hModule,IDS_NOT_INSTALLED,szMessage,1024);
      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);
      return S_FALSE;
   }

   strcpy(signatureDeviceName,pDevice -> model);

   HRESULT rc = scriptel_open_device(pDevice);

   if ( ! ( SCRIPTEL_CODE_SUCCESS == rc ) ) {
      char szMessage[1024];
      LoadString(hModule,IDS_OPEN_ERROR,szMessage,1024);
      MessageBox(NULL,szMessage,"Error",MB_ICONEXCLAMATION | MB_TOPMOST);
      return S_FALSE;
   }

   scriptel_hid_feature_coordinate_range theRange = scriptel_get_coordinate_range(pDevice);

   lcdWidth = theRange.x_max;
   if ( 0 == lcdWidth )
      lcdWidth = pDevice -> display_info.width;

   lcdHeight = theRange.y_max;
   if ( 0 == lcdHeight )
      lcdHeight = pDevice -> display_info.height;

   scaleToPadX = (double)Width() / (double)(65535 + 256);
   scaleToPadY = 1.25 * scaleToPadX / 4.4;

   maximumPenUnitsWidth = (long)((double)Width() / scaleToPadX);
   maximumPenUnitsHeight = (long)((double)Height() / scaleToPadY);

   isLoaded = true;

   LCDRefresh(0,0,0,0,0);

   scriptel_close_device(pDevice);

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

   isActive = false;

   return S_OK;
   }
