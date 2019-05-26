
#include "SignaturePad.h"


   long SignaturePad::SignatureData() {
   return 0L;
   }


   void SignaturePad::ClearSignatureData() {
   return;
   }


   void SignaturePad::DeleteSignatureData() {

   if ( pSignatureDataX )
      delete [] pSignatureDataX;

   if ( pSignatureDataY )
      delete [] pSignatureDataY;

   pSignatureDataX = NULL;
   pSignatureDataY = NULL;

   return;
   }
