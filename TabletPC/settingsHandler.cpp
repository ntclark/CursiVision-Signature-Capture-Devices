/*
                       Copyright (c) 2009,2010,2011,2012,2013,2014 Nathan T. Clark
*/

#include "TabletPC.h"

#include <shlobj.h>

#define PUT_BOOL(v,id)  SendDlgItemMessage(hwnd,id,BM_SETCHECK,v ? BST_CHECKED : BST_UNCHECKED,0L);
#define PUT_LONG(v,id)  { char szX[32]; sprintf(szX,"%ld",v); SetDlgItemText(hwnd,id,szX); }
#define PUT_DOUBLE(v,id) { char szX[64]; sprintf(szX,"%5.2lf",v); SetDlgItemText(hwnd,id,szX); }
#define PUT_STRING(v,id) SetDlgItemText(hwnd,id,v);

#define LOAD_CONTROLS                                         \
{                                                             \
   PUT_DOUBLE(p -> signatureTimeOutPeriod,IDDI_PAD_TIMEOUT)   \
}

#define GET_BOOL(v,id)  v = BST_CHECKED == SendDlgItemMessage(hwnd,id,BM_GETCHECK,0L,0L);
#define GET_LONG(v,id) {char szX[32]; GetDlgItemText(hwnd,id,szX,32); v = atol(szX); }
#define GET_DOUBLE(v,id) {char szX[64]; GetDlgItemText(hwnd,id,szX,64); v = atof(szX); }
#define GET_STRING(v,id) GetDlgItemText(hwnd,id,v,MAX_PATH);

