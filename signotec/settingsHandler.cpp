/*
                       Copyright (c) 2009,2010,2011,2012,2012,2014 Nathan T. Clark
*/

#include "SignaturePad.h"

#include <shellapi.h>
#include <shTypes.h>
#include <shlobj.h>
#include <commctrl.h>

#define PUT_BOOL(v,id)  SendDlgItemMessage(hwnd,id,BM_SETCHECK,v ? BST_CHECKED : BST_UNCHECKED,0L);
#define PUT_LONG(v,id)  { char szX[32]; sprintf(szX,"%ld",v); SetDlgItemText(hwnd,id,szX); }
#define PUT_DOUBLE(v,id) { char szX[64]; sprintf(szX,"%5.2lf",v); SetDlgItemText(hwnd,id,szX); }
#define PUT_STRING(v,id) SetDlgItemText(hwnd,id,v);

#define LOAD_CONTROLS                                         \
{                                                             \
   PUT_DOUBLE(p -> padWidthInInches,IDDI_PAD_WIDTH_IN_INCHES) \
   PUT_DOUBLE(p -> signatureTimeOutPeriod,IDDI_PAD_TIMEOUT)   \
}

#define GET_BOOL(v,id)  v = BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L);
#define GET_LONG(v,id) {char szX[32]; GetDlgItemText(hwnd,id,szX,32); v = atol(szX); }
#define GET_DOUBLE(v,id) {char szX[64]; GetDlgItemText(hwnd,id,szX,64); v = atof(szX); }
#define GET_STRING(v,id) GetDlgItemText(hwnd,id,v,MAX_PATH);

