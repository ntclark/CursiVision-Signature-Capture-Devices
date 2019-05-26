
#pragma once
#pragma warning( disable : 4355)

#include <windows.h>
#include <olectl.h>
#include <process.h>

#include <stdio.h>
#include <objbase.h>
#include <list>

#include <gdiPlus.h>
#include <wingdi.h>

#include "..\..\resource.h"
#include "resource.h"
#include "signaturePadModelIds.h"

#include "PDFEnabler_i.h"
#include "Properties_i.h"
#include "SignaturePad_i.h"
#include "Wacom.h"

#include "PrintingSupport_i.h"
#include "CursiVision_i.h"

#define STU_300     1
#define STU_520     2
#define STU_500     3

#define CAPTURE_SIG_DATA
#define MAX_KEY_ENTRIES 32

using namespace Gdiplus;

#define MONOCHROME_IMAGE_BITS 8
#define IS_COLOR_PAD false
#define PREFERRED_COLOR_IMAGE_BIT_DEPTH   24

struct hotSpot {
   RECT rcLocation;
   long eventId;
};

class SignaturePad : public ISignaturePad {

   public:

      SignaturePad(IUnknown *pUnkOuter);
      ~SignaturePad();

      //   IUnknown

      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

   private:

      long __stdcall TakeFrameWindow(HWND hwnd) { hwndFrameWindow = hwnd; return S_OK; };

      long __stdcall HostHandle(HWND hwnd) { return 0; };

      long __stdcall Load(char *pszOPOSName,HWND hwndClientHost,void *pvICursiVisionServices) { return loaded(pszOPOSName,hwndClientHost,pvICursiVisionServices); };
      long __stdcall Start();

      long __stdcall Unload() { return unload(); };
      long __stdcall Stop();

      BOOL __stdcall IsActive() { return isActive; };

      long __stdcall Width();
      long __stdcall Height();
      long __stdcall LCDWidth();
      long __stdcall LCDHeight();

      double __stdcall WidthInInches() { return padWidthInInches; };
      double __stdcall HeightInInches() { return padWidthInInches * ((double)Height() / (double)Width()); };

      double __stdcall PadToLCDScaleX();
      double __stdcall PadToLCDScaleY();

      double __stdcall PadTimeout() { return signatureTimeOutPeriod; };

      double __stdcall SetZoomFactor(double zoomFactor);

      void __stdcall PrepareForPage(RECT *pRect);

      void __stdcall DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad = FALSE,BOOL isLastPage = TRUE);
      void __stdcall DisplaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy);
      void __stdcall OverlaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy);

      void __stdcall DisplayHotSpotFile(char *pszFile,long eventID,long x,long y,long cx,long cy);
      void __stdcall DisplayHotSpotHandle(UINT_PTR hBitmap,HDC hdc,long eventID,long x,long y,long cx,long cy);
      void __stdcall DisplayOk(long eventID);
      void __stdcall DisplayClear(long eventID);
      void __stdcall DisplayCancel(long eventID);

      short __stdcall KeyPadQueryHotSpot(short keyCode);

      void __stdcall SetBackgroundFile(BSTR bstrFileName);
      void __stdcall SetBackgroundHandle(OLE_HANDLE hBitmap);

      char * __stdcall DeviceName() { return signatureDeviceName; };
      char * __stdcall DeviceModel() { return signatureDeviceModel; };
      char * __stdcall DeviceProductName() { return signatureDeviceProductName; };

      long __stdcall IsLCD() { return true; };
      long __stdcall IsFullPage() { return false; };
      long __stdcall HasScaling() { return true; };
      long __stdcall IsTabletPC() { return false; };

      void __stdcall GetRect(RECT *);
      void __stdcall GetHotSpot(long hotSpotNumber,RECT *);

      long __stdcall ClearInk();

      long __stdcall SignatureData();
      double * __stdcall SignatureDataX() { return pSignatureDataX; };
      double * __stdcall SignatureDataY() { return pSignatureDataY; };
      void __stdcall ClearSignatureData();
      void __stdcall DeleteSignatureData();

      void __stdcall ClearTablet();
      void __stdcall TabletState(long ts);
      BOOL __stdcall EnableTablet();
      BOOL __stdcall DisableTablet();

      void __stdcall SetSigWindow();
      void __stdcall SetInkArea();

      long __stdcall GetPixelBits(HDC hdc);

      long __stdcall MaximumX();
   
      long __stdcall MaximumY();

      long __stdcall MinimumX();
   
      long __stdcall MinimumY();

      long __stdcall MaximumSignableY();

      long __stdcall FireOption(long optionNumber);

      BOOL __stdcall CanSaveSignatureValidationData() { return FALSE; };

      BOOL __stdcall CaptureSignatureValidationData(BOOL) { return E_NOTIMPL; };
      long __stdcall AddKeyData(BSTR) { return E_NOTIMPL; };
      long __stdcall SaveSignatureValidationData(BSTR) { return E_NOTIMPL; };
      long __stdcall ImportSignature(BSTR) { return E_NOTIMPL; };
      long __stdcall ExportSignature(BSTR) { return E_NOTIMPL; };
      long __stdcall GetSignature(BSTR *pBstrClientFree) { return E_NOTIMPL; };
      long __stdcall SetSignature(BSTR) { return E_NOTIMPL; };

      long __stdcall OriginPointX() { return originPoint.x; };

      long __stdcall OriginPointY() { return originPoint.y; };

      long __stdcall BaselinePointX() { return baselinePoint.x; };

      long __stdcall BaselinePointY() { return baselinePoint.y; };

      DWORD __stdcall InkColor() { return inkColor; };

      long __stdcall InkWeight() { return penWidth; };

      long __stdcall ShowProperties();

      private:

      //   IPropertiesClient

      class _IGPropertiesClient : public IGPropertiesClient {

      public:
   
            _IGPropertiesClient(SignaturePad *pp) : pParent(pp), refCount(0) {};
            ~_IGPropertiesClient() {};

            //   IUnknown
      
            STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
            STDMETHOD_ (ULONG, AddRef)();
            STDMETHOD_ (ULONG, Release)();
   
            STDMETHOD(SavePrep)();
            STDMETHOD(InitNew)();
            STDMETHOD(Loaded)();
            STDMETHOD(Saved)();
            STDMETHOD(IsDirty)();
            STDMETHOD(GetClassID)(BYTE *pCLSID);

      private:

            SignaturePad *pParent;
            long refCount;

      } *pIGPropertiesClient;

      // Other methods ?

      void __stdcall PutLCDCaptureMode(long mode);
      void __stdcall LCDRefresh(long Mode,long XPos,long YPos,long XSize,long YSize);

      long SetOKXPos(long v) { OKXPos = v; return v; };
      long SetOKYPos(long v) { OKYPos = v; return v; };
      long SetOKXSize(long v) { OKXSize = v; return v; };
      long SetOKYSize(long v) { OKYSize = v; return v; };
      
      long SetClearXPos(long v) { ClearXPos = v; return v; };
      long SetClearYPos(long v) { ClearYPos = v; return v; };
      long SetClearXSize(long v) { ClearXSize = v; return v; };
      long SetClearYSize(long v) { ClearYSize = v; return v; };
      
      long SetOKHotSpotXPos(long v) { OKHotSpotXPos = v; return v; };
      long SetOKHotSpotYPos(long v) { OKHotSpotYPos = v; return v; };
      long SetOKHotSpotXSize(long v) { OKHotSpotXSize = v; return v; };
      long SetOKHotSpotYSize(long v) { OKHotSpotYSize = v; return v; };
      
      long SetClearHotSpotXPos(long v) { ClearHotSpotXPos = v; return v; };
      long SetClearHotSpotYPos(long v) { ClearHotSpotYPos = v; return v; };
      long SetClearHotSpotXSize(long v) { ClearHotSpotXSize = v; return v; };
      long SetClearHotSpotYSize(long v) { ClearHotSpotYSize = v; return v; };

      long SetSigXPos(long v) { SigXPos = v; return v; };
      long SetSigYPos(long v) { SigYPos = v; return v; };
      long SetSigXSize(long v) { SigXSize = v; return v; };
      long SetSigYSize(long v) { SigYSize = v; return v; };

      long SetInkXPos(long v) { InkXPos = v; return v; };
      long SetInkYPos(long v) { InkYPos = v; return v; };
      long SetInkXSize(long v) { InkXSize = v; return v; };
      long SetInkYSize(long v) { InkYSize = v; return v; };

