
#include "SignaturePad.h"

#define PEN_UP_PRESSURE_THRESHOLD   256

   long pendingOptionId = -1;
   long pendingX = -1,pendingY = -1;

   long oldTimeStamp = -1;

   long ignoreOption = 0L;
   HANDLE hMutexIgnoreOption = INVALID_HANDLE_VALUE;
   unsigned int __stdcall resetOptions(void *);

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
      pThis -> connectionPointContainer.fire_PenPoint(0,0,0.0f);
      pThis -> penIsDown = false;
      return;
   }

   if ( STPAD_CALLBACK_SIGNATURE == eventNumber ) {

      STSensorStopTimer();

      STSensorStartTimer(0,50,0);

      long *pPoint = (long *)pData;

      long x = pPoint[0];
      long y = pPoint[1];

      long pressure = pPoint[2];
      long timeStamp = pPoint[3];

      lastX = x;
      lastY = y;
      lastPressure = pressure;
      lastTimeStamp = timeStamp;

      if ( ! pThis -> penIsDown ) {

         long xPixels = STSignatureScaleToDisplay(x);
         long yPixels = STSignatureScaleToDisplay(y);

         for ( hotSpot *ph : pThis -> hotSpots ) {

            if ( ph -> rcLocation.left > xPixels + 4 || ph -> rcLocation.right < xPixels - 4 )
               continue;

            if ( ph -> rcLocation.top > yPixels + 4 || ph -> rcLocation.bottom < yPixels - 4 )
               continue;

            if ( ignoreOption )
               return;

            pThis -> connectionPointContainer.fire_OptionSelected(ph -> eventId);

            if ( INVALID_HANDLE_VALUE == hMutexIgnoreOption )
               hMutexIgnoreOption = CreateMutex(NULL,FALSE,NULL);

            WaitForSingleObject(hMutexIgnoreOption,INFINITE);

            unsigned threadAddress;
            (HANDLE)_beginthreadex(NULL,0,resetOptions,(void *)NULL,0L,&threadAddress);

            ignoreOption = 1L;

            ReleaseMutex(hMutexIgnoreOption);

            return;

         }

         pThis -> penIsDown = true;

         return;

      }

      pThis -> connectionPointContainer.fire_PenPoint(x,y,0.0f);

      return;

   }

   return;
   }

   unsigned int __stdcall resetOptions(void *) {

   Sleep(1000);

   WaitForSingleObject(hMutexIgnoreOption,INFINITE);

   ignoreOption = 0L;

   ReleaseMutex(hMutexIgnoreOption);

   return 0;
   }