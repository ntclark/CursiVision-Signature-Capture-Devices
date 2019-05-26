
#include "SignaturePad.h"

   void SignaturePad::DisplaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy) {

   Bitmap *pBitmap = Bitmap::FromHBITMAP((HBITMAP)hBitmap,NULL);
   Bitmap *pMonochromeBitmap = pBitmap -> Clone(0,0,cx,cy,PixelFormat8bppIndexed);//PixelFormat4bppIndexed);//PixelFormat1bppIndexed);//PixelFormat8bppIndexed);
   HBITMAP hBitmapMonochrome = NULL;
   pMonochromeBitmap -> GetHBITMAP(Color(0,0,0),&hBitmapMonochrome);
   delete pMonochromeBitmap;
   delete pBitmap;

   long rc = STDisplaySetImage(x,y,hBitmapMonochrome);

   return;
   }


   void SignaturePad::OverlaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   return;
   }


   void SignaturePad::DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad,BOOL isLastPage) {
   OLECHAR bstrFile[MAX_PATH];
   MultiByteToWideChar(CP_ACP,0,pszImageFile,-1,bstrFile,MAX_PATH);
   STDisplaySetImageFromFile(0,0,bstrFile);//LONG nXPos, LONG nYPos, LPCWSTR szPath);
   return;
   }


   void SignaturePad::saveBitmapFile(HDC hdcSource,HBITMAP hBitmap,long bitCount,char *pszFileName) {

   BITMAP bitMap;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   bitMap.bmBitsPixel = (short)bitCount;

   long colorCount = bitMap.bmPlanes * bitMap.bmBitsPixel;

   long colorTableSize = (long)sizeof(RGBQUAD) * ( 1 << (bitMap.bmPlanes * bitMap.bmBitsPixel ) );

   long fileHeaderSize = sizeof(BITMAPFILEHEADER);
   long infoHeaderSize = sizeof(BITMAPINFOHEADER);
   long desiredSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   long entireSize = fileHeaderSize + infoHeaderSize + colorTableSize + desiredSizeImage;

   if ( 24 == bitCount )
      entireSize -= colorTableSize;

   BYTE *pBuffer = new BYTE[entireSize];

   memset(pBuffer,0,entireSize * sizeof(BYTE));

   BITMAPFILEHEADER *pBitmapFileHeader = (BITMAPFILEHEADER *)pBuffer;

   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)(pBuffer + sizeof(BITMAPFILEHEADER));

   BITMAPINFOHEADER *pBitmapInfoHeader = &(pBitmapInfo -> bmiHeader);
   
   pBitmapInfoHeader -> biSize = sizeof(BITMAPINFOHEADER); 
   pBitmapInfoHeader -> biWidth = bitMap.bmWidth;
   pBitmapInfoHeader -> biHeight = bitMap.bmHeight;
   pBitmapInfoHeader -> biPlanes = bitMap.bmPlanes; 
   pBitmapInfoHeader -> biBitCount = bitMap.bmBitsPixel;
   pBitmapInfoHeader -> biCompression = BI_RGB;
   pBitmapInfoHeader -> biClrUsed = 0;
   pBitmapInfoHeader -> biSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   pBitmapInfoHeader -> biClrImportant = 0;

   BYTE *pBits = (BYTE *)pBitmapFileHeader + fileHeaderSize + infoHeaderSize + colorTableSize;

   memset(pBits,0,pBitmapInfoHeader -> biSizeImage);

   long k = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,pBitmapInfo,DIB_RGB_COLORS);

   pBitmapFileHeader -> bfType = 0x4d42;

   pBitmapFileHeader -> bfSize = entireSize;

   pBitmapFileHeader -> bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize;

   sprintf(pszFileName,"%s.bmp",_tempnam(szApplicationDataDirectory,NULL));

   FILE *fBitmap = fopen(pszFileName,"wb");

   fwrite(pBitmapFileHeader,entireSize,1,fBitmap);

   fclose(fBitmap);

   delete [] pBuffer;

   return;
   }
