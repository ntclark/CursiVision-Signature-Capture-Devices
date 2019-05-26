
#include "SignaturePad.h"

   void SignaturePad::DisplaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   loadBitmapFromHandle(x,y,cx,cy,cx,cy,(HBITMAP)hBitmap);
   sendBitmapHandle(hBitmapPad);
   return;
   }


   void SignaturePad::sendBitmapHandle(HBITMAP hBitmap) {
   if ( ! ( NULL == sendingBitmapThreadHandle ) )
      TerminateThread(sendingBitmapThreadHandle,0L);
   sendingBitmapThreadHandle = (HANDLE)_beginthreadex(NULL,0L,eventuallySendBitmap,(void *)this,0L,&sendingBitmapThreadAddress);
   return;
   }


   void SignaturePad::OverlaySignatureBitmapHandle(UINT_PTR hBitmap,HDC hdc,long x,long y,long cx,long cy) {
   return;
   }


   void SignaturePad::DisplaySignatureBitmapFile(char *pszImageFile,BOOL populatePad,BOOL isLastPage) {
   loadBitmapFromFile(0,0,padBitmapPixelWidth,padBitmapPixelHeight,padBitmapPixelWidth,padBitmapPixelHeight,pszImageFile);
   sendBitmapHandle(hBitmapPad);
   return;
   }


   void SignaturePad::saveBitmapFile(HBITMAP hBitmap,char *pszFileName) {

   HDC hdcSource = CreateCompatibleDC(NULL);

   BITMAP bitMap;

   GetObject(hBitmap,sizeof(BITMAP),&bitMap);

   bitMap.bmBitsPixel = (short)displayBitsPerPixel;

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

   sprintf(pszFileName,"%s.bmp",_tempnam(szApplicationDataDirectory,NULL));

   FILE *fBitmap = fopen(pszFileName,"wb");

   fwrite(pBitmapFileHeader,entireSize,1,fBitmap);

   fclose(fBitmap);

   delete [] pBuffer;

   return;
   }


   HBITMAP SignaturePad::createPadBitmap() {
   HDC hdc = CreateCompatibleDC(NULL);
   HBITMAP hBitmap = CreateDIBSection(hdc,&padBitmapInfo,DIB_RGB_COLORS,(void **)&pPadBitmapBits,NULL,0L);
   DeleteDC(hdc);
   return hBitmap;
   }


   HBITMAP SignaturePad::createTemporaryBitmap(long cx,long cy) {

   temporaryBitmapInfo.bmiHeader.biWidth = cx;
   temporaryBitmapInfo.bmiHeader.biHeight = -cy;

   temporaryBitmapInfo.bmiHeader.biBitCount = IS_COLOR_PAD ? (WORD)PREFERRED_COLOR_IMAGE_BIT_DEPTH : (WORD)MONOCHROME_IMAGE_BITS;
   temporaryBitmapInfo.bmiHeader.biSizeImage = cy * ((cx * temporaryBitmapInfo.bmiHeader.biBitCount) & ~31) / 8 ;

   HDC hdc = CreateCompatibleDC(NULL);

   BYTE *ppCreatedBits = NULL;

   HBITMAP hBitmap = CreateDIBSection(hdc,&temporaryBitmapInfo,DIB_RGB_COLORS,(void **)&ppCreatedBits,NULL,0L);

   DeleteDC(hdc);

   return hBitmap;
   }


   void SignaturePad::clearImageBits(long x,long y,long cx,long cy) {

   if ( cx == padBitmapPixelWidth && cy == padBitmapPixelHeight ) {
      memset(pPadBitmapBits,0x00,padBitmapPixelBytes);
      return;
   }

   HDC hdc = CreateCompatibleDC(NULL);
   HBITMAP hBitmap = createTemporaryBitmap(cx,cy);
   HGDIOBJ fms = SelectObject(hdc,hBitmap);
   BitBlt(hdc,0,0,cx,cy,NULL,0,0,WHITENESS);
   SelectObject(hdc,fms);
   loadBitmapFromHandle(x,y,cx,cy,cx,cy,hBitmap);
   DeleteObject(hBitmap);
   DeleteDC(hdc);

   return;
   }

   void SignaturePad::loadBitmapFromFile(long xTarget,long yTarget,long cxTarget,long cyTarget,long cxSource,long cySource,char *pszFileName,long bitBltOperation) {

   BSTR bstrFile = SysAllocStringLen(NULL,MAX_PATH);

   MultiByteToWideChar(CP_ACP,0,pszFileName,-1,bstrFile,MAX_PATH);

   Bitmap *pBitmap = Bitmap::FromFile(bstrFile);

   SysFreeString(bstrFile);

#ifdef FUCK_GDI_PLUS_AND_MICROSOFT
   Status sc = pBitmap->
   if ( pBitmap->lastResult ) {
      delete pBitmap;
      GdiplusShutdown(&gdiplusToken);
      return;
   }
#endif

   HBITMAP hBitmapSource = NULL;

   pBitmap -> GetHBITMAP(Color(0,0,0),&hBitmapSource);

   loadBitmapFromHandle(xTarget,yTarget,cxTarget,cyTarget,cxSource,cySource,hBitmapSource,bitBltOperation);

   delete pBitmap;

   return;
   }

   void SignaturePad::loadBitmapFromHandle(long xTarget,long yTarget,long cxTarget,long cyTarget,long cxSource,long cySource,HBITMAP hBitmapSource,long bitBltOperation) {

   HDC hdcTarget = CreateCompatibleDC(NULL);
   HDC hdcSource = CreateCompatibleDC(NULL);

   HGDIOBJ oldSourceBitmap = SelectObject(hdcSource,hBitmapSource);
   HGDIOBJ oldTargetBitmap = SelectObject(hdcTarget,hBitmapPad);

   long rc = StretchBlt(hdcTarget,xTarget,yTarget,cxTarget,cyTarget,hdcSource,0,0,cxSource,cySource,bitBltOperation);

   SelectObject(hdcSource,oldSourceBitmap);
   SelectObject(hdcTarget,oldTargetBitmap);

   DeleteDC(hdcSource);
   DeleteDC(hdcTarget);

   return;
   }


   unsigned int __stdcall SignaturePad::eventuallySendBitmap(void *p) {
   Sleep(500);
   SignaturePad *pThis = reinterpret_cast<SignaturePad *>(p);
   pThis -> sendBitmapHandleImmediate(hBitmapPad);
   pThis -> sendingBitmapThreadHandle = NULL;
   return 0;
   }


   void SignaturePad::sendBitmapHandleImmediate(HBITMAP hBitmap) {
   char szFile[MAX_PATH];
   saveBitmapFile(hBitmap,szFile);
   setImage(szFile);
   DeleteFile(szFile);
   return;
   }

