
#include "TabletPC.h"

   // IUnknown

   long __stdcall TabletPC::IInkCollectorEvents::QueryInterface(REFIID riid,void **ppv) {

   if ( riid == IID_IUnknown )
      *ppv = static_cast<IUnknown *>(this);
   else

   if ( riid == DIID__IInkCollectorEvents )
      *ppv = static_cast<IInkCollectorEvents *>(this);
   else

      return pParent -> QueryInterface(riid,ppv);

   AddRef();
   return S_OK;
   }
   unsigned long __stdcall TabletPC::IInkCollectorEvents::AddRef() {
   refCount++;
   return refCount;
   }
   unsigned long __stdcall TabletPC::IInkCollectorEvents::Release() { 
   refCount--;
   return refCount;
   }
 
   // IDispatch

   STDMETHODIMP TabletPC::IInkCollectorEvents::GetTypeInfoCount(UINT * pctinfo) { 
   *pctinfo = 0;
   return S_OK;
   } 


   long __stdcall TabletPC::IInkCollectorEvents::GetTypeInfo(UINT itinfo,LCID lcid,ITypeInfo **pptinfo) { 
   *pptinfo = NULL; 
   if ( itinfo != 0 ) 
      return DISP_E_BADINDEX; 
   return E_NOTIMPL; 
   } 
 

   STDMETHODIMP TabletPC::IInkCollectorEvents::GetIDsOfNames(REFIID riid,OLECHAR** rgszNames,UINT cNames,LCID lcid, DISPID* rgdispid) { 
   return E_NOTIMPL;
   }


   STDMETHODIMP TabletPC::IInkCollectorEvents::Invoke(DISPID dispidMember,REFIID riid,LCID lcid, 
                                                         WORD wFlags,DISPPARAMS *pDispParms,VARIANT *pVarResult,
                                                         EXCEPINFO *pexcepinfo,UINT *puArgErr) { 
   switch ( dispidMember ) {

   case DISPID_ICEStroke: {
      return Stroke((IInkCursor *)pDispParms -> rgvarg[2].pdispVal,(IInkStrokeDisp *)pDispParms -> rgvarg[1].pdispVal,pDispParms -> rgvarg[0].pboolVal);
      }

   case DISPID_ICECursorDown: {
      return CursorDown((IInkCursor *)pDispParms -> rgvarg[1].pdispVal,(IInkStrokeDisp *)pDispParms -> rgvarg[0].pdispVal);
      }

   case DISPID_ICECursorButtonDown: {
      return CursorButtonDown((IInkCursor *)pDispParms -> rgvarg[1].pdispVal,(IInkCursorButton *)pDispParms -> rgvarg[0].pdispVal);
      }

   case DISPID_ICENewInAirPackets: {
      return NewInAirPackets(NULL,0L,NULL);
      }

   case DISPID_IPEMouseDown: {
      return MouseDown((InkMouseButton)pDispParms -> rgvarg[4].lVal,(InkShiftKeyModifierFlags)pDispParms -> rgvarg[3].lVal,pDispParms -> rgvarg[2].lVal,pDispParms -> rgvarg[1].lVal,pDispParms -> rgvarg[0].pboolVal);
      }

   case DISPID_IPEMouseUp: {
      return MouseDown((InkMouseButton)pDispParms -> rgvarg[4].lVal,(InkShiftKeyModifierFlags)pDispParms -> rgvarg[3].lVal,pDispParms -> rgvarg[2].lVal,pDispParms -> rgvarg[1].lVal,pDispParms -> rgvarg[0].pboolVal);
      }

   case DISPID_IPEMouseMove: {
      return MouseMove((InkMouseButton)pDispParms -> rgvarg[4].lVal,(InkShiftKeyModifierFlags)pDispParms -> rgvarg[3].lVal,pDispParms -> rgvarg[2].lVal,pDispParms -> rgvarg[1].lVal,pDispParms -> rgvarg[0].pboolVal);
      }

   case DISPID_ICENewPackets: {
      return NewPackets((IInkCursor *)pDispParms -> rgvarg[3].pdispVal,(IInkStrokeDisp *)pDispParms -> rgvarg[2].pdispVal,pDispParms -> rgvarg[1].lVal,&pDispParms -> rgvarg[0]);
      }

   default:
      break;

   }
#if 0
    {	
   DISPID_ICEStroke	= 1,
	DISPID_ICECursorDown	= ( DISPID_ICEStroke + 1 ) ,
	DISPID_ICENewPackets	= ( DISPID_ICECursorDown + 1 ) ,
	DISPID_ICENewInAirPackets	= ( DISPID_ICENewPackets + 1 ) ,
	DISPID_ICECursorButtonDown	= ( DISPID_ICENewInAirPackets + 1 ) ,
	DISPID_ICECursorButtonUp	= ( DISPID_ICECursorButtonDown + 1 ) ,
	DISPID_ICECursorInRange	= ( DISPID_ICECursorButtonUp + 1 ) ,
	DISPID_ICECursorOutOfRange	= ( DISPID_ICECursorInRange + 1 ) ,
	DISPID_ICESystemGesture	= ( DISPID_ICECursorOutOfRange + 1 ) ,
	DISPID_ICEGesture	= ( DISPID_ICESystemGesture + 1 ) ,
	DISPID_ICETabletAdded	= ( DISPID_ICEGesture + 1 ) ,
	DISPID_ICETabletRemoved	= ( DISPID_ICETabletAdded + 1 ) ,
	DISPID_IOEPainting	= ( DISPID_ICETabletRemoved + 1 ) ,
	DISPID_IOEPainted	= ( DISPID_IOEPainting + 1 ) ,
	DISPID_IOESelectionChanging	= ( DISPID_IOEPainted + 1 ) ,
	DISPID_IOESelectionChanged	= ( DISPID_IOESelectionChanging + 1 ) ,
	DISPID_IOESelectionMoving	= ( DISPID_IOESelectionChanged + 1 ) ,
	DISPID_IOESelectionMoved	= ( DISPID_IOESelectionMoving + 1 ) ,
	DISPID_IOESelectionResizing	= ( DISPID_IOESelectionMoved + 1 ) ,
	DISPID_IOESelectionResized	= ( DISPID_IOESelectionResizing + 1 ) ,
	DISPID_IOEStrokesDeleting	= ( DISPID_IOESelectionResized + 1 ) ,
	DISPID_IOEStrokesDeleted	= ( DISPID_IOEStrokesDeleting + 1 ) ,
	DISPID_IPEChangeUICues	= ( DISPID_IOEStrokesDeleted + 1 ) ,
	DISPID_IPEClick	= ( DISPID_IPEChangeUICues + 1 ) ,
	DISPID_IPEDblClick	= ( DISPID_IPEClick + 1 ) ,
	DISPID_IPEInvalidated	= ( DISPID_IPEDblClick + 1 ) ,
	DISPID_IPEMouseDown	= ( DISPID_IPEInvalidated + 1 ) ,
	DISPID_IPEMouseEnter	= ( DISPID_IPEMouseDown + 1 ) ,
	DISPID_IPEMouseHover	= ( DISPID_IPEMouseEnter + 1 ) ,
	DISPID_IPEMouseLeave	= ( DISPID_IPEMouseHover + 1 ) ,
	DISPID_IPEMouseMove	= ( DISPID_IPEMouseLeave + 1 ) ,
	DISPID_IPEMouseUp	= ( DISPID_IPEMouseMove + 1 ) ,
	DISPID_IPEMouseWheel	= ( DISPID_IPEMouseUp + 1 ) ,
	DISPID_IPESizeModeChanged	= ( DISPID_IPEMouseWheel + 1 ) ,
	DISPID_IPEStyleChanged	= ( DISPID_IPESizeModeChanged + 1 ) ,
	DISPID_IPESystemColorsChanged	= ( DISPID_IPEStyleChanged + 1 ) ,
	DISPID_IPEKeyDown	= ( DISPID_IPESystemColorsChanged + 1 ) ,
	DISPID_IPEKeyPress	= ( DISPID_IPEKeyDown + 1 ) ,
	DISPID_IPEKeyUp	= ( DISPID_IPEKeyPress + 1 ) ,
	DISPID_IPEResize	= ( DISPID_IPEKeyUp + 1 ) ,
	DISPID_IPESizeChanged	= ( DISPID_IPEResize + 1 ) 
#endif
   return E_NOTIMPL;
   }

   HRESULT TabletPC::IInkCollectorEvents::Stroke(IInkCursor * pCursor,IInkStrokeDisp *pStroke,VARIANT_BOOL *pCancel) {
   strokeCount++;
   if ( pCancel )
      *pCancel = VARIANT_FALSE;
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::CursorDown(IInkCursor *pCursor,IInkStrokeDisp *pStroke) {
   strokeCount++;
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::NewPackets(IInkCursor *pCursor,IInkStrokeDisp *pStroke,long packetCount,VARIANT *pPacketData) {

   long strokeId = strokeCount - 1;

   if ( oldStrokeNumber != strokeId )
      pParent -> connectionPointContainer.fire_PenPoint(0,0,0.0f);

   long n = 0;
   SAFEARRAY *pValues = (SAFEARRAY *)((VARIANT *)pPacketData -> byref) -> parray;

   for ( long k = 0; k < packetCount; k++ ) {

      long pX,pY;

      long index = 0;
      SafeArrayGetElement(&pValues[k],&index,&pX);

      index = 1;
      SafeArrayGetElement(&pValues[k],&index,&pY);

      pParent -> pIInkRenderer -> InkSpaceToPixel((LONG_PTR)pParent -> hdcClient,&pX,&pY);

      pParent -> connectionPointContainer.fire_PenPoint(pX,pY,0.0f);

   }

   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::DblClick(VARIANT_BOOL * Cancel ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::MouseMove(InkMouseButton button,InkShiftKeyModifierFlags Shift,long pX,long pY,VARIANT_BOOL * Cancel ) {

   if ( ! IMF_Left == button )
      return S_OK;

   long strokeId = strokeCount - 1;

   if ( oldStrokeNumber != strokeId ) 
      pParent -> connectionPointContainer.fire_PenPoint(0,0,0.0f);

   oldStrokeNumber = strokeId;

   pParent -> connectionPointContainer.fire_PenPoint(pX,pY,0.0f);

   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::MouseDown(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long pX,long pY,VARIANT_BOOL * Cancel ) {
   strokeCount++;
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::MouseUp(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long pX,long pY,VARIANT_BOOL * Cancel ) {
   strokeCount++;
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::MouseWheel(InkMouseButton Button,InkShiftKeyModifierFlags Shift,long Delta,long x,long y,VARIANT_BOOL * Cancel ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::NewInAirPackets(IInkCursor * pCursor,long packetCount,VARIANT * pPacketData ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::CursorButtonDown(IInkCursor * pCursor,IInkCursorButton * pButton ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::CursorButtonUp(IInkCursor * pCursor,IInkCursorButton * pButton ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::CursorInRange(IInkCursor * pCursor,VARIANT_BOOL NewCursor,VARIANT & ButtonsState ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::CursorOutOfRange(IInkCursor * pCursor ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::SystemGesture(IInkCursor * Cursor,InkSystemGesture Id,long x,long y,long Modifier,BSTR Character,long CursorMode ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::Gesture(IInkCursor * Cursor,IInkStrokes * Strokes,VARIANT & Gestures,VARIANT_BOOL * Cancel ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::TabletAdded(IInkTablet * Tablet ) {
   return S_OK;
   }

   HRESULT TabletPC::IInkCollectorEvents::TabletRemoved(long TabletId ) {
   return S_OK;
   }
