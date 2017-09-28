
#pragma once

#include <windows.h>
#include <wingdi.h>

#include <stdio.h>
#include <objbase.h>
#include <olectl.h>

#define _MIDL_USE_GUIDDEF_

#include <msinkaut.h>

#include "utilities.h"

#include "..\resource.h"
#include "resource.h"
#include "signaturePadModelIds.h"

#include "PDFEnabler_i.h"
#include "Properties_i.h"
#include "SignaturePad_i.h"
#include "PrintingSupport_i.h"
#include "CursiVision_i.h"

//#import "InkObj.dll" no_namespace exclude("tagXFORM")

HRESULT registerComponent(long resourceId,HMODULE);

class __declspec(dllexport) TabletPC : public ISignaturePad {

   public:

      TabletPC(IUnknown *pUnkOuter);

      ~TabletPC();

      //   IUnknown

      STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
      STDMETHOD_ (ULONG, AddRef)();
      STDMETHOD_ (ULONG, Release)();

   private:

      long __stdcall TakeFrameWindow(HWND hwnd) { hwndFrameWindow = hwnd; return S_OK; };

      long __stdcall HostHandle(HWND);

      long __stdcall Load(char *pszOPOSName,HWND hwndHost,void *pvICursiVisionServices);
      long __stdcall Start();

      long __stdcall Unload();
      long __stdcall Stop();

      BOOL __stdcall IsActive() { return true; };

      long __stdcall Width() { if ( 0 < (rcClient.right - rcClient.left) ) return rcClient.right - rcClient.left; RECT rc; GetWindowRect(hwndClient,&rc); return rc.right - rc.left; };
      long __stdcall Height() { if ( 0 < (rcClient.bottom - rcClient.top) ) return rcClient.bottom - rcClient.top; RECT rc; GetWindowRect(hwndClient,&rc); return rc.bottom - rc.top; };

      long __stdcall LCDWidth() { return Width(); };
      long __stdcall LCDHeight() { return Height(); };

      double __stdcall WidthInInches() { return padWidthInInches; };
      double __stdcall HeightInInches() { return padHeightInInches; };

      double __stdcall PadToLCDScaleX() { return 1.0; };
      double __stdcall PadToLCDScaleY() { return 1.0; };

      double __stdcall PadTimeout() { return signatureTimeOutPeriod; };

      double __stdcall SetZoomFactor(double zoomFactor) { return 1.0; };

      void __stdcall PrepareForPage(RECT *pRect);

