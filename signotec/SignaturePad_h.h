

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Feb 05 21:41:30 2014
 */
/* Compiler settings for .\SignaturePad.odl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __SignaturePad_h_h__
#define __SignaturePad_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ISignaturePad_FWD_DEFINED__
#define __ISignaturePad_FWD_DEFINED__
typedef interface ISignaturePad ISignaturePad;
#endif 	/* __ISignaturePad_FWD_DEFINED__ */


#ifndef __ISignaturePadEvents_FWD_DEFINED__
#define __ISignaturePadEvents_FWD_DEFINED__
typedef interface ISignaturePadEvents ISignaturePadEvents;
#endif 	/* __ISignaturePadEvents_FWD_DEFINED__ */


#ifndef __IVisioLoggerSignaturePad_FWD_DEFINED__
#define __IVisioLoggerSignaturePad_FWD_DEFINED__
typedef interface IVisioLoggerSignaturePad IVisioLoggerSignaturePad;
#endif 	/* __IVisioLoggerSignaturePad_FWD_DEFINED__ */


#ifndef __IVisioLoggerSignaturePadEvents_FWD_DEFINED__
#define __IVisioLoggerSignaturePadEvents_FWD_DEFINED__
typedef interface IVisioLoggerSignaturePadEvents IVisioLoggerSignaturePadEvents;
#endif 	/* __IVisioLoggerSignaturePadEvents_FWD_DEFINED__ */


#ifndef __VisioLoggerSignaturePad_FWD_DEFINED__
#define __VisioLoggerSignaturePad_FWD_DEFINED__

#ifdef __cplusplus
typedef class VisioLoggerSignaturePad VisioLoggerSignaturePad;
#else
typedef struct VisioLoggerSignaturePad VisioLoggerSignaturePad;
#endif /* __cplusplus */

#endif 	/* __VisioLoggerSignaturePad_FWD_DEFINED__ */


#ifndef __CursiVisionSignaturePad_FWD_DEFINED__
#define __CursiVisionSignaturePad_FWD_DEFINED__

#ifdef __cplusplus
typedef class CursiVisionSignaturePad CursiVisionSignaturePad;
#else
typedef struct CursiVisionSignaturePad CursiVisionSignaturePad;
#endif /* __cplusplus */

#endif 	/* __CursiVisionSignaturePad_FWD_DEFINED__ */


/* header files for imported files */
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __InnoVisioNateSignotecSignaturePads_LIBRARY_DEFINED__
#define __InnoVisioNateSignotecSignaturePads_LIBRARY_DEFINED__

/* library InnoVisioNateSignotecSignaturePads */
/* [version][lcid][helpstring][uuid] */ 


EXTERN_C const IID LIBID_InnoVisioNateSignotecSignaturePads;

#ifndef __ISignaturePad_INTERFACE_DEFINED__
#define __ISignaturePad_INTERFACE_DEFINED__

/* interface ISignaturePad */
/* [object][unique][nonextensible][helpstring][uuid] */ 


