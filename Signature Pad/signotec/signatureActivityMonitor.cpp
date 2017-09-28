
#include "SignaturePad.h"

#define PEN_UP_PRESSURE_THRESHOLD   256

   long pendingOptionId = -1;
   long pendingX = -1,pendingY = -1;

   long oldTimeStamp = -1;

   void SignaturePad::signatureActivityMonitor(long eventNumber,void *pData,long dataSize,void *pParameter) {

   static long lastX = -1L;
   static long lastY = -1L;
   static long lastPressure = -1L;
   static long lastTimeStamp = -1L;

   if ( ! eventsAllowed )
      return;

   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pParameter);

   if ( STPAD_CALLBACK_TIMEOUT == eventNumber ) {
      STSensorStopTimer();
      pThis -> connectionPointContainer.fire_PenUp(lastX,lastY);
      pThis -> connectionPointContainer.fire_PenPoint(0,0);
      pThis -> penIsDown = false;
      return;
   }

   if ( STPAD_CALLBACK_SIGNATURE == eventNumber ) {

      STSensorStopTimer();

      STSensorStartTimer(0,50,0);

      long *pPoint = (long *)pData;

      long x = pPoint[0] - pThis -> startPointX;
      long y = pPoint[1] - pThis -> startPointY;
      long pressure = pPoint[2];
      long timeStamp = pPoint[3];

      lastX = x;
      lastY = y;
      lastPressure = pressure;
      lastTimeStamp = timeStamp;

      if ( ! pThis -> penIsDown ) {

         long xPixels = (long)((double)x * pThis -> scaleToPadX);
         long yPixels = (long)((double)y * pThis -> scaleToPadY);

         for ( std::list<hotSpot *>::iterator it = pThis -> hotSpots.begin(); it != pThis -> hotSpots.end(); it++ ) {

            hotSpot *ph = (*it);

            if ( ph -> rcLocation.left > xPixels + 4 || ph -> rcLocation.right < xPixels - 4 )
               continue;

            if ( ph -> rcLocation.top > yPixels + 4 || ph -> rcLocation.bottom < yPixels - 4 )
               continue;

            pThis -> connectionPointContainer.fire_OptionSelected(ph -> eventId);

            return;

         }

         pThis -> penIsDown = true;

         return;

      }

      pThis -> connectionPointContainer.fire_PenPoint(x,y);

      return;

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