      void __stdcall DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad = FALSE,BOOL isLastPage = TRUE);
      void __stdcall DisplaySignatureBitmapHandle(HBITMAP hBitmap,HDC hdc,long x,long y,long cx,long cy);
      void __stdcall OverlaySignatureBitmapHandle(HBITMAP hBitmap,HDC hdc,long x,long y,long cx,long cy);

      void __stdcall DisplayHotSpotFile(char *pszFile,long eventID,long x,long y,long cx,long cy);
      void __stdcall DisplayHotSpotHandle(HBITMAP hBitmap,HDC hdc,long eventID,long x,long y,long cx,long cy);
      void __stdcall DisplayOk(long eventID);
      void __stdcall DisplayClear(long eventID);
      void __stdcall DisplayCancel(long eventID);

      short __stdcall KeyPadQueryHotSpot(short keyCode) { return 0; };

      void __stdcall SetBackgroundFile(BSTR bstrFileName);
      void __stdcall SetBackgroundHandle(OLE_HANDLE hBitmap) { return; };

      char * __stdcall DeviceName() { return signatureDeviceName; };
      char * __stdcall DeviceModel() { return signatureDeviceModel; };
      char * __stdcall DeviceProductName() { return signatureDeviceProductName; };
      long __stdcall IsLCD() { return 0L; };
      long __stdcall IsFullPage() { return 1L; };
      long __stdcall HasScaling() { return 0L; };
      long __stdcall IsTabletPC() { return 1L; };

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

      long __stdcall WriteParameters(char *pszFileName);

      long __stdcall MaximumX();
   
      long __stdcall MaximumY();

      long __stdcall MinimumX();
   
      long __stdcall MinimumY();

      long __stdcall MaximumSignableY();

      long __stdcall FireOption(long optionNumber);

      BOOL __stdcall CanSaveSignatureValidationData() { return FALSE;};

      BOOL __stdcall CaptureSignatureValidationData(BOOL) { return E_NOTIMPL; };

      long __stdcall AddKeyData(BSTR) { return E_NOTIMPL; };

      long __stdcall SaveSignatureValidationData(BSTR) { return E_NOTIMPL; };

      long __stdcall ImportSignature(BSTR) { return E_NOTIMPL; };

      long __stdcall ExportSignature(BSTR) { return E_NOTIMPL; };

      long __stdcall GetSignature(BSTR *) { return E_NOTIMPL; };

      long __stdcall SetSignature(BSTR) { return E_NOTIMPL; };

      long __stdcall OriginPointX() { return 0L; };
   
      long __stdcall OriginPointY() { return 0L; };

      long __stdcall BaselinePointX() { return 0L; };
   
      long __stdcall BaselinePointY() { return 0L; };

      DWORD __stdcall InkColor() { return inkColor; };

      long __stdcall InkWeight() { return penWidth; };

      long __stdcall ShowProperties();

      //   IPropertiesClient

      class _IGPropertiesClient : public IGPropertiesClient {

      public:
   
            _IGPropertiesClient(TabletPC *pp) : pParent(pp), refCount(0) {};
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

            TabletPC *pParent;
            long refCount;

      } *pIGPropertiesClient;

      // Other methods ?

      void __stdcall PutLCDCaptureMode(long mode) { return; }; //pIDSigPlus -> PutLCDCaptureMode(mode); };
      void __stdcall LCDRefresh(long Mode,long XPos,long YPos,long XSize,long YSize) { return ; }; //pIDSigPlus -> LCDRefresh((short)Mode,(short)XPos,(short)YPos,(short)XSize,(short)YSize); };

      BOOL Configure();

      void SaveBitmapFile(HDC hdcSource,HBITMAP hBitmap,long bitCount,char *pszFileName);
      void SaveJPEGFile(HDC hdcSource,HBITMAP hBitmap,long bitCount,char *pszFileName);

      void SetPadParameters();

      // IOleWindow
   
      STDMETHOD (GetWindow)(HWND *);
      STDMETHOD (ContextSensitiveHelp)(BOOL);

      // IOleInPlaceObject

      STDMETHOD (InPlaceActivate)();
      STDMETHOD (InPlaceDeactivate)();
      STDMETHOD (UIDeactivate)();
      STDMETHOD (SetObjectRects)(LPCRECT,LPCRECT);
      STDMETHOD (ReactivateAndUndo)();

      // IOleObject 

      class __declspec(dllexport) _IOleObject : public IOleObject {

      public:

         _IOleObject(TabletPC* pp) : pParent(pp) {};
         ~_IOleObject() {};

         STDMETHOD(QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

      private:

         STDMETHOD(SetClientSite)(IOleClientSite *pClientSite);
         STDMETHOD(GetClientSite)(IOleClientSite **ppCLientSite);
         STDMETHOD(SetHostNames)(LPCOLESTR szContainerApp,LPCOLESTR szContainerObj);
         STDMETHOD(Close)(DWORD dwSaveOption);
         STDMETHOD(SetMoniker)(DWORD dwWhichMoniker, IMoniker *pmk);
         STDMETHOD(GetMoniker)(DWORD dwAssign, DWORD dwWhichMoniker,IMoniker **ppmk);
         STDMETHOD(InitFromData)(IDataObject *pDataObject,BOOL fCreation,DWORD dwReserved);
         STDMETHOD(GetClipboardData)(DWORD dwReserved,IDataObject **ppDataObject);
         STDMETHOD(DoVerb)(LONG iVerb,LPMSG lpmsg,IOleClientSite *pActiveSite,LONG lindex,HWND hwndParent,LPCRECT lprcPosRect);
         STDMETHOD(EnumVerbs)(IEnumOLEVERB **ppenumOleVerb);
         STDMETHOD(Update)();
         STDMETHOD(IsUpToDate)();
         STDMETHOD(GetUserClassID)(CLSID * pClsid);
         STDMETHOD(GetUserType)(DWORD dwFormOfType, LPOLESTR *pszUserType);
         STDMETHOD(SetExtent)(DWORD dwDrawAspect, LPSIZEL lpsizel);
         STDMETHOD(GetExtent)(DWORD dwDrawAspect, LPSIZEL lpsizel);
        
         STDMETHOD(Advise)(IAdviseSink *pAdvSink, DWORD * pdwConnection);
         STDMETHOD(Unadvise)(DWORD dwConnection);
         STDMETHOD(EnumAdvise)(IEnumSTATDATA **ppenumAdvise);
         STDMETHOD(GetMiscStatus)(DWORD dwAspect, DWORD * pdwStatus);        
        
         STDMETHOD(SetColorScheme)(LPLOGPALETTE lpLogpal);
   
         TabletPC *pParent;

      } *pIOleObject;

      // IOleClientSite

      class __declspec(dllexport) _IOleClientSite : public IOleClientSite {

      public:

         _IOleClientSite(TabletPC *p,IOleInPlaceFrame *pFrame,IOleInPlaceSite *pIPSite) : 
                              pParent(p), pIOleInPlaceFrame(pFrame), pIOleInPlaceSite(pIPSite), refCount(0) {};
         ~_IOleClientSite() {};

         //   IUnknown

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

         long _stdcall SaveObject();
         long _stdcall GetMoniker(DWORD,DWORD,IMoniker**);
         long _stdcall GetContainer(IOleContainer**);
         long _stdcall ShowObject();
         long _stdcall OnShowWindow(BOOL);
         long _stdcall RequestNewObjectLayout();

         TabletPC *pParent;
         IOleInPlaceFrame *pIOleInPlaceFrame;
         IOleInPlaceSite *pIOleInPlaceSite;

         long refCount;

      } *pIOleClientSite;

      // IOleInPlaceSite

      class __declspec(dllexport) _IOleInPlaceSite : public IOleInPlaceSite {

      public:

         _IOleInPlaceSite(TabletPC* pp,HWND hostWindow,IOleInPlaceFrame *pFrame) :
                     pParent(pp), hwndHost(hostWindow), pIOleInPlaceFrame(pFrame), refCount(0) {};

         STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

      private:

         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

         STDMETHOD(GetWindow)(HWND*);
         STDMETHOD(ContextSensitiveHelp)(BOOL);
         STDMETHOD(CanInPlaceActivate)();
         STDMETHOD(OnInPlaceActivate)();
         STDMETHOD(OnUIActivate)();
         STDMETHOD(GetWindowContext)(IOleInPlaceFrame**,IOleInPlaceUIWindow**,RECT* position,RECT* clip,OLEINPLACEFRAMEINFO*);
         STDMETHOD(Scroll)(SIZE);
         STDMETHOD(OnUIDeactivate)(BOOL);
         STDMETHOD(OnInPlaceDeactivate)();
         STDMETHOD(DiscardUndoState)();
         STDMETHOD(DeactivateAndUndo)();
         STDMETHOD(OnPosRectChange)(const RECT*);

      protected:

         TabletPC* pParent;
         HWND hwndHost;
         IOleInPlaceFrame *pIOleInPlaceFrame;

         long refCount;

      } * pIOleInPlaceSite;

      class __declspec(dllexport) _IOleInPlaceFrame : public IOleInPlaceFrame {

      public:

         _IOleInPlaceFrame(TabletPC* pp) : pParent(pp), refCount(0) {};

         STDMETHOD(QueryInterface)(REFIID riid,void **ppv);

      private:

         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

         STDMETHOD(GetWindow)(HWND *);
         STDMETHOD(ContextSensitiveHelp)(BOOL);
         STDMETHOD(GetBorder)(RECT *);
         STDMETHOD(RequestBorderSpace)(LPCBORDERWIDTHS);
         STDMETHOD(SetBorderSpace)(LPCBORDERWIDTHS);
         STDMETHOD(SetActiveObject)(IOleInPlaceActiveObject *,LPCOLESTR);
         STDMETHOD(InsertMenus)(HMENU,OLEMENUGROUPWIDTHS *);
         STDMETHOD(SetMenu)(HMENU,HOLEMENU,HWND);
         STDMETHOD(RemoveMenus)(HMENU);
         STDMETHOD(SetStatusText)(LPCOLESTR);
         STDMETHOD(EnableModeless)(BOOL);
         STDMETHOD(TranslateAccelerator)(MSG *,WORD);

      protected:

         TabletPC* pParent;

         long refCount;

      } * pIOleInPlaceFrame;

//      IConnectionPointContainer

     struct __declspec(dllexport) _IConnectionPointContainer : public IConnectionPointContainer {

     public:

        STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
        STDMETHOD_ (ULONG, AddRef)();
        STDMETHOD_ (ULONG, Release)();

        STDMETHOD(FindConnectionPoint)(REFIID riid,IConnectionPoint **);
        STDMETHOD(EnumConnectionPoints)(IEnumConnectionPoints **);

        _IConnectionPointContainer(TabletPC *pp);
		  ~_IConnectionPointContainer();

        void fire_PenUp();
        void fire_PenDown();
        void fire_PenPoint(long x,long y);
        void fire_OptionSelected(long optionNumber);

     private:

		  TabletPC *pParent;

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

        _IConnectionPoint(TabletPC *pp);
		  ~_IConnectionPoint();
		  IUnknown *AdviseSink() { return adviseSink; };

     private:

        int getSlot();
        int findSlot(DWORD dwCookie);

		  IUnknown *adviseSink;
		  TabletPC *pParent;
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

	     _IEnumConnectionPoints(TabletPC *pp,_IConnectionPoint **cp,int connectionPointCount);
       ~_IEnumConnectionPoints();

     private:

        int cpCount,enumeratorIndex;
		  TabletPC *pParent;
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

         _IGPropertyPageClient(TabletPC *p) : pParent(p),refCount(0) {};
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

         TabletPC *pParent;
         long refCount;

      } *pIGPropertyPageClient;

   private:

      class IInkCollectorEvents : _IInkCollectorEvents {

      public:

         IInkCollectorEvents(TabletPC *pp) : pParent(pp), refCount(0), strokeCount(1L), oldStrokeNumber(-1L) {};

         //   IUnknown

         STDMETHOD (QueryInterface)(REFIID riid,void **ppv);
         STDMETHOD_ (ULONG, AddRef)();
         STDMETHOD_ (ULONG, Release)();

         // IDispatch

         STDMETHOD(GetTypeInfoCount)(UINT *pctinfo);
         STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo **pptinfo);
         STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgdispid);
         STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pdispparams, VARIANT *pvarResult, EXCEPINFO *pexcepinfo, UINT *puArgErr);

         // _IInkCollectorEvents

         STDMETHOD(Stroke)(IInkCursor * Cursor,IInkStrokeDisp * Stroke,VARIANT_BOOL * Cancel );

         STDMETHOD(CursorDown)(IInkCursor * Cursor,IInkStrokeDisp * Stroke );

         STDMETHOD(NewPackets)(IInkCursor * Cursor,IInkStrokeDisp * Stroke,long PacketCount,VARIANT * PacketData );

         STDMETHOD(DblClick)(VARIANT_BOOL * Cancel );

         STDMETHOD(MouseMove)(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long pX,long pY,VARIANT_BOOL * Cancel );

         STDMETHOD(MouseDown)(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long pX,long pY,VARIANT_BOOL * Cancel );

         STDMETHOD(MouseUp)(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long pX,long pY,VARIANT_BOOL * Cancel );

         STDMETHOD(MouseWheel)(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long Delta,long x,long y,VARIANT_BOOL * Cancel );

         STDMETHOD(NewInAirPackets)(IInkCursor * pCursor,long packetCount,VARIANT * pPacketData );

         STDMETHOD(CursorButtonDown)(IInkCursor * pCursor,IInkCursorButton * pButton );

         STDMETHOD(CursorButtonUp)(IInkCursor * pCursor,IInkCursorButton * pButton );

         STDMETHOD(CursorInRange)(IInkCursor * pCursor,VARIANT_BOOL NewCursor,VARIANT & ButtonsState );

         STDMETHOD(CursorOutOfRange)(IInkCursor * pCursor );

         STDMETHOD(SystemGesture)(IInkCursor * Cursor,InkSystemGesture Id,long x,long y,long Modifier,BSTR Character,long CursorMode );

         STDMETHOD(Gesture)(IInkCursor * Cursor,IInkStrokes * Strokes,VARIANT & Gestures,VARIANT_BOOL * Cancel );

         STDMETHOD(TabletAdded)(IInkTablet * Tablet );

         STDMETHOD(TabletRemoved)(long TabletId );

         TabletPC *pParent;

         long refCount;
         long strokeCount;
         long oldStrokeNumber;

      } *pIInkCollectorEvents;


      long initializeWindows(HWND hwndParent);
      void initializeParameters();

      void Resized(RECT *);

      void instantiateSigPlus();

      char signatureDeviceName[MAX_PATH];
      char signatureDeviceModel[MAX_PATH];
      char signatureDeviceProductName[MAX_PATH];

      char modelType[MAX_PATH];
      char szConfigurationFile[MAX_PATH];
      char deviceDescription[MAX_PATH];

      long modelId;

      int startPadParameters;

      int endPadParameters;

      double padWidthInInches,padHeightInInches;
      double signatureTimeOutPeriod;
      long penWidth;

      DWORD inkColor;

      RECT rectDisplay,rcClient;

      HWND hwndSigPlusFrame,hwndSigPlusControlHost;
      HWND hwndSettings;
      HWND hwndBitmapDepth;

      HWND hwndClient;
      HWND hwndFrameWindow;

      HDC hdcClient;
      long refCount;

      ICursiVisionServices *pICursiVisionServices;

      IGProperties *pIGProperties;
      IInkCollector *pIInkCollector;
      IInkRenderer *pIInkRenderer;

      double *pSignatureDataX, *pSignatureDataY;
      double scaleToPadX,scaleToPadY;
      double padWidthTopazUnits,padHeightTopazUnits;

      static char szBadConfigurationOption[MAX_PATH];

      static LRESULT CALLBACK frameHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK settingsHandler(HWND,UINT,WPARAM,LPARAM);

      static LRESULT CALLBACK inkHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK swatchHandler(HWND,UINT,WPARAM,LPARAM);
      static LRESULT CALLBACK redTextHandler(HWND,UINT,WPARAM,LPARAM);

      static WNDPROC defaultSwatchHandler;
      static WNDPROC defaultTextHandler;

      IOleClientSite *pIOleClientSite_MySite;
      IOleInPlaceSite *pIOleInPlaceSite_MySite;

      IConnectionPointContainer* pIConnectionPointContainer;
      IConnectionPoint *pIConnectionPoint;
      DWORD dwConnectionCookie;

      friend class _IGPropertiesClient;
      friend class _IGPropertyPageClient;
      friend class _IOleObject;
      friend class _IOleClientSite;
      friend class _IOleInPlaceSite;
      friend class _IOleInPlaceFrame;
      friend struct _IConnectionPointContainer;

};

void SaveBitmapFile(HDC hdcContainingBitmap,HBITMAP hBitmap,long bitCount,char *pszFileName,char *pszNameQualifier = NULL);

#ifdef DEFINE_DATA

   char szModuleName[MAX_PATH];
   char szApplicationDataDirectory[MAX_PATH];
   char szUserDirectory[MAX_PATH];
   HINSTANCE hModule;

#else

   extern char szModuleName[];
   extern char szApplicationDataDirectory[];
   extern char szUserDirectory[];
   extern HINSTANCE hModule;

#endif
      static LRESULT CALLBACK redTextHandler(HWND,UINT,WPARAM,LPARAM);

      static WNDPROC defaultSwatchHandler;
      static WNDPROC defaultTextHandler;
