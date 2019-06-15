#include <WacomGSS/STU/ProtocolHelper.hpp>
#include <WacomGSS/Win32/com.hpp>

#if defined(USE_ATLTRACE)
#include <atlbase.h>
#include <atltrace.h>

std::wstring toString(WICPixelFormatGUID const & guid) {
  RPC_WSTR rpcStr;
  UuidToStringW(&guid, &rpcStr);
  std::wstring  ret((wchar_t *)rpcStr);
  RpcStringFree(&rpcStr);
  return ret;
}
#endif

namespace WacomGSS
{
  namespace STU
  {    

    namespace ProtocolHelper
    {

      
      std::vector<uint8_t> flattenMonochrome(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, uint16_t screenWidth, uint16_t screenHeight)
      {
        UINT bitmapWidth, bitmapHeight;

        Win32::hresult_succeeded(pIWICBitmapSource->GetSize(&bitmapWidth, &bitmapHeight));

        Win32::com_ptr<IWICFormatConverter> pIWICFormatConverter;
        Win32::hresult_succeeded(pIWICImagingFactory->CreateFormatConverter(&pIWICFormatConverter));

        if (bitmapWidth != screenWidth || bitmapHeight != screenHeight)
        {
          Win32::com_ptr<IWICBitmapScaler> pIWICBitmapScaler;
          Win32::hresult_succeeded(pIWICImagingFactory->CreateBitmapScaler(&pIWICBitmapScaler));
  
          Win32::hresult_succeeded(pIWICBitmapScaler->Initialize(pIWICBitmapSource, screenWidth, screenHeight, WICBitmapInterpolationModeFant));
          
          Win32::hresult_succeeded(pIWICFormatConverter->Initialize(pIWICBitmapScaler.get(), GUID_WICPixelFormatBlackWhite, WICBitmapDitherTypeErrorDiffusion, nullptr, 0.0f, WICBitmapPaletteTypeFixedBW));
        }
        else
        {
          Win32::hresult_succeeded(pIWICFormatConverter->Initialize(pIWICBitmapSource, GUID_WICPixelFormatBlackWhite, WICBitmapDitherTypeErrorDiffusion, nullptr, 0.0f, WICBitmapPaletteTypeFixedBW));
        }
             
        auto stride = (screenWidth+7u)/8u;

        std::vector<uint8_t> data(screenHeight * stride);
        
        Win32::hresult_succeeded(pIWICFormatConverter->CopyPixels(nullptr, stride, data.size(), data.data()));

        return std::move(data);
      }



      static inline void swapBytes(uint16_t * begin, uint16_t const * end)
      {
        for (; begin != end; ++begin)
        {
          auto b = *begin;
          *begin = static_cast<uint16_t>( ((b>>8) | (b<<8)) & 0xffff );
        }
      }



      std::vector<uint8_t> flattenColor16(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, uint16_t screenWidth, uint16_t screenHeight)
      {
        UINT bitmapWidth, bitmapHeight;

        Win32::hresult_succeeded(pIWICBitmapSource->GetSize(&bitmapWidth, &bitmapHeight));

        Win32::com_ptr<IWICFormatConverter> pIWICFormatConverter;
        Win32::hresult_succeeded(pIWICImagingFactory->CreateFormatConverter(&pIWICFormatConverter));

        if (bitmapWidth != screenWidth || bitmapHeight != screenHeight)
        {
          Win32::com_ptr<IWICBitmapScaler> pIWICBitmapScaler;
          Win32::hresult_succeeded(pIWICImagingFactory->CreateBitmapScaler(&pIWICBitmapScaler));
  
          Win32::hresult_succeeded(pIWICBitmapScaler->Initialize(pIWICBitmapSource, screenWidth, screenHeight, WICBitmapInterpolationModeFant));
          
          Win32::hresult_succeeded(pIWICFormatConverter->Initialize(pIWICBitmapScaler.get(), GUID_WICPixelFormat16bppBGR565, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom));
        }
        else
        {
          Win32::hresult_succeeded(pIWICFormatConverter->Initialize(pIWICBitmapSource, GUID_WICPixelFormat16bppBGR565, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom));
        }
      
        auto stride = screenWidth * 2u;

        std::vector<uint8_t> data(screenHeight * stride);
      
        Win32::hresult_succeeded(pIWICFormatConverter->CopyPixels(nullptr, stride, data.size(), data.data()));

        swapBytes(reinterpret_cast<std::uint16_t *>(data.data()), reinterpret_cast<uint16_t *>(data.data()+data.size()));

        return std::move(data);
      }