//      IConnectionPointContainer

     struct __declspec(dllexport) _IConnectionPointContainer : public IConnectionPointContainer {

     public:

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD(FindConnectionPoint)(REFIID riid,IConnectionPoint **);
        STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **);

        _IConnectionPointContainer(SignaturePad *pp);
        ~_IConnectionPointContainer();

        void fire_PenUp();
        void fire_PenDown();
        void fire_PenPoint(long x,long y,float inkWeight);
        void fire_OptionSelected(long optionNumber);

     private:

        SignaturePad *pParent;

     } connectionPointContainer;


     struct __declspec(dllexport) _IConnectionPoint : IConnectionPoint {

     public:

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD (GetConnectionInterface)(IID *);
        STDMETHOD (GetConnectionPointContainer)(IConnectionPointContainer **ppCPC);
        STDMETHOD (Advise)(IUnknown *pUnk,DWORD *pdwCookie);
        STDMETHOD (Unadvise)(DWORD);
        STDMETHOD (EnumConnections)(IEnumConnections **ppEnum);

        _IConnectionPoint(SignaturePad *pp);
        ~_IConnectionPoint();
        IUnknown *AdviseSink() { return adviseSink; };

     private:

        int getSlot();
        int findSlot(DWORD dwCookie);

        IUnknown *adviseSink;
        SignaturePad *pParent;
        DWORD nextCookie;
        int countConnections,countLiveConnections;

        CONNECTDATA *connections;

     } connectionPoint;


     struct __declspec(dllexport) _IEnumConnectionPoints : IEnumConnectionPoints {

     public:

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD (Next)(ULONG cConnections,IConnectionPoint **rgpcn,ULONG *pcFetched);
        STDMETHOD (Skip)(ULONG cConnections);
        STDMETHOD (Reset)();
        STDMETHOD (Clone)(IEnumConnectionPoints **);

        _IEnumConnectionPoints(SignaturePad *pp,_IConnectionPoint **cp,int connectionPointCount);
       ~_IEnumConnectionPoints();

     private:

        int cpCount,enumeratorIndex;
        SignaturePad *pParent;
        _IConnectionPoint **connectionPoints;

     } *enumConnectionPoints;


     struct __declspec(dllexport) _IEnumerateConnections : public IEnumConnections {

     public:

        _IEnumerateConnections(IUnknown* pParentUnknown,ULONG cConnections,CONNECTDATA* paConnections,ULONG initialIndex);
        ~_IEnumerateConnections();

         STDMETHODIMP         QueryInterface(REFIID, void **);
         STDMETHODIMP_(ULONG) AddRef();
         STDMETHODIMP_(ULONG) Release();
         STDMETHODIMP         Next(ULONG, CONNECTDATA*, ULONG*);
         STDMETHODIMP         Skip(ULONG);
         STDMETHODIMP         Reset();
         STDMETHODIMP         Clone(IEnumConnections**);

      private:
         ULONG refCount;
         IUnknown *pParentUnknown;
         ULONG enumeratorIndex;
         ULONG countConnections;
         CONNECTDATA *connections;

      } *enumConnections;

      //      IPropertyPageClient

      class _IGPropertyPageClient : public IGPropertyPageClient {
      public:

         _IGPropertyPageClient(SignaturePad *p) : pParent(p),refCount(0) {};
         ~_IGPropertyPageClient() {};

         //   IUnknown

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

      private:

         STDMETHOD(BeforeAllPropertyPages)();
         STDMETHOD(GetPropertyPagesInfo)(long* countPages,SAFEARRAY** stringDescriptions,SAFEARRAY** stringHelpDirs,SAFEARRAY** pSize);
         STDMETHOD(CreatePropertyPage)(long,HWND,RECT*,BOOL,HWND *pHwndPropertyPage);
         STDMETHOD(Apply)();
         STDMETHOD(IsPageDirty)(long,BOOL*);
         STDMETHOD(Help)(BSTR bstrHelpDir);
         STDMETHOD(TranslateAccelerator)(long,long*);
         STDMETHOD(AfterAllPropertyPages)(BOOL);
         STDMETHOD(DestroyPropertyPage)(long);

         STDMETHOD(GetPropertySheetHeader)(void *pHeader);
         STDMETHOD(get_PropertyPageCount)(long *pCount);
         STDMETHOD(GetPropertySheets)(void *pSheets);

         SignaturePad *pParent;
         long refCount;

      } *pIGPropertyPageClient;

   private:

      void initializeParameters();

      long loaded(char *pszDeviceName,HWND hwndClientHost,void *pvICursiVisionServices);
      long unload();

      void saveBitmapFile(HBITMAP hBitmap,char *pszFileName);
      HBITMAP createPadBitmap();
      void clearImageBits(long x,long y,long cx,long cy);
      HBITMAP createTemporaryBitmap(long cx,long cy);
      void loadBitmapFromHandle(long x,long y,long cx,long cy,long cxSource,long cySource,HBITMAP theHandle,long operation = SRCCOPY);
      void loadBitmapFromFile(long x,long y,long cx,long cy,long cxSource,long cySource,char *pszFileName,long operation = SRCCOPY);
      void sendBitmapHandle(HBITMAP hBitmap);
      void sendBitmapHandleImmediate(HBITMAP hBitmap);
      char signatureDeviceName[MAX_PATH];
      char signatureDeviceModel[MAX_PATH];
      char signatureDeviceProductName[MAX_PATH];

      char modelType[MAX_PATH];
      char deviceDescription[MAX_PATH];

      long modelId;
      bool modelHasScaling;

      CAPS thePad;

      int startPadParameters;

      int OKXPos,OKYPos,OKXSize,OKYSize;
      int ClearXPos,ClearYPos,ClearXSize,ClearYSize;
      int OKHotSpotXPos,OKHotSpotYPos,OKHotSpotXSize,OKHotSpotYSize;
      int ClearHotSpotXPos,ClearHotSpotYPos,ClearHotSpotXSize,ClearHotSpotYSize;
      int SigXPos,SigYPos,SigXSize,SigYSize;
      int InkXPos,InkYPos,InkXSize,InkYSize;

      int endPadParameters;

      long displayBitsPerPixel;

      GdiplusStartupInput gdiplusStartupInput;
      ULONG_PTR gdiplusToken;

      POINTL originPoint;
      POINTL baselinePoint;

      BOOL isActive;
      BOOL isLoaded;

      double padWidthInInches;
      double signatureTimeOutPeriod;

      long penWidth;
      DWORD inkColor;

      char szOKFile[MAX_PATH],szClearFile[MAX_PATH];

      HWND hwndSettings;
      HWND hwndFrameWindow;

      long refCount;

      ICursiVisionServices *pICursiVisionServices;

      IGProperties *pIGProperties;

      double *pSignatureDataX, *pSignatureDataY;
      double scaleToPadX,scaleToPadY;
      long maximumPenUnitsWidth,maximumPenUnitsHeight;
      
      unsigned int sendingBitmapThreadAddress;
      HANDLE sendingBitmapThreadHandle;

      std::list<hotSpot *> hotSpots;

      static unsigned int __stdcall eventuallySendBitmap(void *p);

      static LRESULT CALLBACK settingsHandler(HWND,UINT,WPARAM,LPARAM);
      static bool CALLBACK signaturePadCallback(PEN pen,void* ptrUser);
      static unsigned int __stdcall fireOptionThreaded(void *);
      static LRESULT CALLBACK originPointHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK originPointPadImageHandler(HWND,UINT,WPARAM,LPARAM);

      static LRESULT CALLBACK inkHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK swatchHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK redTextHandler(HWND,UINT,WPARAM,LPARAM);

      static WNDPROC defaultSwatchHandler;
      static WNDPROC defaultTextHandler;

      IConnectionPointContainer* pIConnectionPointContainer;
      IConnectionPoint *pIConnectionPoint;
      DWORD dwConnectionCookie;

};

#ifdef DEFINE_DATA

   char szModuleName[MAX_PATH];
   char szApplicationDataDirectory[MAX_PATH];
   char szUserDirectory[MAX_PATH];
   HINSTANCE hModule;

   BITMAPINFO padBitmapInfo = {0};
   BITMAPINFO temporaryBitmapInfo = {0};
   BYTE *pPadBitmapBits = NULL;
   HBITMAP hBitmapPad = NULL;

   long padBitmapPixelWidth = 0L;
   long padBitmapPixelHeight = 0L;
   long padBitmapPixelBytes = 0L;

#else

   extern char szModuleName[];
   extern char szApplicationDataDirectory[];
   extern char szUserDirectory[];
   extern HINSTANCE hModule;

   extern BITMAPINFO padBitmapInfo;
   extern BITMAPINFO temporaryBitmapInfo;
   extern BYTE *pPadBitmapBits;
   extern HBITMAP hBitmapPad;

   extern long padBitmapPixelWidth;
   extern long padBitmapPixelHeight;
   extern long padBitmapPixelBytes;


#endif