#define UNLOAD_CONTROLS                                       \
{                                                             \
   GET_DOUBLE(p -> padWidthInInches,IDDI_PAD_WIDTH_IN_INCHES) \
   GET_DOUBLE(p -> signatureTimeOutPeriod,IDDI_PAD_TIMEOUT)   \
}
                         
   extern "C" void disableAll(HWND hwnd,long *pExceptions);
   extern "C" void enableAll(HWND hwnd,long *pExceptions);

   long entryPadSelection = -1L;

   PAD_IMPLEMENTATION_FILES

   static HWND hwndPadImage = NULL;
   static WNDPROC nativePadImageHandler = NULL;

   LRESULT CALLBACK SignaturePad::settingsHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   SignaturePad *p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);
      p = (SignaturePad *)pPage -> lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)p);
      p -> pIGProperties -> Push();
      p -> pIGProperties -> Push();

      for ( long k = IDDI_PAD_MODEL_MIN; k <= IDDI_PAD_MODEL_MAX; k++ ) {
         char szModel[128];
         GetWindowText(GetDlgItem(hwnd,k),szModel,128);
         if ( strstr(szModel,p -> DeviceProductName()) ) {
            SendMessage(GetDlgItem(hwnd,k),BM_SETCHECK,BST_CHECKED,0L);
            entryPadSelection = k;
            break;
         }
      }

      LOAD_CONTROLS;

      long exceptions[] = {0,0};
      enableAll(hwnd,exceptions);

      char szTemp[1024];

      LoadString(hModule,IDDI_PAD_MODEL_CHOICE_LABEL2,szTemp,sizeof(szTemp));
      SetDlgItemText(hwnd,IDDI_PAD_MODEL_CHOICE_LABEL2,szTemp);

      BOOL isAdministrator = false;
      SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
      PSID administratorsGroup; 

      isAdministrator = AllocateAndInitializeSid(&ntAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_ADMINS,0, 0, 0, 0, 0, 0,&administratorsGroup); 

      if ( isAdministrator ) {
         if ( ! CheckTokenMembership(NULL,administratorsGroup,&isAdministrator) ) 
            isAdministrator = false;
         FreeSid(administratorsGroup); 
      }

      if ( ! isAdministrator ) {
         LoadString(hModule,IDDI_PAD_MODEL_CHOICE_NONADMIN,szTemp,sizeof(szTemp));
         SignaturePad::defaultTextHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PAD_MODEL_CHOICE_NONADMIN),GWLP_WNDPROC,(LONG_PTR)SignaturePad::redTextHandler);
         SetDlgItemText(hwnd,IDDI_PAD_MODEL_CHOICE_NONADMIN,szTemp);
         ShowWindow(GetDlgItem(hwnd,IDDI_PAD_MODEL_CHOICE_LABEL2),SW_HIDE);
         for ( long k = IDDI_PAD_MODEL_MIN; k <= IDDI_PAD_MODEL_MAX; k++ ) 
            EnableWindow(GetDlgItem(hwnd,k),FALSE);
         ShowWindow(GetDlgItem(hwnd,IDDI_PAD_MODEL_CHOICE_NONADMIN),SW_SHOW);
      } else
         ShowWindow(GetDlgItem(hwnd,IDDI_PAD_MODEL_CHOICE_NONADMIN),SW_HIDE);

      }
      return LRESULT(FALSE);

   case WM_COMMAND: {

      if ( IDDI_PAD_MODEL_MIN <= LOWORD(wParam) && LOWORD(wParam) <= IDDI_PAD_MODEL_MAX ) {
         if ( BST_CHECKED != SendMessage((HWND)lParam,BM_GETCHECK,0L,0L) )
            break;
         long exceptions[32];
         memset(exceptions,0,32 * sizeof(long));
         for ( long j = IDDI_PAD_MODEL_MIN; j <= IDDI_PAD_MODEL_MAX; j++ )
            exceptions[j - IDDI_PAD_MODEL_MIN] = j;
         exceptions[IDDI_PAD_MODEL_MAX - IDDI_PAD_MODEL_MIN + 1] = IDDI_PAD_MODEL_CHOICE_LABEL1;
         exceptions[IDDI_PAD_MODEL_MAX - IDDI_PAD_MODEL_MIN + 2] = IDDI_PAD_MODEL_CHOICE_LABEL2;
         if ( LOWORD(wParam) != entryPadSelection )
            disableAll(hwnd,exceptions);
         else {
            exceptions[0] = 0;
            enableAll(hwnd,exceptions);
         }
         break;
      }

      switch ( LOWORD(wParam) ) {

      case IDDI_PAD_PROPERTIES_ACCEPT:
         UNLOAD_CONTROLS;
         break;

      case IDDI_PAD_ORIGINPOINT_SET: {
         DialogBoxParam(hModule,MAKEINTRESOURCE(IDD_PAD_ORIGINPOINT),hwnd,(DLGPROC)SignaturePad::originPointHandler,(ULONG_PTR)p);
         }
         break;

      case IDDI_INK_PROPERTIES_SET: {
         DialogBoxParam(hModule,MAKEINTRESOURCE(IDD_PAD_INK_PROPERTIES),hwnd,(DLGPROC)SignaturePad::inkHandler,(ULONG_PTR)p);
         }
         break;

      default:
         break;
      }

      }
      break;


   case WM_NOTIFY: {

      NMHDR *pNotifyHeader = (NMHDR *)lParam;

      switch ( pNotifyHeader -> code ) {

      case PSN_SETACTIVE:
         LOAD_CONTROLS
         break;

      case PSN_KILLACTIVE:
         SetWindowLongPtr(pNotifyHeader -> hwndFrom,DWLP_MSGRESULT,FALSE);
         break;

      case PSN_APPLY: {

         PSHNOTIFY *pNotify = (PSHNOTIFY *)lParam;

         UNLOAD_CONTROLS;

         if ( pNotify -> lParam ) {

            bool saveProperties = false;
            for ( long k = IDDI_PAD_MODEL_MIN; k <= IDDI_PAD_MODEL_MAX; k++ ) {
               long isSelected = (long)SendMessage(GetDlgItem(hwnd,k),BM_GETCHECK,0L,0L);
               if ( isSelected ) {
                  if ( entryPadSelection == k ) {
                     saveProperties = true;
                     break;
                  }
                  if ( p -> pICursiVisionServices )
                     p -> pICursiVisionServices -> RegisterSignaturePad(wszFiles[k - IDDI_PAD_MODEL_MIN]);
                  break;
               }
            }

            if ( saveProperties )
               p -> pIGProperties -> Save();

            p -> pIGProperties -> Discard();
            p -> pIGProperties -> Discard();

         } else {

            p -> pIGProperties -> Discard();
            p -> pIGProperties -> Push();

         }

         SetWindowLongPtr(pNotifyHeader -> hwndFrom,DWLP_MSGRESULT,PSNRET_NOERROR);

         }
         break;

      case PSN_RESET:
         p -> pIGProperties -> Pop();
         p -> pIGProperties -> Pop();
         break;

      default:
         break;
      }

      }
      break;

   default:
      break;
   }

   return LRESULT(FALSE);
   }

   static boolean mouseEntered = false;
   static boolean isSettingMode = false;

   static long lastMouseX = -1L, lastMouseY = -1L;
   static long currentMouseX = -1L, currentMouseY = -1L;
   static long originalOriginX = -1L, originalOriginY = -1L;

   LRESULT CALLBACK SignaturePad::originPointHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   SignaturePad *p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      SetWindowLongPtr(hwnd,GWLP_USERDATA,lParam);

      p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

      long cx = p -> LCDWidth();
      long cy = p -> LCDHeight();

      WNDCLASS gClass;
      
      memset(&gClass,0,sizeof(WNDCLASS));
      gClass.style = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
      gClass.lpfnWndProc = SignaturePad::originPointPadImageHandler;
      gClass.cbClsExtra = 32;
      gClass.cbWndExtra = 32;
      gClass.hInstance = hModule;
      gClass.hIcon = NULL;
      gClass.hCursor = NULL;
      gClass.hbrBackground = 0;
      gClass.lpszMenuName = NULL;
      gClass.lpszClassName = "PadImage";

      RegisterClass(&gClass);

      hwndPadImage = CreateWindowEx(0*WS_EX_CLIENTEDGE,"PadImage","",WS_CHILD | WS_VISIBLE, 8, 64, cx, cy, hwnd, NULL, hModule,(void *)lParam);

      RECT rcButton;
      GetWindowRect(GetDlgItem(hwnd,IDDI_PAD_ORIGINPOINT_OK),&rcButton);

      long bottom = 32 + cy + 8 + 14 + 4 + 64;

      SetWindowPos(hwnd,HWND_TOP,0,0,cx + 24, bottom, SWP_NOMOVE);

      cx = rcButton.right - rcButton.left;
      cy = rcButton.bottom - rcButton.top;

      SetWindowPos(GetDlgItem(hwnd,IDDI_PAD_ORIGINPOINT_OK),HWND_TOP,4, bottom - cy - 8 - 24, 0L, 0L, SWP_NOSIZE);
      SetWindowPos(GetDlgItem(hwnd,IDDI_PAD_ORIGINPOINT_CANCEL),HWND_TOP,4 + cx + 8, bottom - cy - 8 - 24, 0L, 0L, SWP_NOSIZE);

      originalOriginX = p -> originPoint.x;
      originalOriginY = p -> originPoint.y;

      POINT ptl;
      GetCursorPos(&ptl);

      SetWindowPos(hwnd,HWND_TOP,ptl.x + 32,ptl.y + 32,0,0,SWP_NOSIZE);

      }
      break;

   case WM_COMMAND: {

      switch ( LOWORD(wParam) ) {
      case IDDI_PAD_ORIGINPOINT_OK: {
         if ( -1L != currentMouseX )
            p -> originPoint.x = currentMouseX;                                                                                           
         if ( -1L != currentMouseY ) 
            p -> originPoint.y = currentMouseY;                                                                                           
         EndDialog(hwnd,0L);         
         }
         break;

      case IDDI_PAD_ORIGINPOINT_CANCEL:
         EndDialog(hwnd,1L);         
         break;
      }

      }
      break;

   }

   return (LRESULT)0L;
   }