#define UNLOAD_CONTROLS                                       \
{                                                             \
   GET_DOUBLE(p -> signatureTimeOutPeriod,IDDI_PAD_TIMEOUT)   \
}

   long entryPadSelection = -1L;
                                
   char szFeatureDescriptions[][64] = {"The InnoVisioNate Phablet signature Pad","Wacom LCD","signotec Signature pads","Scriptel ST1500","Windows Tablet PC OS"};
   wchar_t wszFiles[][64] = {L"pkAPI.ocx",L"WacomCV.ocx",L"signotecCV.ocx",L"ScriptelST1500.ocx",L"TabletPC.ocx"};

   extern "C" void disableAll(HWND hwnd,long *pExceptions);
   extern "C" void enableAll(HWND hwnd,long *pExceptions);

   LRESULT CALLBACK TabletPC::settingsHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   TabletPC *p = (TabletPC *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {

      PROPSHEETPAGE *pPage = reinterpret_cast<PROPSHEETPAGE *>(lParam);

      p = (TabletPC *)pPage -> lParam;

      SetWindowLongPtr(hwnd,GWLP_USERDATA,(LONG_PTR)p);

      p -> pIGProperties -> Push();
      p -> pIGProperties -> Push();

      LOAD_CONTROLS;

#ifndef EVALUATION_BUILD
      for ( long k = IDDI_PAD_MODEL_MIN; k <= IDDI_PAD_MODEL_MAX; k++ ) {
         char szModel[128];
         GetWindowText(GetDlgItem(hwnd,k),szModel,128);
         if ( strstr(szModel,p -> signatureDeviceProductName) ) {
            SendMessage(GetDlgItem(hwnd,k),BM_SETCHECK,BST_CHECKED,0L);
            entryPadSelection = k;
            break;
         }
      }
#endif

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
         TabletPC::defaultTextHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_PAD_MODEL_CHOICE_NONADMIN),GWLP_WNDPROC,(LONG_PTR)TabletPC::redTextHandler);
         SetDlgItemText(hwnd,IDDI_PAD_MODEL_CHOICE_NONADMIN,szTemp);
         ShowWindow(GetDlgItem(hwnd,IDDI_PAD_MODEL_CHOICE_LABEL2),SW_HIDE);
         for ( long k = IDDI_PAD_MODEL_MIN; k <= IDDI_PAD_MODEL_MAX; k++ ) 
            EnableWindow(GetDlgItem(hwnd,k),FALSE);
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

      case IDDI_INK_PROPERTIES_SET:
         DialogBoxParam(hModule,MAKEINTRESOURCE(IDD_PAD_INK_PROPERTIES),hwnd,(DLGPROC)TabletPC::inkHandler,(long)p);
         break;

      default:
         break;
      }

      }
      break;


   case WM_NOTIFY: {

      NMHDR *pNotifyHeader = (NMHDR *)lParam;

      switch ( pNotifyHeader -> code ) {

      case PSN_SETACTIVE: {
         LOAD_CONTROLS
         }
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
               long isSelected = SendMessage(GetDlgItem(hwnd,k),BM_GETCHECK,0L,0L);
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

            for ( long k = IDDI_PAD_MODEL_MIN; k <= IDDI_PAD_MODEL_MAX; k++ ) {
               long isSelected = SendMessage(GetDlgItem(hwnd,k),BM_GETCHECK,0L,0L);
               if ( isSelected ) {
                  if ( entryPadSelection == k )
                     break;
                  if ( p -> pICursiVisionServices )
                     p -> pICursiVisionServices -> RegisterSignaturePad(wszFiles[k - IDDI_PAD_MODEL_MIN]);
                  break;
               }

            }

         } else {
            p -> pIGProperties -> Discard();
            p -> pIGProperties -> Push();
         }

         SetWindowLongPtr(pNotifyHeader -> hwndFrom,DWLP_MSGRESULT,PSNRET_NOERROR);

         }
         break;

      case PSN_RESET: {
         p -> pIGProperties -> Pop();
         p -> pIGProperties -> Pop();
         }
         break;

      case TTN_NEEDTEXT: {
         NMTTDISPINFO *pNotify = (NMTTDISPINFO *)lParam;
         long k = 0L;
         if ( pNotify->uFlags & TTF_IDISHWND )
            k = SendMessage((HWND)pNotify -> hdr.idFrom,TBM_GETPOS,0L,0L);
         else
            k = SendDlgItemMessage(hwnd,pNotify -> hdr.idFrom,TBM_GETPOS,0L,0L);
         }
         return (LRESULT)0;

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


   static DWORD entryInkColor = -1L;


   LRESULT CALLBACK TabletPC::inkHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   TabletPC *p = (TabletPC *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_INITDIALOG: {
      SetWindowLongPtr(hwnd,GWLP_USERDATA,lParam);
      p = (TabletPC *)GetWindowLongPtr(hwnd,GWLP_USERDATA);
      SetWindowLongPtr(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_COLOR_SWATCH),GWLP_USERDATA,(LONG_PTR)p);
      TabletPC::defaultSwatchHandler = (WNDPROC)SetWindowLongPtr(GetDlgItem(hwnd,IDDI_INK_PROPERTIES_COLOR_SWATCH),GWLP_WNDPROC,(LONG_PTR)TabletPC::swatchHandler);
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
      AdjustWindowRect(&rcAdjust,GetWindowLongPtr(hwnd,GWL_STYLE),FALSE);

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
      return LRESULT(FALSE);

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

   
   LRESULT CALLBACK TabletPC::swatchHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   TabletPC *p = (TabletPC *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

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

   return CallWindowProc(TabletPC::defaultSwatchHandler,hwnd,msg,wParam,lParam);
   }



   LRESULT CALLBACK TabletPC::redTextHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam) {

   TabletPC *p = (TabletPC *)GetWindowLongPtr(hwnd,GWLP_USERDATA);

   switch ( msg ) {

   case WM_PAINT: {
      PAINTSTRUCT ps = {0};
      BeginPaint(hwnd,&ps);
      char szText[1024];
      GetWindowText(hwnd,szText,1024);
      HFONT hGUIFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SelectObject(ps.hdc,hGUIFont);
      SetTextColor(ps.hdc,RGB(255,0,0));
      SetBkColor(ps.hdc,GetSysColor(COLOR_WINDOW));
      FillRect(ps.hdc,&ps.rcPaint,(HBRUSH)(COLOR_WINDOW + 1));
      DrawText(ps.hdc,szText,strlen(szText),&ps.rcPaint,DT_TOP);
      EndPaint(hwnd,&ps);
      }
      break;

   default:
      break;

   }

   return CallWindowProc(TabletPC::defaultTextHandler,hwnd,msg,wParam,lParam);
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
   long id = GetWindowLongPtr(hwndTest,GWL_ID);
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
   long id = GetWindowLongPtr(hwndTest,GWL_ID);
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
