
#include "SignaturePad.h"

   void setThreadName(char const * threadName);

   long pendingOptionId = -1;

   static int oldSwValue = -1;
   static bool lastWasMove = false;

   unsigned int __stdcall SignaturePad::signatureActivityMonitor(void *pv) {

   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(pv);

   setThreadName("wacom-report handler");

   auto interfaceQueue = pThis -> pTablet -> interfaceQueue();

   Report activityReport;

   CVReportHandler *pReportHandler = new CVReportHandler(pThis);

   while ( interfaceQueue.wait_getReport_predicate(activityReport) ) {

      pReportHandler -> handleReport(activityReport.begin(),activityReport.end(),false);

      if ( pThis -> signatureActivityStopRequested )
         break;

   }

   delete pReportHandler;

   pThis -> hActivityMonitorThread = NULL;

   pThis -> signatureActivityStopRequested = false;

   return 0;
   }

   void SignaturePad::CVReportHandler::onReport(Protocol::PenData &penData) {

   if ( oldSwValue == penData.sw ) {

      if ( penData.sw > 0 ) {
         lastWasMove = false;
         pSignaturePad -> connectionPointContainer.fire_PenPoint(penData.x,penData.y,0.0f);
      }

      if ( 0 == penData.pressure) {
         if ( ! lastWasMove )
            pSignaturePad -> connectionPointContainer.fire_PenPoint(0,0,0.0f);
         lastWasMove = true;
      }

      return;

   }

   if ( 0 == penData.sw && penData.pressure ) {
      pSignaturePad -> connectionPointContainer.fire_PenUp();
      return;
   }

   oldSwValue = penData.sw;

   bool optionSelected = false;

   if ( penData.pressure && 0 < pSignaturePad -> hotSpots.size() ) {

      long x = (long)((double)penData.x * pSignaturePad -> scaleToPadX);
      long y = (long)((double)penData.y * pSignaturePad -> scaleToPadY);

      for ( std::list<hotSpot *>::iterator it = pSignaturePad -> hotSpots.begin(); it != pSignaturePad -> hotSpots.end(); it++ ) {

         hotSpot *ph = (*it);

         if ( ph -> rcLocation.left > x || ph -> rcLocation.right < x )
            continue;

         if ( ph -> rcLocation.top > y || ph -> rcLocation.bottom < y )
            continue;

         optionSelected = true;

         unsigned int threadAddr;
         pendingOptionId = ph -> eventId;
         HANDLE hThread = (HANDLE)_beginthreadex(NULL,4096,fireOptionThreaded,(void *)pSignaturePad,CREATE_SUSPENDED,&threadAddr);
         ResumeThread(hThread);

         break;

      }

      if ( oldSwValue && ! optionSelected )
         pSignaturePad -> connectionPointContainer.fire_PenDown();


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


   #pragma pack(push,8)

   struct ThreadNameInfo {
   DWORD  dwType;     // Must be 0x1000.
   LPCSTR szName;     // Pointer to name (in user addr space).
   DWORD  dwThreadID; // Thread ID (-1=caller thread).
   DWORD  dwFlags;    // Reserved for future use, must be zero.
   };

   #pragma pack(pop)

   const DWORD MS_VC_EXCEPTION = 0x406D1388;

   void setThreadName(uint32_t threadId, char const * threadName) {

   ThreadNameInfo threadNameInfo;
   threadNameInfo.dwType = 0x1000;
   threadNameInfo.szName = threadName;
   threadNameInfo.dwThreadID = threadId;
   threadNameInfo.dwFlags = 0;

   __try {
   
   ::RaiseException(MS_VC_EXCEPTION, 0, sizeof(threadNameInfo) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR const *>(&threadNameInfo));
   
   } __except (EXCEPTION_EXECUTE_HANDLER) {
   }

   return;
   }
    

   void setThreadName(char const * threadName) {
   setThreadName(0xffffffff, threadName);
   }