#define POINT_RADIUS 8

#define REFRESH_ORIGIN_POINT                                                                                                     \
      {                                                                                                                          \
      HDC hdc = GetDC(hwnd);                                                                                                     \
      HPEN hPen = CreatePen(PS_SOLID,1,RGB(128,128,128));                                                                        \
      HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));                                                                              \
                                                                                                                                 \
      HPEN oldPen = (HPEN)SelectObject(hdc,hPen);                                                                                \
      HBRUSH oldBrush = (HBRUSH)SelectObject(hdc,hBrush);                                                                        \
                                                                                                                                 \
      SetROP2(hdc,R2_XORPEN);                                                                                                    \
                                                                                                                                 \
      if ( -1L != lastMouseX ) {                                                                                                 \
         MoveToEx(hdc,lastMouseX,lastMouseY,NULL);                                                                               \
         Ellipse(hdc,lastMouseX - POINT_RADIUS,lastMouseY - POINT_RADIUS,lastMouseX + POINT_RADIUS,lastMouseY + POINT_RADIUS);   \
      }                                                                                                                          \
                                                                                                                                 \
      lastMouseX = LOWORD(lParam);                                                                                               \
      lastMouseY = HIWORD(lParam);                                                                                               \
      Ellipse(hdc,lastMouseX - POINT_RADIUS,lastMouseY - POINT_RADIUS,lastMouseX + POINT_RADIUS,lastMouseY + POINT_RADIUS);      \
                                                                                                                                 \
      DeleteObject(SelectObject(hdc,oldPen));                                                                                    \
      DeleteObject(SelectObject(hdc,oldBrush));                                                                                  \
                                                                                                                                 \
      ReleaseDC(hwnd,hdc);                                                                                                       \
      }
                                                                                                                                 

   LRESULT CALLBACK SignaturePad::originPointPadImageHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   SignaturePad *p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {
   case WM_CREATE: {
      CREATESTRUCT *pcs = (CREATESTRUCT *)lParam;
      SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)pcs -> lpCreateParams);
      }
      break;

   case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hwnd,&ps);
      FillRect(ps.hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 1));