      std::vector<uint8_t> flattenColor24(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, uint16_t screenWidth, uint16_t screenHeight)
      {
        UINT bitmapWidth, bitmapHeight;

        Win32::hresult_succeeded(pIWICBitmapSource->GetSize(&bitmapWidth, &bitmapHeight));

        Win32::com_ptr<IWICFormatConverter> pIWICFormatConverter;
        Win32::hresult_succeeded(pIWICImagingFactory->CreateFormatConverter(&pIWICFormatConverter));

        if (bitmapWidth != screenWidth || bitmapHeight != screenHeight)
        {
          Win32::com_ptr<IWICBitmapScaler> pIWICBitmapScaler;
          Win32::hresult_succeeded(pIWICImagingFactory->CreateBitmapScaler(&pIWICBitmapScaler));
  
          Win32::hresult_succeeded(pIWICBitmapScaler->Initialize(pIWICBitmapSource, screenWidth, screenHeight, WICBitmapInterpolationModeFant));
          
          Win32::hresult_succeeded(pIWICFormatConverter->Initialize(pIWICBitmapScaler.get(), GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom));
        }
        else
        {
          Win32::hresult_succeeded(pIWICFormatConverter->Initialize(pIWICBitmapSource, GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, nullptr, 0.0f, WICBitmapPaletteTypeCustom));
        }
      
        auto stride = screenWidth * 3u;

        std::vector<uint8_t> data(screenHeight * stride);
      
        Win32::hresult_succeeded(pIWICFormatConverter->CopyPixels(nullptr, stride, data.size(), data.data()));

        return std::move(data);
      }


      //=======================================================================



      static UINT UINTround(float f)
      {
        return (static_cast<UINT>(f * 2.0f) + 1u) / 2u;
      }



      static bool clipBitmap(IWICImagingFactory * pIWICImagingFactory, Win32::com_ptr<IWICBitmapSource> & bitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, UINT & bitmapWidth, UINT & bitmapHeight, std::uint8_t clipMode)
      {
        bool requiresBorders;

         if (bitmapWidth > screenWidth || bitmapHeight > screenHeight)
        { 
          requiresBorders = false;

          UINT clippedX, clippedY, clippedWidth, clippedHeight;
            
          if (bitmapWidth > screenWidth)
          {              
            switch (clipMode & 0x0f)
            {
              default:
              case 0: clippedX = 0; break;
              case 1: clippedX = (bitmapWidth - screenWidth) / 2; break;
              case 2: clippedX = (bitmapWidth - screenWidth) - 1; break;
            }
            clippedWidth = screenWidth;
          }
          else
          {
            clippedX = 0;
            clippedWidth = bitmapWidth;
            requiresBorders = true;
          }
            
          if (bitmapHeight > screenHeight)
          {              
            switch ((clipMode >> 4) & 0xf)
            {
              default:
              case 0: clippedY = 0; break;
              case 1: clippedY = (bitmapHeight - screenHeight) / 2; break;
              case 2: clippedY = (bitmapHeight - screenHeight) - 1; break;
            }
            clippedHeight = screenHeight;
          }
          else
          {
            clippedY = 0;
            clippedHeight = bitmapHeight;
            requiresBorders = true;
          }

          Win32::com_ptr<IWICBitmap> clippedBitmap;
          Win32::hresult_succeeded( pIWICImagingFactory->CreateBitmapFromSourceRect(bitmapSource.get(), clippedX, clippedY, clippedWidth, clippedHeight, &clippedBitmap) );  
          bitmapSource = std::move(clippedBitmap);
          bitmapWidth  = clippedWidth;
          bitmapHeight = clippedHeight;  
        }
        else
        {
          requiresBorders = true;
          //Win32::com_ptr<IWICBitmap> unclippedBitmap;
          //Win32::hresult_succeeded( pIWICImagingFactory->CreateBitmapFromSourceRect(bitmapSource.get(), 0, 0, bitmapWidth, bitmapHeight, &unclippedBitmap) );  
          //bitmapSource = std::move(unclippedBitmap);
        }

        return requiresBorders;
      }

      
      
