
#pragma once

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <objbase.h>
#include <list>
#include <process.h>

//#include <wingdi.h>

#include "resource.h"
#include "signaturePadModelIds.h"

#include "PDFEnabler_i.h"
#include "Properties_i.h"
#include "SignaturePad_i.h"
#include "PrintingSupport_i.h"
#include "CursiVision_i.h"

#include "STPadLib.h"

#define MODEL_ID_SIGMA 1
#define MODEL_ID_OMEGA 2

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
      double __stdcall HeightInInches() { 
         return padWidthInInches * ((double)Height() / (double)Width()); 
      };

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
      char * __stdcall DeviceProductName() { return STR_SIGNOTEC_MODEL; };

      long __stdcall IsLCD() { return true; };
      long __stdcall IsFullPage() { return false; };
      long __stdcall HasScaling() { return true; };
      long __stdcall IsTabletPC() { return false; };

      void __stdcall GetRect(RECT *);
      void __stdcall GetHotSpot(long hotSpotNumber,RECT *);

      long __stdcall ClearInk();

      long __stdcall SignatureData();
      double * __stdcall SignatureDataX() { return NULL; };
      double * __stdcall SignatureDataY() { return NULL; };
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

      BOOL Configure();

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

        void fire_PenUp(long x,long y);
        void fire_PenDown(long x,long y);
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

         STDMETHODIMP QueryInterface(REFIID, void **);
         STDMETHODIMP_(ULONG) AddRef();
         STDMETHODIMP_(ULONG) Release();
         STDMETHODIMP Next(ULONG, CONNECTDATA*, ULONG*);
         STDMETHODIMP Skip(ULONG);
         STDMETHODIMP Reset();
         STDMETHODIMP Clone(IEnumConnections**);

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

      char signatureDeviceName[MAX_PATH];
      char signatureDeviceModel[MAX_PATH];
      char signatureDeviceProductName[MAX_PATH];
      char szOKFile[MAX_PATH],szClearFile[MAX_PATH];

      char modelType[MAX_PATH];
      char deviceDescription[MAX_PATH];

      long modelId;
      bool modelHasScaling;

      int startPadParameters;

      int OKXPos,OKYPos,OKXSize,OKYSize;
      int ClearXPos,ClearYPos,ClearXSize,ClearYSize;
      int OKHotSpotXPos,OKHotSpotYPos,OKHotSpotXSize,OKHotSpotYSize;
      int ClearHotSpotXPos,ClearHotSpotYPos,ClearHotSpotXSize,ClearHotSpotYSize;
      int SigXPos,SigYPos,SigXSize,SigYSize;
      int InkXPos,InkYPos,InkXSize,InkYSize;

      int endPadParameters;

      POINTL originPoint;
      POINTL baselinePoint;

      BOOL isActive;
      BOOL isLoaded;
      BOOL penIsDown;

      long signaturePadCount;
      VARIANT_BOOL tabletComTest;

      double padWidthInInches;
      double signatureTimeOutPeriod;

      long penWidth;
      long startPointX,startPointY;
      long endPointX,endPointY;
      long padBitsPerPixel;
      DWORD inkColor;

      HWND hwndSettings;
      HWND hwndFrameWindow;

      long refCount;

      ICursiVisionServices *pICursiVisionServices;

      IGProperties *pIGProperties;

      double scaleToPadX,scaleToPadY;
      long maximumPenUnitsWidth,maximumPenUnitsHeight;
      
      std::list<hotSpot *> hotSpots;

      IConnectionPointContainer* pIConnectionPointContainer;
      IConnectionPoint *pIConnectionPoint;
      DWORD dwConnectionCookie;

      static void signatureActivityMonitor(long eventType,void *pData,long dataSize,void *pParameter);

      static LRESULT CALLBACK settingsHandler(HWND,UINT,WPARAM,LPARAM);

      static LRESULT CALLBACK originPointHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK originPointPadImageHandler(HWND,UINT,WPARAM,LPARAM);

      static LRESULT CALLBACK inkHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK swatchHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK redTextHandler(HWND,UINT,WPARAM,LPARAM);

      static WNDPROC defaultSwatchHandler;
      static WNDPROC defaultTextHandler;

};

#ifdef DEFINE_DATA

   char szModuleName[MAX_PATH];
   char szApplicationDataDirectory[MAX_PATH];
   HINSTANCE hModule;
   BOOL eventsAllowed;

#else

   extern char szModuleName[];
   extern char szApplicationDataDirectory[];
   extern HINSTANCE hModule;
   extern BOOL eventsAllowed;

#endif