#if PAD == 2 || PAD == 3
      {
      HPEN hPen = CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 2, 0L); 
      HGDIOBJ oldPen = SelectObject(ps.hdc,hPen);
      MoveToEx(ps.hdc,0,p -> BaselinePointY(),NULL);
      LineTo(ps.hdc,p -> Width(),p -> BaselinePointY());
      SelectObject(ps.hdc,oldPen);
      DeleteObject(hPen);
      }
#endif
      HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
      HBRUSH oldBrush = (HBRUSH)SelectObject(ps.hdc,hBrush);

      HPEN hPen = CreatePen(PS_SOLID,1,RGB(128,128,128));
      HPEN oldPen = (HPEN)SelectObject(ps.hdc,hPen);
      SetROP2(ps.hdc,R2_XORPEN);                                                                                                    

      Ellipse(ps.hdc,p -> originPoint.x - POINT_RADIUS,p -> originPoint.y - POINT_RADIUS,p -> originPoint.x + POINT_RADIUS,p -> originPoint.y + POINT_RADIUS);

      lastMouseX = p -> originPoint.x;
      lastMouseY = p -> originPoint.y;
      DeleteObject(SelectObject(ps.hdc,oldBrush));
      EndPaint(hwnd,&ps);
      return (LRESULT)0L;
      }

   case WM_LBUTTONDOWN: {

      if ( isSettingMode ) {
         currentMouseX = LOWORD(lParam);
         currentMouseY = HIWORD(lParam);
         isSettingMode = false;
         break;
      }

      isSettingMode = true;

      REFRESH_ORIGIN_POINT

      }
      break;

   case WM_MOUSEMOVE: {

      if ( LOWORD(lParam) == lastMouseX && HIWORD(lParam) == lastMouseY )
         break;

      if ( ! isSettingMode )
         break;

      if ( ! mouseEntered ) {
         TRACKMOUSEEVENT mouseTracking = {0};
         mouseTracking.cbSize = sizeof(TRACKMOUSEEVENT);
         mouseTracking.dwFlags = TME_LEAVE;
         mouseTracking.hwndTrack = hwnd;
         TrackMouseEvent(&mouseTracking);
         mouseEntered = true;
      }

      REFRESH_ORIGIN_POINT

      }                                                                                                                          
      break;                                                                                                                     
                                                                                                                                 
   case WM_MOUSELEAVE: {                                                                                                         
                                                                                                                                 
      mouseEntered = false;                                                                                                      

      if ( ! isSettingMode ) 
         break;
                                                                                                                           
      HDC hdc = GetDC(hwnd);                                                                                                     
      HPEN hPen = CreatePen(PS_SOLID,1,RGB(128,128,128));                                                                        
      HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));                                                                            
                                                                                                                                 
      HPEN oldPen = (HPEN)SelectObject(hdc,hPen);                                                                                
      HBRUSH oldBrush = (HBRUSH)SelectObject(hdc,hBrush);                                                                        
                                                                                                                                 
      SetROP2(hdc,R2_XORPEN);                                                                                                    
                                                                                                                                 
      if ( -1L != lastMouseX ) {                                                                                                 
         MoveToEx(hdc,lastMouseX,lastMouseY,NULL);                                                                               
         Ellipse(hdc,lastMouseX - POINT_RADIUS,lastMouseY - POINT_RADIUS,lastMouseX + POINT_RADIUS,lastMouseY + POINT_RADIUS);   
      }                                                                                                                          
                                                                                                                                 
      DeleteObject(SelectObject(hdc,oldPen));                                                                                    
      DeleteObject(SelectObject(hdc,oldBrush));                                                                                  
                                                                                                                                 
      ReleaseDC(hwnd,hdc);

      lastMouseX = -1L;
      lastMouseY = -1L;

      }
      break;

   }

   return DefWindowProc(hwnd,msg,wParam,lParam);
   }



   static DWORD entryInkColor = -1L;

   LRESULT CALLBACK SignaturePad::inkHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   SignaturePad *p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      SetWindowLongPtr(hwnd,GWLP_USERDATA,lParam);
      p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_COLOR_SWATCH),GWLP_USERDATA,(LONG_PTR)p);
      SignaturePad::defaultSwatchHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_COLOR_SWATCH),GWLP_WNDPROC,(LONG_PTR)SignaturePad::swatchHandler);
      EnableWindow(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_COLOR_SWATCH),FALSE);
      char szX[32];
      sprintf(szX,"%ld",p -> penWidth);
      SetWindowText(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_WEIGHT),szX);

      RECT rcWeight,rcParent;

      GetWindowRect(hwnd,&rcParent);
      GetWindowRect(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_WEIGHT),&rcWeight);

      rcWeight.left -= rcParent.left;
      rcWeight.right -= rcParent.left;
      rcWeight.top -= rcParent.top;
      rcWeight.bottom -= rcParent.top;

      long cyWeight = rcWeight.bottom - rcWeight.top;

      long dx = rcWeight.right - rcWeight.left;
      long dy = rcWeight.bottom - rcWeight.top;

      RECT rcAdjust;
      memcpy(&rcAdjust,&rcWeight,sizeof(RECT));
      AdjustWindowRect(&rcAdjust,(long)GetWindowLongPtr(hwnd,GWL_STYLE),FALSE);

      dx = dx - (rcAdjust.right - rcAdjust.left);
      dy = dy - (rcAdjust.bottom - rcAdjust.top);

      HWND weightSpinner = CreateWindowEx(0L,UPDOWN_CLASS,"",WS_VISIBLE | WS_CHILD | UDS_SETBUDDYINT,rcWeight.right + dx + 2,rcWeight.top + dy + 2,30,cyWeight,
                                              hwnd,(HMENU)IDDI_INK_PROPERTIES_WEIGHT_SPIN,hModule,(void *)p);

      SendMessage(weightSpinner,UDM_SETRANGE,0L,MAKELONG(16,1));
      SendMessage(weightSpinner,UDM_SETPOS,0L,MAKELONG((short)p -> penWidth,0));
      SendMessage(weightSpinner,UDM_SETBUDDY,(WPARAM)GetDlgItem(hwnd,IDDI_INK_PROPERTIES_WEIGHT),0L);

      entryInkColor = p -> inkColor;

      POINT ptl;
      GetCursorPos(&ptl);

      SetWindowPos(hwnd,HWND_TOP,ptl.x + 32,ptl.y + 32,0,0,SWP_NOSIZE);

      }
      break;

   case WM_COMMAND:

      switch ( LOWORD(wParam) ) {
      case IDDI_INK_PROPERTIES_OK:
         char szX[32];
         GetWindowText(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_WEIGHT),szX,32);
         p -> penWidth = atol(szX);
         EndDialog(hwnd,1L);
         break;

      case IDDI_INK_PROPERTIES_CANCEL:
         p -> inkColor = entryInkColor;
         EndDialog(hwnd,0L);
         break;

      case IDDI_INK_PROPERTIES_COLOR_CHOOSE: {
         CHOOSECOLOR chooseColor = {0};
         COLORREF customColors[16];
         memset(customColors,0xFFFF,16 * sizeof(COLORREF));
         chooseColor.lStructSize = sizeof(CHOOSECOLOR);
         chooseColor.hwndOwner = hwnd;
         chooseColor.hInstance = (HWND)hModule;
         chooseColor.rgbResult = p -> inkColor;
         chooseColor.Flags = CC_RGBINIT | CC_FULLOPEN;
         chooseColor.lpCustColors = customColors;
         if ( ChooseColor(&chooseColor) ) {
            p -> inkColor = chooseColor.rgbResult;
            InvalidateRect(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_COLOR_SWATCH),NULL,TRUE);  
         }
         }
         break;
      }

   default:
      break;

   }
   return (LRESULT)0;
   }

   
   LRESULT CALLBACK SignaturePad::swatchHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   SignaturePad *p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_PAINT: {
      PAINTSTRUCT ps = {0};
      HBRUSH hb = CreateSolidBrush((COLORREF)p -> inkColor);
      BeginPaint(hwnd,&ps);
      FillRect(ps.hdc,&ps.rcPaint,hb);
      DeleteObject(hb);
      EndPaint(hwnd,&ps);
      }
      return (LRESULT)0L;

   default:
      break;

   }

   return CallWindowProc(SignaturePad::defaultSwatchHandler,hwnd,msg,wParam,lParam);
   }


   LRESULT CALLBACK SignaturePad::redTextHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   SignaturePad *p = (SignaturePad *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_PAINT: {
      PAINTSTRUCT ps = {0};
      BeginPaint(hwnd,&ps);
      char szText[1024];
      GetWindowText(hwnd,szText,1024);
      HFONT hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SelectObject(ps.hdc,hGUIFont);
      SetTextColor(ps.hdc,RGB(255,0,0));
      SetBkColor(ps.hdc,GetSysColor(COLOR_MENU));
      DrawText(ps.hdc,szText,(DWORD)strlen(szText),&ps.rcPaint,DT_TOP);
      EndPaint(hwnd,&ps);
      }
      break;

   default:
      break;

   }

   return CallWindowProc(SignaturePad::defaultTextHandler,hwnd,msg,wParam,lParam);
   }


   BOOL CALLBACK doDisable(HWND hwndTest,LPARAM lParam);
   BOOL CALLBACK doEnable(HWND hwndTest,LPARAM lParam);

   extern "C" void enableAll(HWND hwnd,long *pExceptions) {
   EnumChildWindows(hwnd,doEnable,(LPARAM)pExceptions);
   return;
   }

   extern "C" void disableAll(HWND hwnd,long *pExceptions) {
   EnumChildWindows(hwnd,doDisable,(LPARAM)pExceptions);
   return;
   }

   BOOL CALLBACK doDisable(HWND hwndTest,LPARAM lParam) {
   long *pExceptions = (long *)lParam;
   long id = (long)GetWindowLongPtr(hwndTest,GWL_ID);
   for ( long k = 0; 1; k++ ) {
      if ( ! pExceptions[k] )
         break;
      if ( id == pExceptions[k] ) {
         EnableWindow(hwndTest,TRUE);
         return TRUE;
      }
   }
   EnableWindow(hwndTest,FALSE);
   return TRUE;
   }


   BOOL CALLBACK doEnable(HWND hwndTest,LPARAM lParam) {
   long *pExceptions = (long *)lParam;
   long id = (long)GetWindowLongPtr(hwndTest,GWL_ID);
   for ( long k = 0; 1; k++ ) {
      if ( ! pExceptions[k] )
         break;
      if ( id == pExceptions[k] ) {
         EnableWindow(hwndTest,FALSE);
         return TRUE;
      }
   }
   EnableWindow(hwndTest,TRUE);
   return TRUE;
   }