      static bool scaleBitmap(IWICImagingFactory * pIWICImagingFactory, Win32::com_ptr<IWICBitmapSource> & bitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, UINT & bitmapWidth, UINT & bitmapHeight)
      {
        float bitmapRatio = float(bitmapWidth) / float(bitmapHeight);
        float screenRatio = float(screenWidth) / float(screenHeight);

        bool requiresBorders = (bitmapRatio != screenRatio);
        if (requiresBorders)
        {
          UINT scaledWidth, scaledHeight;
          if (bitmapRatio > screenRatio)
          {
            // source is wider than dest, fit to width, scale height
            scaledWidth  = screenWidth;
            scaledHeight = UINTround(scaledWidth / bitmapRatio);
          }
          else
          {
            scaledHeight = screenHeight;
            scaledWidth  = UINTround(scaledHeight * bitmapRatio);        
          }
    
          Win32::com_ptr<IWICBitmapScaler> bitmapScaler;
          Win32::hresult_succeeded( pIWICImagingFactory->CreateBitmapScaler(&bitmapScaler) );
          Win32::hresult_succeeded( bitmapScaler->Initialize( bitmapSource.get(), scaledWidth, scaledHeight, WICBitmapInterpolationModeFant)  );

          bitmapSource = std::move(bitmapScaler);
          bitmapWidth  = scaledWidth;
          bitmapHeight = scaledHeight;
        }

        return requiresBorders;
      }



      static void fillRect(Win32::com_ptr<IWICBitmap> & bitmap, WICRect const & rc, uint8_t byte)
      {           
        Win32::com_ptr<IWICBitmapLock> bitmapLock; 
        Win32::hresult_succeeded( bitmap->Lock(&rc, WICBitmapLockWrite, &bitmapLock) );
  
        UINT   size;
        BYTE * pData;  
        Win32::hresult_succeeded( bitmapLock->GetDataPointer(&size, &pData) );

        FillMemory(pData, size, byte);
      }

      
      inline void fillWhite(Win32::com_ptr<IWICBitmap> & bitmap, WICRect const & rc) 
      {
        fillRect(bitmap, rc, 0xff);
      }


      inline void fillBlack(Win32::com_ptr<IWICBitmap> & bitmap, WICRect const & rc)
      {           
        fillRect(bitmap, rc, 0x00);
      }