EXTERN_C const IID IID_ISignaturePad;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A64AB7AF-8A26-4f07-8877-56FE99570321")
    ISignaturePad : public IUnknown
    {
    public:
        virtual long STDMETHODCALLTYPE TakeFrameWindow( 
            HWND hwndFrame) = 0;
        
        virtual long STDMETHODCALLTYPE HostHandle( 
            HWND hwnd) = 0;
        
        virtual long STDMETHODCALLTYPE Load( 
            unsigned char *pszOPOSName,
            HWND hwndClientHost,
            void *pvICursiVisionServices) = 0;
        
        virtual long STDMETHODCALLTYPE Start( void) = 0;
        
        virtual long STDMETHODCALLTYPE Unload( void) = 0;
        
        virtual long STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE IsActive( void) = 0;
        
        virtual long STDMETHODCALLTYPE Width( void) = 0;
        
        virtual long STDMETHODCALLTYPE Height( void) = 0;
        
        virtual long STDMETHODCALLTYPE LCDWidth( void) = 0;
        
        virtual long STDMETHODCALLTYPE LCDHeight( void) = 0;
        
        virtual double STDMETHODCALLTYPE WidthInInches( void) = 0;
        
        virtual double STDMETHODCALLTYPE HeightInInches( void) = 0;
        
        virtual double STDMETHODCALLTYPE PadToLCDScaleX( void) = 0;
        
        virtual double STDMETHODCALLTYPE PadToLCDScaleY( void) = 0;
        
        virtual double STDMETHODCALLTYPE PadTimeout( void) = 0;
        
        virtual double STDMETHODCALLTYPE SetZoomFactor( 
            double zoomFactor) = 0;
        
        virtual unsigned char *STDMETHODCALLTYPE DeviceName( void) = 0;
        
        virtual unsigned char *STDMETHODCALLTYPE DeviceProductName( void) = 0;
        
        virtual unsigned char *STDMETHODCALLTYPE DeviceModel( void) = 0;
        
        virtual long STDMETHODCALLTYPE IsLCD( void) = 0;
        
        virtual long STDMETHODCALLTYPE IsFullPage( void) = 0;
        
        virtual long STDMETHODCALLTYPE HasScaling( void) = 0;
        
        virtual long STDMETHODCALLTYPE IsTabletPC( void) = 0;
        
        virtual void STDMETHODCALLTYPE PrepareForPage( 
            RECT *pRect) = 0;
        
        virtual void STDMETHODCALLTYPE DisplaySignatureBitmapFile( 
            unsigned char *pszImageFile,
            /* [defaultvalue][optional] */ BOOL populatePad = 0,
            /* [defaultvalue][optional] */ BOOL isLastPage = 1) = 0;
        
        virtual void STDMETHODCALLTYPE DisplaySignatureBitmapHandle( 
            HBITMAP hBitmap,
            HDC hdc,
            long x,
            long y,
            long cx,
            long cy) = 0;
        
        virtual void STDMETHODCALLTYPE OverlaySignatureBitmapHandle( 
            HBITMAP hBitmap,
            HDC hdc,
            long x,
            long y,
            long cx,
            long cy) = 0;
        
        virtual void STDMETHODCALLTYPE DisplayHotSpotFile( 
            unsigned char *pszFile,
            long eventID,
            long x,
            long y,
            long cx,
            long cy) = 0;
        
        virtual void STDMETHODCALLTYPE DisplayHotSpotHandle( 
            HBITMAP hBitmap,
            HDC hdc,
            long eventID,
            long x,
            long y,
            long cx,
            long cy) = 0;
        
        virtual void STDMETHODCALLTYPE DisplayOk( 
            long eventID) = 0;
        
        virtual void STDMETHODCALLTYPE DisplayClear( 
            long eventID) = 0;
        
        virtual void STDMETHODCALLTYPE DisplayCancel( 
            long eventID) = 0;
        
        virtual short STDMETHODCALLTYPE KeyPadQueryHotSpot( 
            short KeyCode) = 0;
        
        virtual void STDMETHODCALLTYPE SetBackgroundFile( 
            BSTR bstrFileName) = 0;
        
        virtual void STDMETHODCALLTYPE SetBackgroundHandle( 
            OLE_HANDLE __MIDL__ISignaturePad0000) = 0;
        
        virtual void STDMETHODCALLTYPE GetRect( 
            RECT *__MIDL__ISignaturePad0001) = 0;
        
        virtual void STDMETHODCALLTYPE GetHotSpot( 
            long hotSpotNumber,
            RECT *__MIDL__ISignaturePad0002) = 0;
        
        virtual long STDMETHODCALLTYPE SignatureData( void) = 0;
        
        virtual double *STDMETHODCALLTYPE SignatureDataX( void) = 0;
        
        virtual double *STDMETHODCALLTYPE SignatureDataY( void) = 0;
        
        virtual void STDMETHODCALLTYPE ClearSignatureData( void) = 0;
        
        virtual void STDMETHODCALLTYPE DeleteSignatureData( void) = 0;
        
        virtual void STDMETHODCALLTYPE PutLCDCaptureMode( 
            long mode) = 0;
        
        virtual void STDMETHODCALLTYPE TabletState( 
            long __MIDL__ISignaturePad0003) = 0;
        
        virtual BOOL STDMETHODCALLTYPE EnableTablet( void) = 0;
        
        virtual void STDMETHODCALLTYPE ClearTablet( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE DisableTablet( void) = 0;
        
        virtual void STDMETHODCALLTYPE LCDRefresh( 
            long Mode,
            long XPos,
            long YPos,
            long XSize,
            long YSize) = 0;
        
        virtual void STDMETHODCALLTYPE SetSigWindow( void) = 0;
        
        virtual void STDMETHODCALLTYPE SetInkArea( void) = 0;
        
        virtual long STDMETHODCALLTYPE GetPixelBits( 
            HDC __MIDL__ISignaturePad0004) = 0;
        
        virtual long STDMETHODCALLTYPE MaximumX( void) = 0;
        
        virtual long STDMETHODCALLTYPE MaximumY( void) = 0;
        
        virtual long STDMETHODCALLTYPE MinimumX( void) = 0;
        
        virtual long STDMETHODCALLTYPE MinimumY( void) = 0;
        
        virtual long STDMETHODCALLTYPE MaximumSignableY( void) = 0;
        
        virtual long STDMETHODCALLTYPE FireOption( 
            long optionNumber) = 0;
        
        virtual BOOL STDMETHODCALLTYPE CanSaveSignatureValidationData( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE CaptureSignatureValidationData( 
            BOOL __MIDL__ISignaturePad0005) = 0;
        
        virtual long STDMETHODCALLTYPE AddKeyData( 
            BSTR __MIDL__ISignaturePad0006) = 0;
        
        virtual long STDMETHODCALLTYPE SaveSignatureValidationData( 
            BSTR __MIDL__ISignaturePad0007) = 0;
        
        virtual long STDMETHODCALLTYPE ImportSignature( 
            BSTR toFileName) = 0;
        
        virtual long STDMETHODCALLTYPE ExportSignature( 
            BSTR fromFileName) = 0;
        
        virtual long STDMETHODCALLTYPE GetSignature( 
            BSTR *pBstrClientFree) = 0;
        
        virtual long STDMETHODCALLTYPE SetSignature( 
            BSTR __MIDL__ISignaturePad0008) = 0;
        
        virtual long STDMETHODCALLTYPE OriginPointX( void) = 0;
        
        virtual long STDMETHODCALLTYPE OriginPointY( void) = 0;
        
        virtual long STDMETHODCALLTYPE BaselinePointX( void) = 0;
        
        virtual long STDMETHODCALLTYPE BaselinePointY( void) = 0;
        
        virtual DWORD STDMETHODCALLTYPE InkColor( void) = 0;
        
        virtual long STDMETHODCALLTYPE InkWeight( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ShowProperties( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISignaturePadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISignaturePad * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISignaturePad * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *TakeFrameWindow )( 
            ISignaturePad * This,
            HWND hwndFrame);
        
        long ( STDMETHODCALLTYPE *HostHandle )( 
            ISignaturePad * This,
            HWND hwnd);
        
        long ( STDMETHODCALLTYPE *Load )( 
            ISignaturePad * This,
            unsigned char *pszOPOSName,
            HWND hwndClientHost,
            void *pvICursiVisionServices);
        
        long ( STDMETHODCALLTYPE *Start )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *Unload )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *Stop )( 
            ISignaturePad * This);
        
        BOOL ( STDMETHODCALLTYPE *IsActive )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *Width )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *Height )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *LCDWidth )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *LCDHeight )( 
            ISignaturePad * This);
        
        double ( STDMETHODCALLTYPE *WidthInInches )( 
            ISignaturePad * This);
        
        double ( STDMETHODCALLTYPE *HeightInInches )( 
            ISignaturePad * This);
        
        double ( STDMETHODCALLTYPE *PadToLCDScaleX )( 
            ISignaturePad * This);
        
        double ( STDMETHODCALLTYPE *PadToLCDScaleY )( 
            ISignaturePad * This);
        
        double ( STDMETHODCALLTYPE *PadTimeout )( 
            ISignaturePad * This);
        
        double ( STDMETHODCALLTYPE *SetZoomFactor )( 
            ISignaturePad * This,
            double zoomFactor);
        
        unsigned char *( STDMETHODCALLTYPE *DeviceName )( 
            ISignaturePad * This);
        
        unsigned char *( STDMETHODCALLTYPE *DeviceProductName )( 
            ISignaturePad * This);
        
        unsigned char *( STDMETHODCALLTYPE *DeviceModel )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *IsLCD )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *IsFullPage )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *HasScaling )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *IsTabletPC )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *PrepareForPage )( 
            ISignaturePad * This,
            RECT *pRect);
        
        void ( STDMETHODCALLTYPE *DisplaySignatureBitmapFile )( 
            ISignaturePad * This,
            unsigned char *pszImageFile,
            /* [defaultvalue][optional] */ BOOL populatePad,
            /* [defaultvalue][optional] */ BOOL isLastPage);
        
        void ( STDMETHODCALLTYPE *DisplaySignatureBitmapHandle )( 
            ISignaturePad * This,
            HBITMAP hBitmap,
            HDC hdc,
            long x,
            long y,
            long cx,
            long cy);
        
        void ( STDMETHODCALLTYPE *OverlaySignatureBitmapHandle )( 
            ISignaturePad * This,
            HBITMAP hBitmap,
            HDC hdc,
            long x,
            long y,
            long cx,
            long cy);
        
        void ( STDMETHODCALLTYPE *DisplayHotSpotFile )( 
            ISignaturePad * This,
            unsigned char *pszFile,
            long eventID,
            long x,
            long y,
            long cx,
            long cy);
        
        void ( STDMETHODCALLTYPE *DisplayHotSpotHandle )( 
            ISignaturePad * This,
            HBITMAP hBitmap,
            HDC hdc,
            long eventID,
            long x,
            long y,
            long cx,
            long cy);
        
        void ( STDMETHODCALLTYPE *DisplayOk )( 
            ISignaturePad * This,
            long eventID);
        
        void ( STDMETHODCALLTYPE *DisplayClear )( 
            ISignaturePad * This,
            long eventID);
        
        void ( STDMETHODCALLTYPE *DisplayCancel )( 
            ISignaturePad * This,
            long eventID);
        
        short ( STDMETHODCALLTYPE *KeyPadQueryHotSpot )( 
            ISignaturePad * This,
            short KeyCode);
        
        void ( STDMETHODCALLTYPE *SetBackgroundFile )( 
            ISignaturePad * This,
            BSTR bstrFileName);
        
        void ( STDMETHODCALLTYPE *SetBackgroundHandle )( 
            ISignaturePad * This,
            OLE_HANDLE __MIDL__ISignaturePad0000);
        
        void ( STDMETHODCALLTYPE *GetRect )( 
            ISignaturePad * This,
            RECT *__MIDL__ISignaturePad0001);
        
        void ( STDMETHODCALLTYPE *GetHotSpot )( 
            ISignaturePad * This,
            long hotSpotNumber,
            RECT *__MIDL__ISignaturePad0002);
        
        long ( STDMETHODCALLTYPE *SignatureData )( 
            ISignaturePad * This);
        
        double *( STDMETHODCALLTYPE *SignatureDataX )( 
            ISignaturePad * This);
        
        double *( STDMETHODCALLTYPE *SignatureDataY )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *ClearSignatureData )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *DeleteSignatureData )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *PutLCDCaptureMode )( 
            ISignaturePad * This,
            long mode);
        
        void ( STDMETHODCALLTYPE *TabletState )( 
            ISignaturePad * This,
            long __MIDL__ISignaturePad0003);
        
        BOOL ( STDMETHODCALLTYPE *EnableTablet )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *ClearTablet )( 
            ISignaturePad * This);
        
        BOOL ( STDMETHODCALLTYPE *DisableTablet )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *LCDRefresh )( 
            ISignaturePad * This,
            long Mode,
            long XPos,
            long YPos,
            long XSize,
            long YSize);
        
        void ( STDMETHODCALLTYPE *SetSigWindow )( 
            ISignaturePad * This);
        
        void ( STDMETHODCALLTYPE *SetInkArea )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *GetPixelBits )( 
            ISignaturePad * This,
            HDC __MIDL__ISignaturePad0004);
        
        long ( STDMETHODCALLTYPE *MaximumX )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *MaximumY )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *MinimumX )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *MinimumY )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *MaximumSignableY )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *FireOption )( 
            ISignaturePad * This,
            long optionNumber);
        
        BOOL ( STDMETHODCALLTYPE *CanSaveSignatureValidationData )( 
            ISignaturePad * This);
        
        BOOL ( STDMETHODCALLTYPE *CaptureSignatureValidationData )( 
            ISignaturePad * This,
            BOOL __MIDL__ISignaturePad0005);
        
        long ( STDMETHODCALLTYPE *AddKeyData )( 
            ISignaturePad * This,
            BSTR __MIDL__ISignaturePad0006);
        
        long ( STDMETHODCALLTYPE *SaveSignatureValidationData )( 
            ISignaturePad * This,
            BSTR __MIDL__ISignaturePad0007);
        
        long ( STDMETHODCALLTYPE *ImportSignature )( 
            ISignaturePad * This,
            BSTR toFileName);
        
        long ( STDMETHODCALLTYPE *ExportSignature )( 
            ISignaturePad * This,
            BSTR fromFileName);
        
        long ( STDMETHODCALLTYPE *GetSignature )( 
            ISignaturePad * This,
            BSTR *pBstrClientFree);
        
        long ( STDMETHODCALLTYPE *SetSignature )( 
            ISignaturePad * This,
            BSTR __MIDL__ISignaturePad0008);
        
        long ( STDMETHODCALLTYPE *OriginPointX )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *OriginPointY )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *BaselinePointX )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *BaselinePointY )( 
            ISignaturePad * This);
        
        DWORD ( STDMETHODCALLTYPE *InkColor )( 
            ISignaturePad * This);
        
        long ( STDMETHODCALLTYPE *InkWeight )( 
            ISignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *ShowProperties )( 
            ISignaturePad * This);
        
        END_INTERFACE
    } ISignaturePadVtbl;

    interface ISignaturePad
    {
        CONST_VTBL struct ISignaturePadVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISignaturePad_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISignaturePad_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISignaturePad_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISignaturePad_TakeFrameWindow(This,hwndFrame)	\
    ( (This)->lpVtbl -> TakeFrameWindow(This,hwndFrame) ) 

#define ISignaturePad_HostHandle(This,hwnd)	\
    ( (This)->lpVtbl -> HostHandle(This,hwnd) ) 

#define ISignaturePad_Load(This,pszOPOSName,hwndClientHost,pvICursiVisionServices)	\
    ( (This)->lpVtbl -> Load(This,pszOPOSName,hwndClientHost,pvICursiVisionServices) ) 

#define ISignaturePad_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define ISignaturePad_Unload(This)	\
    ( (This)->lpVtbl -> Unload(This) ) 

#define ISignaturePad_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define ISignaturePad_IsActive(This)	\
    ( (This)->lpVtbl -> IsActive(This) ) 

#define ISignaturePad_Width(This)	\
    ( (This)->lpVtbl -> Width(This) ) 

#define ISignaturePad_Height(This)	\
    ( (This)->lpVtbl -> Height(This) ) 

#define ISignaturePad_LCDWidth(This)	\
    ( (This)->lpVtbl -> LCDWidth(This) ) 

#define ISignaturePad_LCDHeight(This)	\
    ( (This)->lpVtbl -> LCDHeight(This) ) 

#define ISignaturePad_WidthInInches(This)	\
    ( (This)->lpVtbl -> WidthInInches(This) ) 

#define ISignaturePad_HeightInInches(This)	\
    ( (This)->lpVtbl -> HeightInInches(This) ) 

#define ISignaturePad_PadToLCDScaleX(This)	\
    ( (This)->lpVtbl -> PadToLCDScaleX(This) ) 

#define ISignaturePad_PadToLCDScaleY(This)	\
    ( (This)->lpVtbl -> PadToLCDScaleY(This) ) 

#define ISignaturePad_PadTimeout(This)	\
    ( (This)->lpVtbl -> PadTimeout(This) ) 

#define ISignaturePad_SetZoomFactor(This,zoomFactor)	\
    ( (This)->lpVtbl -> SetZoomFactor(This,zoomFactor) ) 

#define ISignaturePad_DeviceName(This)	\
    ( (This)->lpVtbl -> DeviceName(This) ) 

#define ISignaturePad_DeviceProductName(This)	\
    ( (This)->lpVtbl -> DeviceProductName(This) ) 

#define ISignaturePad_DeviceModel(This)	\
    ( (This)->lpVtbl -> DeviceModel(This) ) 

#define ISignaturePad_IsLCD(This)	\
    ( (This)->lpVtbl -> IsLCD(This) ) 

#define ISignaturePad_IsFullPage(This)	\
    ( (This)->lpVtbl -> IsFullPage(This) ) 

#define ISignaturePad_HasScaling(This)	\
    ( (This)->lpVtbl -> HasScaling(This) ) 

#define ISignaturePad_IsTabletPC(This)	\
    ( (This)->lpVtbl -> IsTabletPC(This) ) 

#define ISignaturePad_PrepareForPage(This,pRect)	\
    ( (This)->lpVtbl -> PrepareForPage(This,pRect) ) 

#define ISignaturePad_DisplaySignatureBitmapFile(This,pszImageFile,populatePad,isLastPage)	\
    ( (This)->lpVtbl -> DisplaySignatureBitmapFile(This,pszImageFile,populatePad,isLastPage) ) 

#define ISignaturePad_DisplaySignatureBitmapHandle(This,hBitmap,hdc,x,y,cx,cy)	\
    ( (This)->lpVtbl -> DisplaySignatureBitmapHandle(This,hBitmap,hdc,x,y,cx,cy) ) 

#define ISignaturePad_OverlaySignatureBitmapHandle(This,hBitmap,hdc,x,y,cx,cy)	\
    ( (This)->lpVtbl -> OverlaySignatureBitmapHandle(This,hBitmap,hdc,x,y,cx,cy) ) 

#define ISignaturePad_DisplayHotSpotFile(This,pszFile,eventID,x,y,cx,cy)	\
    ( (This)->lpVtbl -> DisplayHotSpotFile(This,pszFile,eventID,x,y,cx,cy) ) 

#define ISignaturePad_DisplayHotSpotHandle(This,hBitmap,hdc,eventID,x,y,cx,cy)	\
    ( (This)->lpVtbl -> DisplayHotSpotHandle(This,hBitmap,hdc,eventID,x,y,cx,cy) ) 

#define ISignaturePad_DisplayOk(This,eventID)	\
    ( (This)->lpVtbl -> DisplayOk(This,eventID) ) 

#define ISignaturePad_DisplayClear(This,eventID)	\
    ( (This)->lpVtbl -> DisplayClear(This,eventID) ) 

#define ISignaturePad_DisplayCancel(This,eventID)	\
    ( (This)->lpVtbl -> DisplayCancel(This,eventID) ) 

#define ISignaturePad_KeyPadQueryHotSpot(This,KeyCode)	\
    ( (This)->lpVtbl -> KeyPadQueryHotSpot(This,KeyCode) ) 

#define ISignaturePad_SetBackgroundFile(This,bstrFileName)	\
    ( (This)->lpVtbl -> SetBackgroundFile(This,bstrFileName) ) 

#define ISignaturePad_SetBackgroundHandle(This,__MIDL__ISignaturePad0000)	\
    ( (This)->lpVtbl -> SetBackgroundHandle(This,__MIDL__ISignaturePad0000) ) 

#define ISignaturePad_GetRect(This,__MIDL__ISignaturePad0001)	\
    ( (This)->lpVtbl -> GetRect(This,__MIDL__ISignaturePad0001) ) 

#define ISignaturePad_GetHotSpot(This,hotSpotNumber,__MIDL__ISignaturePad0002)	\
    ( (This)->lpVtbl -> GetHotSpot(This,hotSpotNumber,__MIDL__ISignaturePad0002) ) 

#define ISignaturePad_SignatureData(This)	\
    ( (This)->lpVtbl -> SignatureData(This) ) 

#define ISignaturePad_SignatureDataX(This)	\
    ( (This)->lpVtbl -> SignatureDataX(This) ) 

#define ISignaturePad_SignatureDataY(This)	\
    ( (This)->lpVtbl -> SignatureDataY(This) ) 

#define ISignaturePad_ClearSignatureData(This)	\
    ( (This)->lpVtbl -> ClearSignatureData(This) ) 

#define ISignaturePad_DeleteSignatureData(This)	\
    ( (This)->lpVtbl -> DeleteSignatureData(This) ) 

#define ISignaturePad_PutLCDCaptureMode(This,mode)	\
    ( (This)->lpVtbl -> PutLCDCaptureMode(This,mode) ) 

#define ISignaturePad_TabletState(This,__MIDL__ISignaturePad0003)	\
    ( (This)->lpVtbl -> TabletState(This,__MIDL__ISignaturePad0003) ) 

#define ISignaturePad_EnableTablet(This)	\
    ( (This)->lpVtbl -> EnableTablet(This) ) 

#define ISignaturePad_ClearTablet(This)	\
    ( (This)->lpVtbl -> ClearTablet(This) ) 

#define ISignaturePad_DisableTablet(This)	\
    ( (This)->lpVtbl -> DisableTablet(This) ) 

#define ISignaturePad_LCDRefresh(This,Mode,XPos,YPos,XSize,YSize)	\
    ( (This)->lpVtbl -> LCDRefresh(This,Mode,XPos,YPos,XSize,YSize) ) 

#define ISignaturePad_SetSigWindow(This)	\
    ( (This)->lpVtbl -> SetSigWindow(This) ) 

#define ISignaturePad_SetInkArea(This)	\
    ( (This)->lpVtbl -> SetInkArea(This) ) 

#define ISignaturePad_GetPixelBits(This,__MIDL__ISignaturePad0004)	\
    ( (This)->lpVtbl -> GetPixelBits(This,__MIDL__ISignaturePad0004) ) 

#define ISignaturePad_MaximumX(This)	\
    ( (This)->lpVtbl -> MaximumX(This) ) 

#define ISignaturePad_MaximumY(This)	\
    ( (This)->lpVtbl -> MaximumY(This) ) 

#define ISignaturePad_MinimumX(This)	\
    ( (This)->lpVtbl -> MinimumX(This) ) 

#define ISignaturePad_MinimumY(This)	\
    ( (This)->lpVtbl -> MinimumY(This) ) 

#define ISignaturePad_MaximumSignableY(This)	\
    ( (This)->lpVtbl -> MaximumSignableY(This) ) 

#define ISignaturePad_FireOption(This,optionNumber)	\
    ( (This)->lpVtbl -> FireOption(This,optionNumber) ) 

#define ISignaturePad_CanSaveSignatureValidationData(This)	\
    ( (This)->lpVtbl -> CanSaveSignatureValidationData(This) ) 

#define ISignaturePad_CaptureSignatureValidationData(This,__MIDL__ISignaturePad0005)	\
    ( (This)->lpVtbl -> CaptureSignatureValidationData(This,__MIDL__ISignaturePad0005) ) 

#define ISignaturePad_AddKeyData(This,__MIDL__ISignaturePad0006)	\
    ( (This)->lpVtbl -> AddKeyData(This,__MIDL__ISignaturePad0006) ) 

#define ISignaturePad_SaveSignatureValidationData(This,__MIDL__ISignaturePad0007)	\
    ( (This)->lpVtbl -> SaveSignatureValidationData(This,__MIDL__ISignaturePad0007) ) 

#define ISignaturePad_ImportSignature(This,toFileName)	\
    ( (This)->lpVtbl -> ImportSignature(This,toFileName) ) 

#define ISignaturePad_ExportSignature(This,fromFileName)	\
    ( (This)->lpVtbl -> ExportSignature(This,fromFileName) ) 

#define ISignaturePad_GetSignature(This,pBstrClientFree)	\
    ( (This)->lpVtbl -> GetSignature(This,pBstrClientFree) ) 

#define ISignaturePad_SetSignature(This,__MIDL__ISignaturePad0008)	\
    ( (This)->lpVtbl -> SetSignature(This,__MIDL__ISignaturePad0008) ) 

#define ISignaturePad_OriginPointX(This)	\
    ( (This)->lpVtbl -> OriginPointX(This) ) 

#define ISignaturePad_OriginPointY(This)	\
    ( (This)->lpVtbl -> OriginPointY(This) ) 

#define ISignaturePad_BaselinePointX(This)	\
    ( (This)->lpVtbl -> BaselinePointX(This) ) 

#define ISignaturePad_BaselinePointY(This)	\
    ( (This)->lpVtbl -> BaselinePointY(This) ) 

#define ISignaturePad_InkColor(This)	\
    ( (This)->lpVtbl -> InkColor(This) ) 

#define ISignaturePad_InkWeight(This)	\
    ( (This)->lpVtbl -> InkWeight(This) ) 

#define ISignaturePad_ShowProperties(This)	\
    ( (This)->lpVtbl -> ShowProperties(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISignaturePad_INTERFACE_DEFINED__ */


#ifndef __ISignaturePadEvents_INTERFACE_DEFINED__
#define __ISignaturePadEvents_INTERFACE_DEFINED__

/* interface ISignaturePadEvents */
/* [object][uuid] */ 


EXTERN_C const IID IID_ISignaturePadEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("A64AB7AF-8A26-4f07-8877-56FE99570322")
    ISignaturePadEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TabletTimerEvent( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PenDown( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PenUp( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clicked( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PenPoint( 
            long x,
            long y) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OptionSelected( 
            long optionNumber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISignaturePadEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISignaturePadEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISignaturePadEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *TabletTimerEvent )( 
            ISignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *PenDown )( 
            ISignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *PenUp )( 
            ISignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clicked )( 
            ISignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *PenPoint )( 
            ISignaturePadEvents * This,
            long x,
            long y);
        
        HRESULT ( STDMETHODCALLTYPE *OptionSelected )( 
            ISignaturePadEvents * This,
            long optionNumber);
        
        END_INTERFACE
    } ISignaturePadEventsVtbl;

    interface ISignaturePadEvents
    {
        CONST_VTBL struct ISignaturePadEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISignaturePadEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISignaturePadEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISignaturePadEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISignaturePadEvents_TabletTimerEvent(This)	\
    ( (This)->lpVtbl -> TabletTimerEvent(This) ) 

#define ISignaturePadEvents_PenDown(This)	\
    ( (This)->lpVtbl -> PenDown(This) ) 

#define ISignaturePadEvents_PenUp(This)	\
    ( (This)->lpVtbl -> PenUp(This) ) 

#define ISignaturePadEvents_Clicked(This)	\
    ( (This)->lpVtbl -> Clicked(This) ) 

#define ISignaturePadEvents_PenPoint(This,x,y)	\
    ( (This)->lpVtbl -> PenPoint(This,x,y) ) 

#define ISignaturePadEvents_OptionSelected(This,optionNumber)	\
    ( (This)->lpVtbl -> OptionSelected(This,optionNumber) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISignaturePadEvents_INTERFACE_DEFINED__ */


#ifndef __IVisioLoggerSignaturePad_INTERFACE_DEFINED__
#define __IVisioLoggerSignaturePad_INTERFACE_DEFINED__

/* interface IVisioLoggerSignaturePad */
/* [object][uuid] */ 


EXTERN_C const IID IID_IVisioLoggerSignaturePad;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E6621BD-F297-4E50-A5CC-41D4A6AFC5E2")
    IVisioLoggerSignaturePad : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Initialize( 
            BSTR padName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ClearDisplay( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ShowBitmap( 
            long x,
            long y,
            long width,
            long height,
            OLE_HANDLE hBitmap,
            long durationInMilliseconds) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddBitmap( 
            long x,
            long y,
            long width,
            long height,
            OLE_HANDLE hBitmap,
            long durationInMilliseconds) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FlushBitmap( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Stop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Kill( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsConnected( 
            /* [retval][out] */ long *__MIDL__IVisioLoggerSignaturePad0000) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HasLCD( 
            /* [retval][out] */ long *__MIDL__IVisioLoggerSignaturePad0001) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LCDXSize( 
            /* [retval][out] */ long *pLCDXSize) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LCDYSize( 
            /* [retval][out] */ long *pLCDYSize) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LCDXStart( 
            /* [retval][out] */ long *pLCDXStart) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LCDYStart( 
            /* [retval][out] */ long *pLCDYStart) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_StrokeCount( 
            /* [retval][out] */ long *pStrokeCount) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PointCount( 
            /* [in] */ long strokeNumber,
            /* [retval][out] */ long *pPointCount) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [in] */ long strokeNumber,
            /* [in] */ long pointNumber,
            /* [retval][out] */ long *pX) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [in] */ long strokeNumber,
            /* [in] */ long pointNumber,
            /* [retval][out] */ long *pY) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PadScaleX( 
            /* [retval][out] */ double *pPadScaleX) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PadScaleY( 
            /* [retval][out] */ double *pPadScaleY) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MenuMode( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_MenuMode( 
            /* [in] */ long menuMode) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SmallFontSize( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MediumFontSize( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LargeFontSize( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SmallFontPixels( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MediumFontPixels( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LargeFontPixels( 
            /* [retval][out] */ long *pResult) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_BitDepth( 
            /* [in] */ long bitDepth) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVisioLoggerSignaturePadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVisioLoggerSignaturePad * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVisioLoggerSignaturePad * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVisioLoggerSignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *Initialize )( 
            IVisioLoggerSignaturePad * This,
            BSTR padName);
        
        HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IVisioLoggerSignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *ClearDisplay )( 
            IVisioLoggerSignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *ShowBitmap )( 
            IVisioLoggerSignaturePad * This,
            long x,
            long y,
            long width,
            long height,
            OLE_HANDLE hBitmap,
            long durationInMilliseconds);
        
        HRESULT ( STDMETHODCALLTYPE *AddBitmap )( 
            IVisioLoggerSignaturePad * This,
            long x,
            long y,
            long width,
            long height,
            OLE_HANDLE hBitmap,
            long durationInMilliseconds);
        
        HRESULT ( STDMETHODCALLTYPE *FlushBitmap )( 
            IVisioLoggerSignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IVisioLoggerSignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *Stop )( 
            IVisioLoggerSignaturePad * This);
        
        HRESULT ( STDMETHODCALLTYPE *Kill )( 
            IVisioLoggerSignaturePad * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsConnected )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *__MIDL__IVisioLoggerSignaturePad0000);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_HasLCD )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *__MIDL__IVisioLoggerSignaturePad0001);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LCDXSize )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pLCDXSize);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LCDYSize )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pLCDYSize);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LCDXStart )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pLCDXStart);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LCDYStart )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pLCDYStart);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_StrokeCount )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pStrokeCount);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointCount )( 
            IVisioLoggerSignaturePad * This,
            /* [in] */ long strokeNumber,
            /* [retval][out] */ long *pPointCount);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_X )( 
            IVisioLoggerSignaturePad * This,
            /* [in] */ long strokeNumber,
            /* [in] */ long pointNumber,
            /* [retval][out] */ long *pX);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_Y )( 
            IVisioLoggerSignaturePad * This,
            /* [in] */ long strokeNumber,
            /* [in] */ long pointNumber,
            /* [retval][out] */ long *pY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PadScaleX )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ double *pPadScaleX);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_PadScaleY )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ double *pPadScaleY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MenuMode )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_MenuMode )( 
            IVisioLoggerSignaturePad * This,
            /* [in] */ long menuMode);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SmallFontSize )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MediumFontSize )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LargeFontSize )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_SmallFontPixels )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_MediumFontPixels )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE *get_LargeFontPixels )( 
            IVisioLoggerSignaturePad * This,
            /* [retval][out] */ long *pResult);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE *put_BitDepth )( 
            IVisioLoggerSignaturePad * This,
            /* [in] */ long bitDepth);
        
        END_INTERFACE
    } IVisioLoggerSignaturePadVtbl;

    interface IVisioLoggerSignaturePad
    {
        CONST_VTBL struct IVisioLoggerSignaturePadVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVisioLoggerSignaturePad_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVisioLoggerSignaturePad_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVisioLoggerSignaturePad_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVisioLoggerSignaturePad_Initialize(This,padName)	\
    ( (This)->lpVtbl -> Initialize(This,padName) ) 

#define IVisioLoggerSignaturePad_Clear(This)	\
    ( (This)->lpVtbl -> Clear(This) ) 

#define IVisioLoggerSignaturePad_ClearDisplay(This)	\
    ( (This)->lpVtbl -> ClearDisplay(This) ) 

#define IVisioLoggerSignaturePad_ShowBitmap(This,x,y,width,height,hBitmap,durationInMilliseconds)	\
    ( (This)->lpVtbl -> ShowBitmap(This,x,y,width,height,hBitmap,durationInMilliseconds) ) 

#define IVisioLoggerSignaturePad_AddBitmap(This,x,y,width,height,hBitmap,durationInMilliseconds)	\
    ( (This)->lpVtbl -> AddBitmap(This,x,y,width,height,hBitmap,durationInMilliseconds) ) 

#define IVisioLoggerSignaturePad_FlushBitmap(This)	\
    ( (This)->lpVtbl -> FlushBitmap(This) ) 

#define IVisioLoggerSignaturePad_Start(This)	\
    ( (This)->lpVtbl -> Start(This) ) 

#define IVisioLoggerSignaturePad_Stop(This)	\
    ( (This)->lpVtbl -> Stop(This) ) 

#define IVisioLoggerSignaturePad_Kill(This)	\
    ( (This)->lpVtbl -> Kill(This) ) 

#define IVisioLoggerSignaturePad_get_IsConnected(This,__MIDL__IVisioLoggerSignaturePad0000)	\
    ( (This)->lpVtbl -> get_IsConnected(This,__MIDL__IVisioLoggerSignaturePad0000) ) 

#define IVisioLoggerSignaturePad_get_HasLCD(This,__MIDL__IVisioLoggerSignaturePad0001)	\
    ( (This)->lpVtbl -> get_HasLCD(This,__MIDL__IVisioLoggerSignaturePad0001) ) 

#define IVisioLoggerSignaturePad_get_LCDXSize(This,pLCDXSize)	\
    ( (This)->lpVtbl -> get_LCDXSize(This,pLCDXSize) ) 

#define IVisioLoggerSignaturePad_get_LCDYSize(This,pLCDYSize)	\
    ( (This)->lpVtbl -> get_LCDYSize(This,pLCDYSize) ) 

#define IVisioLoggerSignaturePad_get_LCDXStart(This,pLCDXStart)	\
    ( (This)->lpVtbl -> get_LCDXStart(This,pLCDXStart) ) 

#define IVisioLoggerSignaturePad_get_LCDYStart(This,pLCDYStart)	\
    ( (This)->lpVtbl -> get_LCDYStart(This,pLCDYStart) ) 

#define IVisioLoggerSignaturePad_get_StrokeCount(This,pStrokeCount)	\
    ( (This)->lpVtbl -> get_StrokeCount(This,pStrokeCount) ) 

#define IVisioLoggerSignaturePad_get_PointCount(This,strokeNumber,pPointCount)	\
    ( (This)->lpVtbl -> get_PointCount(This,strokeNumber,pPointCount) ) 

#define IVisioLoggerSignaturePad_get_X(This,strokeNumber,pointNumber,pX)	\
    ( (This)->lpVtbl -> get_X(This,strokeNumber,pointNumber,pX) ) 

#define IVisioLoggerSignaturePad_get_Y(This,strokeNumber,pointNumber,pY)	\
    ( (This)->lpVtbl -> get_Y(This,strokeNumber,pointNumber,pY) ) 

#define IVisioLoggerSignaturePad_get_PadScaleX(This,pPadScaleX)	\
    ( (This)->lpVtbl -> get_PadScaleX(This,pPadScaleX) ) 

#define IVisioLoggerSignaturePad_get_PadScaleY(This,pPadScaleY)	\
    ( (This)->lpVtbl -> get_PadScaleY(This,pPadScaleY) ) 

#define IVisioLoggerSignaturePad_get_MenuMode(This,pResult)	\
    ( (This)->lpVtbl -> get_MenuMode(This,pResult) ) 

#define IVisioLoggerSignaturePad_put_MenuMode(This,menuMode)	\
    ( (This)->lpVtbl -> put_MenuMode(This,menuMode) ) 

#define IVisioLoggerSignaturePad_get_SmallFontSize(This,pResult)	\
    ( (This)->lpVtbl -> get_SmallFontSize(This,pResult) ) 

#define IVisioLoggerSignaturePad_get_MediumFontSize(This,pResult)	\
    ( (This)->lpVtbl -> get_MediumFontSize(This,pResult) ) 

#define IVisioLoggerSignaturePad_get_LargeFontSize(This,pResult)	\
    ( (This)->lpVtbl -> get_LargeFontSize(This,pResult) ) 

#define IVisioLoggerSignaturePad_get_SmallFontPixels(This,pResult)	\
    ( (This)->lpVtbl -> get_SmallFontPixels(This,pResult) ) 

#define IVisioLoggerSignaturePad_get_MediumFontPixels(This,pResult)	\
    ( (This)->lpVtbl -> get_MediumFontPixels(This,pResult) ) 

#define IVisioLoggerSignaturePad_get_LargeFontPixels(This,pResult)	\
    ( (This)->lpVtbl -> get_LargeFontPixels(This,pResult) ) 

#define IVisioLoggerSignaturePad_put_BitDepth(This,bitDepth)	\
    ( (This)->lpVtbl -> put_BitDepth(This,bitDepth) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVisioLoggerSignaturePad_INTERFACE_DEFINED__ */


#ifndef __IVisioLoggerSignaturePadEvents_INTERFACE_DEFINED__
#define __IVisioLoggerSignaturePadEvents_INTERFACE_DEFINED__

/* interface IVisioLoggerSignaturePadEvents */
/* [object][uuid] */ 


EXTERN_C const IID IID_IVisioLoggerSignaturePadEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6E6621BD-F297-4E50-A5CC-41D4A6AFC5E3")
    IVisioLoggerSignaturePadEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE TabletTimerEvent( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PenDown( 
            long x,
            long y) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PenUp( 
            long x,
            long y) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clicked( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PenPoint( 
            long x,
            long y) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OptionSelected( 
            long optionNumber) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IVisioLoggerSignaturePadEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVisioLoggerSignaturePadEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVisioLoggerSignaturePadEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVisioLoggerSignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *TabletTimerEvent )( 
            IVisioLoggerSignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *PenDown )( 
            IVisioLoggerSignaturePadEvents * This,
            long x,
            long y);
        
        HRESULT ( STDMETHODCALLTYPE *PenUp )( 
            IVisioLoggerSignaturePadEvents * This,
            long x,
            long y);
        
        HRESULT ( STDMETHODCALLTYPE *Clicked )( 
            IVisioLoggerSignaturePadEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *PenPoint )( 
            IVisioLoggerSignaturePadEvents * This,
            long x,
            long y);
        
        HRESULT ( STDMETHODCALLTYPE *OptionSelected )( 
            IVisioLoggerSignaturePadEvents * This,
            long optionNumber);
        
        END_INTERFACE
    } IVisioLoggerSignaturePadEventsVtbl;

    interface IVisioLoggerSignaturePadEvents
    {
        CONST_VTBL struct IVisioLoggerSignaturePadEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVisioLoggerSignaturePadEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVisioLoggerSignaturePadEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVisioLoggerSignaturePadEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVisioLoggerSignaturePadEvents_TabletTimerEvent(This)	\
    ( (This)->lpVtbl -> TabletTimerEvent(This) ) 

#define IVisioLoggerSignaturePadEvents_PenDown(This,x,y)	\
    ( (This)->lpVtbl -> PenDown(This,x,y) ) 

#define IVisioLoggerSignaturePadEvents_PenUp(This,x,y)	\
    ( (This)->lpVtbl -> PenUp(This,x,y) ) 

#define IVisioLoggerSignaturePadEvents_Clicked(This)	\
    ( (This)->lpVtbl -> Clicked(This) ) 

#define IVisioLoggerSignaturePadEvents_PenPoint(This,x,y)	\
    ( (This)->lpVtbl -> PenPoint(This,x,y) ) 

#define IVisioLoggerSignaturePadEvents_OptionSelected(This,optionNumber)	\
    ( (This)->lpVtbl -> OptionSelected(This,optionNumber) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVisioLoggerSignaturePadEvents_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_VisioLoggerSignaturePad;

#ifdef __cplusplus

class DECLSPEC_UUID("B5A6A777-EA35-4dd7-9A54-F47239125534")
VisioLoggerSignaturePad;
#endif

EXTERN_C const CLSID CLSID_CursiVisionSignaturePad;

#ifdef __cplusplus

class DECLSPEC_UUID("A64AB7AF-8A26-4f07-8877-56FE99570323")
CursiVisionSignaturePad;
#endif
#endif /* __InnoVisioNateSignotecSignaturePads_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


