
#include "SignaturePad.h"

#include <float.h>


   long SignaturePad::SignatureData() {

   short totalStrokes = 0L;

//FIXME
#if 0
   pISigLib -> get_NumberOfStrokes(&totalStrokes);

   if ( 0 == totalStrokes )
      return 0;

   short totalPoints = 0;

   for ( long stroke = 0; stroke < totalStrokes; stroke++ ) {
      totalPoints++;
      short thisPoints = 0L;
      pISigLib -> get_NumPointsForStroke((short)stroke,&thisPoints);
      totalPoints += thisPoints;
   }

   if ( pSignatureDataX )
      delete [] pSignatureDataX;

   if ( pSignatureDataY )
      delete [] pSignatureDataY;

   pSignatureDataX = new double[totalPoints + 1];
   pSignatureDataY = new double[totalPoints + 1];
   
   memset(pSignatureDataX,0,(totalPoints + 1) * sizeof(double));
   memset(pSignatureDataY,0,(totalPoints + 1) * sizeof(double));

   double signatureMinX,signatureMinY,signatureMaxX,signatureMaxY;

   signatureMinX = DBL_MAX;
   signatureMinY = DBL_MAX;
   signatureMaxX = -DBL_MAX;
   signatureMaxY = -DBL_MAX;

   short pointNumber = 0;

   for ( short stroke = 0; stroke < totalStrokes; stroke++ ) {

      pointNumber++;

      short pointCount = 0L;
      pISigLib -> get_NumPointsForStroke((short)stroke,&pointCount);

      for ( short k = 0; k < pointCount; k++ ) {

         pSignatureDataX[pointNumber] = 0;
         pSignatureDataY[pointNumber] = 0;
//FIXME
#if 0
         pISigLib -> get_PointXValueDouble(stroke,k,&pSignatureDataX[pointNumber]);
         pISigLib -> get_PointYValueDouble(stroke,k,&pSignatureDataY[pointNumber]);
#endif

         signatureMinX = min(signatureMinX,pSignatureDataX[pointNumber]);
         signatureMinY = min(signatureMinY,pSignatureDataY[pointNumber]);

         signatureMaxX = max(signatureMaxX,pSignatureDataX[pointNumber]);
         signatureMaxY = max(signatureMaxY,pSignatureDataY[pointNumber]);

         pointNumber++;

      }

   }

   if ( 0.0 == scaleToPadX ) {

      short tabletLogicalXSize;
      short tabletLogicalYSize;
      short tabletXStart,tabletXStop;
      short tabletYStart,tabletYStop;

//FIXME
#if 0
      pISigLib -> get_TabletLogicalXSize(&tabletLogicalXSize);
      pISigLib -> get_TabletLogicalYSize(&tabletLogicalYSize);

      pISigLib -> get_TabletXStart(&tabletXStart);
      pISigLib -> get_TabletYStart(&tabletYStart);

      pISigLib -> get_TabletXStop(&tabletXStop);
      pISigLib -> get_TabletYStop(&tabletYStop);
#endif

      scaleToPadX = (double)thePad.lcd_width / (double)tabletLogicalXSize;
      scaleToPadY = (double)thePad.lcd_height / (double)tabletLogicalYSize;

      padWidthTopazUnits = (double)(tabletXStop - tabletXStart);
      padHeightTopazUnits = (double)(tabletYStop - tabletYStart);

   }

   double minX = signatureMinX * scaleToPadX;
   double minY = signatureMinY * scaleToPadY;

   for ( long k = 0; k < totalPoints; k++ ) {

      if ( 0.0 == pSignatureDataX[k] && 0.0 == pSignatureDataY[k] )
         continue;

      pSignatureDataX[k] = minX + (pSignatureDataX[k] - signatureMinX) * scaleToPadX;
      pSignatureDataY[k] = minY + (pSignatureDataY[k] - signatureMinY) * scaleToPadY;

   }

   return totalPoints;
#endif
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