      void resizeImage(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, IWICBitmapSource * * pRetVal, Scale scale, Border border, std::uint8_t clipMode)
      {
        Win32::initializeRetVal(pRetVal);

        if (!pIWICImagingFactory || !pIWICBitmapSource)
          Win32::throw_HRESULT_error(E_INVALIDARG);
        
        Win32::com_ptr<IWICBitmapSource> bitmapSource(pIWICBitmapSource, Win32::addref);

        if (scale != Scale_Stretch)
        {      
          UINT bitmapWidth, bitmapHeight;
          Win32::hresult_succeeded( bitmapSource->GetSize(&bitmapWidth, &bitmapHeight) );
          
          if (bitmapWidth != screenWidth || bitmapHeight != screenHeight)
          {
            bool requiresBorders;

            if (scale == Scale_Clip)
            {
              requiresBorders = clipBitmap(pIWICImagingFactory, bitmapSource, screenWidth, screenHeight, bitmapWidth, bitmapHeight, clipMode);            
            }
            else
            {
              requiresBorders = scaleBitmap(pIWICImagingFactory, bitmapSource, screenWidth, screenHeight, bitmapWidth, bitmapHeight);
              clipMode = Clip_Center | Clip_Middle;
            }

            if (requiresBorders)
            {
              WICPixelFormatGUID pixelFormat;
              Win32::hresult_succeeded( bitmapSource->GetPixelFormat(&pixelFormat) );          
          
              Win32::com_ptr<IWICBitmap> bitmap;
              Win32::hresult_succeeded( pIWICImagingFactory->CreateBitmap(screenWidth, screenHeight, pixelFormat, WICBitmapCacheOnDemand, &bitmap) );
          
              {
                // Copy palette (only needed for some types of bitmap)
                Win32::com_ptr<IWICPalette> palette;
                Win32::hresult_succeeded( pIWICImagingFactory->CreatePalette(&palette) );
                if (SUCCEEDED( bitmapSource->CopyPalette(palette.get()) )) // may fail with WINCODEC_ERR_PALETTEUNAVAILABLE
                {          
                  Win32::hresult_succeeded( bitmap->SetPalette(palette.get()) );
                }
              }
          
              WICRect bitmapSourcePosition;
              if (clipMode & Clip_Center) {
                bitmapSourcePosition.X = (INT)(screenWidth  - bitmapWidth) / 2;
              }
              else if (clipMode & Clip_Right) {
                bitmapSourcePosition.X = (INT)(screenWidth  - bitmapWidth);
              }
              else {  // Clip_Left
                bitmapSourcePosition.X = 0;
              }
              if (clipMode & Clip_Middle) {
                bitmapSourcePosition.Y = (INT)(screenHeight - bitmapHeight) / 2;
              }
              else if (clipMode & Clip_Bottom) {
                bitmapSourcePosition.Y = (INT)(screenHeight - bitmapHeight);
              }
              else {    // Clip_Top
                bitmapSourcePosition.Y = 0;
              }
              bitmapSourcePosition.Width  = (INT)bitmapWidth;
              bitmapSourcePosition.Height = (INT)bitmapHeight;

              if (border == Border_White)
              {
                WICRect rc = {0, 0, screenWidth, screenHeight};
                fillWhite(bitmap, rc);
              }
              else 
              {
                WICRect rc = {0, 0, screenWidth, screenHeight};
                fillBlack(bitmap, rc);
              }

              {
                // copy bitmap               
                Win32::com_ptr<IWICBitmapLock> bitmapLock;
                Win32::hresult_succeeded( bitmap->Lock(&bitmapSourcePosition, WICBitmapLockWrite, &bitmapLock) );

                UINT stride;
                Win32::hresult_succeeded( bitmapLock->GetStride(&stride) );

                UINT   size;
                BYTE * pData;
                Win32::hresult_succeeded( bitmapLock->GetDataPointer(&size, &pData) );

                Win32::hresult_succeeded( bitmapSource->CopyPixels(nullptr, stride, size, pData) );
              }

              bitmapSource = std::move(bitmap);
            }        
          }
        }
        
        *pRetVal = bitmapSource.release();
      }



      std::vector<std::uint8_t> flatten(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, Protocol::EncodingMode encodingMode)
      {
        switch (encodingMode)
        {
          case Protocol::EncodingMode_1bit      : 
          case Protocol::EncodingMode_1bit_Zlib :
          case Protocol::EncodingMode_1bit_Bulk :
            return flattenMonochrome(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight);
          
          case Protocol::EncodingMode_16bit      : 
          case Protocol::EncodingMode_16bit_Bulk : 
            return flattenColor16(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight);

          case Protocol::EncodingMode_24bit      : 
          case Protocol::EncodingMode_24bit_Bulk : 
            return flattenColor24(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight);
        }
        throw std::out_of_range("invalid value for EncodingMode");
      }


      std::vector<std::uint8_t> resizeImageAndFlatten(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, Protocol::EncodingMode encodingMode, Scale scale, Border border, std::uint8_t clipMode)      
      {
        Win32::com_ptr<IWICBitmapSource> bitmap;
        resizeImage(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight, &bitmap, scale, border, clipMode);

        return flatten(pIWICImagingFactory, bitmap.get(), screenWidth, screenHeight, encodingMode);
      }

    } // namespace ProtocolHelper

  } // namespace STU

} // namespace WacomGSS
