
#include "SignaturePad.h"

   long pendingOptionId = -1;
   long pendingX = -1,pendingY = -1;

   unsigned int __stdcall SignaturePad::signatureActivityMonitor(void *pv) {

   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv);

   if ( ! pThis -> pISTFamilyDevice )
      return 0L;

   CoInitialize(NULL);

   pThis -> signatureActivityStopRequested = false;

   long rc;
   long region,x,y;
   InputDeviceStatus tipSwitch;

   pThis -> pISTFamilyDevice -> StartReading_2(RunMode_Pen,&rc);

   bool penDown = false;

   InputDeviceStatus oldTipSwitch = InputDeviceStatus_PenTipSwitchFingerUp;

   long upperRightX = (long)((double)RESOLUTION_X * 0.65 / pThis -> scaleToPadX);

   long upperRightY = (long)((double)RESOLUTION_Y * 0.25 / pThis -> scaleToPadY); 

   while ( ! pThis -> signatureActivityStopRequested ) {

      HRESULT hr = pThis -> pISTFamilyDevice -> GetCoordinatePacket(&region,&pThis -> pISTSignaturePoint);

      if ( 0 > region ) {
         Sleep(100);
         continue;
      }

      pThis -> pISTSignaturePoint -> get_SwitchStatus(&tipSwitch);

      pThis -> pISTSignaturePoint -> get_X(&x);

      pThis -> pISTSignaturePoint -> get_Y(&y);

      if ( oldTipSwitch != tipSwitch ) {

         if ( penDown && InputDeviceStatus_PenTipSwitchFingerUp == tipSwitch )
            pThis -> connectionPointContainer.fire_PenUp(x,y);

         else if ( InputDeviceStatus_PenTipSwitchFingerDown == tipSwitch ) {
            pThis -> connectionPointContainer.fire_PenDown(x,y);
            penDown = true;
         }

         pThis -> connectionPointContainer.fire_PenPoint(0,0);

         oldTipSwitch = tipSwitch;

         if ( 3 == region && InputDeviceStatus_PenTipSwitchFingerUp == tipSwitch )
            pThis -> connectionPointContainer.fire_OptionSelected(1L);

         if ( 4 == region && InputDeviceStatus_PenTipSwitchFingerUp == tipSwitch )
            pThis -> connectionPointContainer.fire_OptionSelected(0L);

         continue;

      }

      if ( x > upperRightX && y < upperRightY )
         pThis -> connectionPointContainer.fire_PenPoint(0,0);
      else
         pThis -> connectionPointContainer.fire_PenPoint(x,y);

   }

   if ( pThis -> pISTFamilyDevice )
      pThis -> pISTFamilyDevice -> StopReading_2(RunMode_DigitizerOff,&rc);

   pThis -> hActivityMonitorThread = NULL;

   pThis -> signatureActivityStopRequested = false;

   return 0L;
   }



   unsigned int __stdcall SignaturePad::fireOptionThreaded(void *pv) {
   CoInitialize(NULL);
   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv);
   pThis -> connectionPointContainer.fire_OptionSelected(pendingOptionId);
   pendingOptionId = -1L;
   CoUninitialize();
   return 0;
   }
