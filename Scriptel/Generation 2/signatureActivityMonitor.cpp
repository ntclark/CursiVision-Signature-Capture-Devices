
#include "SignaturePad.h"

   long pendingOptionId = -1;
   long pendingX = -1,pendingY = -1;

   unsigned int __stdcall SignaturePad::signatureActivityMonitor(void *pv) {

   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv);

   pThis -> pDevice -> user_defined = pv;

   pThis -> signatureActivityStopRequested = false;

   scriptel_register_input_callback(pThis -> pDevice,pThis -> signatureReportCallback);

   //scriptel_open_device(pThis -> pDevice);

   while ( pThis -> pDevice -> open && ! pThis -> signatureActivityStopRequested )
      scriptel_device_read(pThis -> pDevice);

   pThis -> hActivityMonitorThread = NULL;

   pThis -> signatureActivityStopRequested = false;

   //scriptel_close_device(pThis -> pDevice);

   return 0;
   }


   void SignaturePad::signatureReportCallback(scriptel_device *pDevice,scriptel_input_report *pInputReport,unsigned char reportId,unsigned char *pReportBuffer,unsigned int reportLength) {

   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pDevice -> user_defined);

   static long lastX = -1L;
   static long lastY = -1L;

   switch ( reportId ) {

   case SCRIPTEL_INPUT_COORDINATE:
   case SCRIPTEL_INPUT_EXTENDEDCOORDINATE: {

         long x = pInputReport -> extended_coordinate.x;
         long y = pInputReport -> extended_coordinate.y;

         if ( pInputReport -> extended_coordinate.sample_info.pen.tip_switch ) {
            if ( -1L == lastX )
               pThis -> connectionPointContainer.fire_PenDown(x,y);
            else
               pThis -> connectionPointContainer.fire_PenPoint(x,y,0.0f);
            lastX = x;
            lastY = y;
         } else {
            pThis -> connectionPointContainer.fire_PenUp(x,y);
            lastX = -1L;
            lastY = -1L;
         }

      }
      break;

   case SCRIPTEL_INPUT_BUTTONPRESS:

      if ( 0 == stricmp("ok",pInputReport -> button_press.region -> button.caption) )
         pThis -> connectionPointContainer.fire_OptionSelected(1L);
      else {
         pThis -> connectionPointContainer.fire_OptionSelected(0L);
         scriptel_clear_screen(pThis -> pDevice);
      }

      lastX = -1L;
      lastY = -1L;

      break;
   }

   return;
   }



   unsigned int __stdcall SignaturePad::fireOptionThreaded(void *pv) {
   CoInitialize(NULL);
   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv);
   pThis -> connectionPointContainer.fire_OptionSelected(pendingOptionId);
   pendingOptionId = -1L;
   CoUninitialize();
   return 0;
   }
