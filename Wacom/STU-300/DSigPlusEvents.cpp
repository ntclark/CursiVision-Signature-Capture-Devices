
#include <process.h>

#include "SignaturePad.h"

   long pendingOptionId = -1;
   long pendingX = -1,pendingY = -1;

   bool SignaturePad::signaturePadCallback(PEN thePen,void *pv) {

   static int oldSwValue = -1;
   static bool lastWasMove = false;

   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv); 

#if 0

   if ( 0 == thePen.sw ) {
      if ( 0 < oldSwValue )
         pThis -> connectionPointContainer.fire_PenUp();
      oldSwValue = thePen.sw;
      return false;
   }

   if ( 1 == thePen.sw && 0 == oldSwValue ) {
      pThis -> connectionPointContainer.fire_PenDown();
      oldSwValue = thePen.sw;
      return false;
   }

   long x = (long)((double)thePen.x * pThis -> scaleToPadX);
   long y = (long)((double)thePen.y * pThis -> scaleToPadY);

   pThis -> connectionPointContainer.fire_PenPoint(x,y,0.0f);

#else

   if ( oldSwValue != thePen.sw ) {

      if ( 0 == thePen.sw && thePen.press ) {
         pThis -> connectionPointContainer.fire_PenUp();
         return true;
      }

      oldSwValue = thePen.sw;

      if ( oldSwValue )
         pThis -> connectionPointContainer.fire_PenDown();

      if ( thePen.press && 0 < pThis -> hotSpots.size() ) {

         long x = (long)((double)thePen.x * pThis -> scaleToPadX);
         long y = (long)((double)thePen.y * pThis -> scaleToPadY);

         for ( std::list<hotSpot *>::iterator it = pThis -> hotSpots.begin(); it != pThis -> hotSpots.end(); it++ ) {

            hotSpot *ph = (*it);

            if ( ph -> rcLocation.left > x || ph -> rcLocation.right < x )
               continue;

            if ( ph -> rcLocation.top > y || ph -> rcLocation.bottom < y )
               continue;

            EndCapture();

            unsigned int threadAddr;
            pendingOptionId = ph -> eventId;
            HANDLE hThread = (HANDLE)_beginthreadex(NULL,4096,fireOptionThreaded,(void *)pThis,CREATE_SUSPENDED,&threadAddr);
            ResumeThread(hThread);

            break;

         }

      }

      return false;
   }

   if ( thePen.sw > 0 ) {
      lastWasMove = false;
      pThis -> connectionPointContainer.fire_PenPoint(thePen.x,thePen.y,0.0f);
   }

   if ( 0 == thePen.press ) {
      if ( ! lastWasMove )
         pThis -> connectionPointContainer.fire_PenPoint(0,0,0.0f);
      lastWasMove = true;
   }

#endif

	return false;
   }


   unsigned int __stdcall SignaturePad::fireOptionThreaded(void *pv) {
   CoInitialize(NULL);
   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv);
   pThis -> connectionPointContainer.fire_OptionSelected(pendingOptionId);
   pendingOptionId = -1L;
   CoUninitialize();
   return 0;
   }
