
#include "SignaturePad.h"

   SignaturePad::_IConnectionPointContainer::_IConnectionPointContainer(SignaturePad *pp) : 
     pParent(pp)
   { 
   return;
   }

   SignaturePad::_IConnectionPointContainer::~_IConnectionPointContainer() {
   return;
   }


   HRESULT SignaturePad::_IConnectionPointContainer::QueryInterface(REFIID riid,void **ppv) {
   return pParent -> QueryInterface(riid,ppv);
   }


   STDMETHODIMP_(ULONG) SignaturePad::_IConnectionPointContainer::AddRef() {
   return pParent -> AddRef();
   }

   STDMETHODIMP_(ULONG) SignaturePad::_IConnectionPointContainer::Release() {
   return pParent -> Release();
   }


   STDMETHODIMP SignaturePad::_IConnectionPointContainer::EnumConnectionPoints(IEnumConnectionPoints **ppEnum) {

   _IConnectionPoint *connectionPoints[1];

   *ppEnum = NULL;
 
   if ( pParent -> enumConnectionPoints ) delete pParent -> enumConnectionPoints;
 
   connectionPoints[0] = &pParent -> connectionPoint;
   pParent -> enumConnectionPoints = new _IEnumConnectionPoints(pParent,connectionPoints,1);
 
   return pParent -> enumConnectionPoints -> QueryInterface(IID_IEnumConnectionPoints,(void **)ppEnum);
   }
 
 
   STDMETHODIMP SignaturePad::_IConnectionPointContainer::FindConnectionPoint(REFIID riid,IConnectionPoint **ppCP) {
   *ppCP = NULL;
   if ( riid == IID_ISignaturePadEvents )
      return pParent -> connectionPoint.QueryInterface(IID_IConnectionPoint,(void **)ppCP);
   return CONNECT_E_NOCONNECTION;
   }


   void SignaturePad::_IConnectionPointContainer::fire_PenUp() {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   pParent -> connectionPoint.EnumConnections(&pIEnum);
   if ( ! pIEnum ) return;
   while ( 1 ) {
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;
      ISignaturePadEvents * pClient = reinterpret_cast<ISignaturePadEvents *>(connectData.pUnk);
      pClient -> PenUp();
   }
   static_cast<IUnknown*>(pIEnum) -> Release();
   return;
   }


   void SignaturePad::_IConnectionPointContainer::fire_PenDown() {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   pParent -> connectionPoint.EnumConnections(&pIEnum);
   if ( ! pIEnum ) return;
   while ( 1 ) {
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;
      ISignaturePadEvents * pClient = reinterpret_cast<ISignaturePadEvents *>(connectData.pUnk);
      pClient -> PenDown();
   }
   static_cast<IUnknown*>(pIEnum) -> Release();
   return;
   }


   void SignaturePad::_IConnectionPointContainer::fire_PenPoint(long x,long y) {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   pParent -> connectionPoint.EnumConnections(&pIEnum);
   if ( ! pIEnum ) return;
   while ( 1 ) {
      if ( pIEnum -> Next(1, &connectData, NULL) ) break;
      ISignaturePadEvents * pClient = reinterpret_cast<ISignaturePadEvents *>(connectData.pUnk);
      pClient -> PenPoint(x,y);
   }
   static_cast<IUnknown*>(pIEnum) -> Release();
   return;
   }


   void SignaturePad::_IConnectionPointContainer::fire_OptionSelected(long optionNumber) {
   IEnumConnections* pIEnum;
   CONNECTDATA connectData;
   pParent -> connectionPoint.EnumConnections(&pIEnum);
   if ( ! pIEnum ) 
      return;
   while ( 1 ) {
      if ( pIEnum -> Next(1, &connectData, NULL) ) 
         break;
      ISignaturePadEvents * pClient = reinterpret_cast<ISignaturePadEvents *>(connectData.pUnk);
      pClient -> OptionSelected(optionNumber);
   }
   static_cast<IUnknown*>(pIEnum) -> Release();
   return;
   }


