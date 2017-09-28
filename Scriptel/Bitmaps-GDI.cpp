 
#include <stddef.h>

#include "SignaturePad.h"

   void SignaturePad::SendImage() {

   if ( padBitsPerPixel != displayBitsPerPixel )
      ConvertBitmap();

   setImage(szCumulativeBitmapFile);
      //DeleteFile(szCumulativeBitmapFile);
   memset(szCumulativeBitmapFile,0,sizeof(szCumulativeBitmapFile));

   return;
   }


   void SignaturePad::DisplaySignatureBitmapHandle(HBITMAP hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   HGDIOBJ oldBitmap = SelectObject(hdc,hBitmap);
   MergeBitmap(hdc,x,y,cx,cy);
   SelectObject(hdc,oldBitmap);
   return;
   }


   void SignaturePad::OverlaySignatureBitmapHandle(HBITMAP hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   char szBitmapFileName[MAX_PATH];
   SaveBitmapFile(hdc,hBitmap,displayBitsPerPixel,szBitmapFileName);
   setImage(szBitmapFileName);
   DeleteFile(szBitmapFileName);
   return;
   }


   void SignaturePad::DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad,BOOL isLastPage) {
   setImage(pszImageFile);
   return;
   }

   void SignaturePad::CreateBitmapFile() {

#if 0
   if ( szCumulativeBitmapFile[0] )
      DeleteFile(szCumulativeBitmapFile);
#endif

   HDC hdc = CreateCompatibleDC(NULL);

   HBITMAP hBitmap = CreateCompatibleBitmap(hdc,thePad.lcd_width,thePad.lcd_height);
//   HBITMAP hBitmap = CreateBitmap(thePad.lcd_width,thePad.lcd_height,1,displayBitsPerPixel,NULL);

   SelectObject(hdc,hBitmap);

   long rc = BitBlt(hdc,0,0,thePad.lcd_width,thePad.lcd_height,NULL,0,0,WHITENESS);

   SaveBitmapFile(hdc,hBitmap,displayBitsPerPixel,szCumulativeBitmapFile);
   
   DeleteObject(hBitmap);

   DeleteDC(hdc);

   return;
   }


   void SignaturePad::SaveBitmapFile(HDC hdcSource,HBITMAP hBitmap,long bitsPerPixel,char *pszFileName) {

   BITMAP bitMap;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   bitMap.bmBitsPixel = (short)bitsPerPixel;

   long colorCount = bitMap.bmPlanes * bitMap.bmBitsPixel;

   WORD colorTableSize = sizeof(RGBQUAD) * ( 1 << (bitMap.bmPlanes * bitMap.bmBitsPixel ) );

   long fileHeaderSize = sizeof(BITMAPFILEHEADER);
   long infoHeaderSize = sizeof(BITMAPINFOHEADER);
   long desiredSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   long entireSize = fileHeaderSize + infoHeaderSize + colorTableSize + desiredSizeImage;

   if ( 24 == bitMap.bmBitsPixel )
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

   if ( pszFileName ) 
      sprintf(pszFileName,"%s.bmp",_tempnam(szApplicationDataDirectory,NULL));
   else
      pszFileName = szCumulativeBitmapFile;

   FILE *fBitmap = fopen(pszFileName,"wb");

   fwrite(pBitmapFileHeader,entireSize,1,fBitmap);

   fclose(fBitmap);

   delete [] pBuffer;

   return;
   }


   void SignaturePad::MergeBitmapFile(char *pszFileName,long x,long y,long cx,long cy) {

   FILE *fBitmap = fopen(pszFileName,"rb");
   if ( ! fBitmap )
      return;
   
   BITMAPFILEHEADER bitmapFileHeader;
   
   if ( ! fread(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,fBitmap) ) {
      fclose(fBitmap);
      return;
   }
   
   long colorTableSize = bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);

   BYTE *buffer = new BYTE[colorTableSize];
   
   if ( ! fread(buffer,colorTableSize,1,fBitmap) ) {
      delete [] buffer;
      fclose(fBitmap);
      return;
   }
   
   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)buffer;

   BYTE *pBits = new BYTE[pBitmapInfo -> bmiHeader.biSizeImage];

   long rc = fread(pBits,pBitmapInfo -> bmiHeader.biSizeImage,1,fBitmap);

   HDC hdcTarget = CreateCompatibleDC(NULL);
   HBITMAP hBitmap = CreateBitmap(cx,cy,1,displayBitsPerPixel,NULL);

   SelectObject(hdcTarget,hBitmap);

   rc = StretchDIBits(hdcTarget,0,0,cx,cy,0,0,
                                 pBitmapInfo -> bmiHeader.biWidth,pBitmapInfo -> bmiHeader.biHeight,pBits,pBitmapInfo,DIB_RGB_COLORS,SRCCOPY);

   fclose(fBitmap);

   MergeBitmap(hdcTarget,x,y,cx,cy);

   delete [] pBits;
   delete [] buffer;

   DeleteObject(hBitmap);
   DeleteDC(hdcTarget);

   return;
   }


   void SignaturePad::MergeBitmap(HDC hdcSource,long x,long y,long cx,long cy) {

   FILE *fBitmap = fopen(szCumulativeBitmapFile,"rb");
   if ( ! fBitmap )
      return;
   
   BITMAPFILEHEADER bitmapFileHeader;
   
   if ( ! fread(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,fBitmap) ) {
      fclose(fBitmap);
      return;
   }
   
   long colorTableSize = bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);

   BYTE *buffer = new BYTE[colorTableSize];
   
   if ( ! fread(buffer,colorTableSize,1,fBitmap) ) {
      delete [] buffer;
      fclose(fBitmap);
      return;
   }
   
   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)buffer;

   BYTE *pBits = new BYTE[pBitmapInfo -> bmiHeader.biSizeImage];

   long rc = fread(pBits,pBitmapInfo -> bmiHeader.biSizeImage,1,fBitmap);

   HDC hdcTarget = CreateCompatibleDC(hdcSource);//NULL);
   HBITMAP hBitmap = CreateBitmap(thePad.lcd_width,thePad.lcd_height,1,displayBitsPerPixel,NULL);
   //HBITMAP hBitmap = CreateCompatibleBitmap(hdcTarget,thePad.lcd_width,thePad.lcd_height);

   SelectObject(hdcTarget,hBitmap);

   rc = StretchDIBits(hdcTarget,0,0,thePad.lcd_width,thePad.lcd_height,0,0,
                                 pBitmapInfo -> bmiHeader.biWidth,pBitmapInfo -> bmiHeader.biHeight,pBits,pBitmapInfo,DIB_RGB_COLORS,SRCCOPY);

   rc = BitBlt(hdcTarget,x,y,cx,cy,hdcSource,0,0,SRCCOPY);
  
   delete [] pBits;
   delete [] buffer;

   fclose(fBitmap);

   SaveBitmapFile(hdcTarget,hBitmap,displayBitsPerPixel);

   DeleteObject(hBitmap);

   DeleteDC(hdcTarget);

   return;
   }


   void SignaturePad::SaveJPEGFile(HDC hdcSource,HBITMAP hBitmap,char *pszFileName) {

   BITMAP bitMap;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   bitMap.bmBitsPixel = (short)displayBitsPerPixel;

   long colorCount = bitMap.bmPlanes * bitMap.bmBitsPixel;

   WORD colorTableSize = sizeof(RGBQUAD) * ( 1 << (bitMap.bmPlanes * bitMap.bmBitsPixel ) );

   long fileHeaderSize = sizeof(BITMAPFILEHEADER);
   long infoHeaderSize = sizeof(BITMAPV5HEADER);
   long desiredSizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   long entireSize = fileHeaderSize + infoHeaderSize + colorTableSize + desiredSizeImage;

   if ( 24 == displayBitsPerPixel )
      entireSize -= colorTableSize;

   BYTE *pBuffer = new BYTE[entireSize];

   memset(pBuffer,0,entireSize * sizeof(BYTE));

   BITMAPFILEHEADER *pBitmapFileHeader = (BITMAPFILEHEADER *)pBuffer;

   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)(pBuffer + sizeof(BITMAPFILEHEADER));

   BITMAPV5HEADER *pBitmapInfoHeader = (BITMAPV5HEADER *)&(pBitmapInfo -> bmiHeader);
   
   pBitmapInfoHeader -> bV5Size = sizeof(BITMAPV5HEADER); 
   pBitmapInfoHeader -> bV5Width = bitMap.bmWidth;
   pBitmapInfoHeader -> bV5Height = bitMap.bmHeight;
   pBitmapInfoHeader -> bV5Planes = bitMap.bmPlanes; 
   pBitmapInfoHeader -> bV5BitCount = bitMap.bmBitsPixel;
   pBitmapInfoHeader -> bV5Compression = BI_JPEG;
   pBitmapInfoHeader -> bV5ClrUsed = 0;
   pBitmapInfoHeader -> bV5SizeImage = bitMap.bmHeight * ((bitMap.bmWidth * bitMap.bmPlanes * bitMap.bmBitsPixel + 31) & ~31) / 8 ;
   pBitmapInfoHeader -> bV5ClrImportant = 0;

   BYTE *pBits = (BYTE *)pBitmapFileHeader + fileHeaderSize + infoHeaderSize + colorTableSize;

   memset(pBits,0,pBitmapInfoHeader -> bV5SizeImage);

   long k = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,NULL,pBitmapInfo,DIB_RGB_COLORS);
   k = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,pBitmapInfo,DIB_RGB_COLORS);

   pBitmapFileHeader -> bfType = 0x4d42;

   pBitmapFileHeader -> bfSize = entireSize;

   pBitmapFileHeader -> bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPV5HEADER) + colorTableSize;

   sprintf(pszFileName,"%s.jpg",_tempnam(szApplicationDataDirectory,NULL));

   FILE *fBitmap = fopen(pszFileName,"wb");

   fwrite(pBitmapFileHeader,entireSize,1,fBitmap);

   fclose(fBitmap);

   delete [] pBuffer;

   return;
   }


   void SignaturePad::ConvertBitmap() {

   FILE *fBitmap = fopen(szCumulativeBitmapFile,"rb");
   if ( ! fBitmap )
      return;
   
   BITMAPFILEHEADER bitmapFileHeader;
   
   if ( ! fread(&bitmapFileHeader,sizeof(BITMAPFILEHEADER),1,fBitmap) ) {
      fclose(fBitmap);
      return;
   }
   
   long colorTableSize = bitmapFileHeader.bfOffBits - sizeof(BITMAPFILEHEADER);

   BYTE *buffer = new BYTE[colorTableSize];
   
   if ( ! fread(buffer,colorTableSize,1,fBitmap) ) {
      delete [] buffer;
      fclose(fBitmap);
      return;
   }
   
   BITMAPINFO *pBitmapInfo = (BITMAPINFO *)buffer;

   BYTE *pBits = new BYTE[pBitmapInfo -> bmiHeader.biSizeImage];

   long rc = fread(pBits,pBitmapInfo -> bmiHeader.biSizeImage,1,fBitmap);

   HDC hdcTarget = CreateCompatibleDC(NULL);

   HBITMAP hBitmap = CreateBitmap(thePad.lcd_width,thePad.lcd_height,1,padBitsPerPixel,NULL);

   SelectObject(hdcTarget,hBitmap);

   rc = StretchDIBits(hdcTarget,0,0,thePad.lcd_width,thePad.lcd_height,0,0,
                                 pBitmapInfo -> bmiHeader.biWidth,pBitmapInfo -> bmiHeader.biHeight,pBits,pBitmapInfo,DIB_RGB_COLORS,SRCCOPY);

   fclose(fBitmap);

   SaveBitmapFile(hdcTarget,hBitmap,padBitsPerPixel);


#if 0
   BITMAP bitMap;
   BITMAPINFO bitMapInfo;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   memset(&bitMapInfo,0,sizeof(bitMapInfo));

   bitMapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bitMapInfo.bmiHeader.biWidth = bitMap.bmWidth;
   bitMapInfo.bmiHeader.biHeight = bitMap.bmHeight;
   bitMapInfo.bmiHeader.biPlanes = bitMap.bmPlanes;
   bitMapInfo.bmiHeader.biBitCount = (unsigned short)bitCount;
   bitMapInfo.bmiHeader.biCompression= BI_RGB;

   BYTE *pBits = NULL;

   HBITMAP hNewBitmap = CreateDIBSection(hdcSource,&bitMapInfo,DIB_RGB_COLORS,reinterpret_cast<void **>(&pBits),NULL,0L);

   long rc = GetDIBits(hdcSource,hBitmap,0,bitMap.bmHeight,pBits,&bitMapInfo,DIB_RGB_COLORS);

   return hNewBitmap;
#endif
   return;
   }
