
#include "TabletPC.h"

   LRESULT CALLBACK TabletPC::frameHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   TabletPC *p = reinterpret_cast<TabletPC *>(GetWindowLongPtr(hwnd,GWLP_USERDATA));

   switch ( msg ) {

   case WM_CREATE: {
      CREATESTRUCT *pc = reinterpret_cast<CREATESTRUCT *>(lParam);
      p = reinterpret_cast<TabletPC *>(pc -> lpCreateParams);
      SetWindowLongPtr(hwnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(p));
      }
      break;

   case WM_SIZE: {

      HRESULT rc = DefWindowProc(hwnd,msg,wParam,lParam);

      GetWindowRect(hwnd,&p -> rectDisplay);

      p -> rectDisplay.right -= p -> rectDisplay.left;
      p -> rectDisplay.bottom -= p -> rectDisplay.top;
      p -> rectDisplay.left = 0;
      p -> rectDisplay.top = 0;

      SetWindowPos(p -> hwndSigPlusControlHost,HWND_BOTTOM,0,0,p -> rectDisplay.right - p -> rectDisplay.left,p -> rectDisplay.bottom - p -> rectDisplay.top,SWP_SHOWWINDOW);

      return rc;
      }
      break;

   case WM_DESTROY:
      break;

   default:
      break;
   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }



