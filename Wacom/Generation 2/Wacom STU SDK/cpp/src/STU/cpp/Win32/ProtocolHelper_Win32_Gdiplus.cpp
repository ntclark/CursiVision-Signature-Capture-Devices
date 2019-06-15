#include <WacomGSS/STU/Win32/ProtocolHelper.hpp>

namespace WacomGSS
{
  namespace STU
  {    

    namespace ProtocolHelper
    {

      static constexpr bool isInk(Gdiplus::ARGB value)
      {        
        return ( ((value>>16)&0xff) | ((value>> 8)&0xff) | (value&0xff)) <= 0x80;
      }
      
      std::vector<uint8_t> flattenMonochrome(Gdiplus::Bitmap & bmp, uint16_t screenWidth, uint16_t screenHeight)
      {
        std::vector<uint8_t> pData;

        Gdiplus::Rect rc(0, 0, screenWidth, screenHeight);
        Gdiplus::BitmapData bd;
  
        Win32::Gdiplus_Ok(bmp.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppRGB, &bd));
        try
        {
          auto screenWidth_Aligned = (screenWidth & ~7) + (((screenWidth&7)!=0) << 3);
          
          pData.resize(static_cast<unsigned>(screenWidth_Aligned / 8 * screenHeight), 0xffu);

          Gdiplus::ARGB const * row = (Gdiplus::ARGB const *)bd.Scan0;
          // draw screen
          uint8_t * rawData = pData.data();
          
          for (unsigned bit = 0, y = 0; y < screenHeight; ++y, bit=0) 
          {
            for (unsigned x = 0; x < screenWidth; ++x) 
            {
              Gdiplus::ARGB rgb32 = row[x];

              uint8_t px = static_cast<uint8_t>( isInk(rgb32) );
  
              *rawData &= ~(px << (7-bit%8));
  
              rawData += (bit++ % 8)==7;
            }
            row += bd.Stride/sizeof(Gdiplus::ARGB);
  
            rawData += (bit&7)!=0;
          }
         
        }
        catch (...)
        {
          // ignore return
          bmp.UnlockBits(&bd);

          throw;
        }

        Win32::Gdiplus_Ok(bmp.UnlockBits(&bd));

        return std::move(pData);
      }


      static constexpr uint16_t rgb16_565(Gdiplus::ARGB value)
      {        
        return 
          static_cast<uint16_t>
          (
            (
                ((value >> 8) & 0x0000f800) // red
              | ((value >> 5) & 0x000007e0) // green
              | ((value >> 3) & 0x0000001f) // blue             
            )
            & 0xffff
          )
        ;
      }

      std::vector<uint8_t> flattenColor16(Gdiplus::Bitmap & bmp, uint16_t screenWidth, uint16_t screenHeight)
      {
        std::vector<uint8_t> pData;

        Gdiplus::Rect rc(0, 0, screenWidth, screenHeight);
        Gdiplus::BitmapData bd;

        Win32::Gdiplus_Ok(bmp.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppRGB, &bd));
        try
        {
          pData.reserve(screenWidth * 2u * screenHeight);
          
          Gdiplus::ARGB const * row = (Gdiplus::ARGB const *)bd.Scan0;
        
          for (unsigned y = 0; y < screenHeight; ++y) 
          {
            for (unsigned x = 0; x < screenWidth; ++x) 
            {
              Gdiplus::ARGB rgb32 = row[x];
              uint16_t      rgb16 = rgb16_565(rgb32);
            
              // Note change of byte order
              pData.push_back(static_cast<uint8_t>((rgb16 >> 8) & 0xff));
              pData.push_back(static_cast<uint8_t>((rgb16     ) & 0xff));                 
            }

            row += bd.Stride/sizeof(DWORD);
          }
        } 
        catch (...)
        {
          // ignore return
          bmp.UnlockBits(&bd);
  
          throw;
        }

        Win32::Gdiplus_Ok(bmp.UnlockBits(&bd));

        return std::move(pData);
      }



      std::vector<uint8_t> flattenColor24(Gdiplus::Bitmap & bmp, uint16_t screenWidth, uint16_t screenHeight)
      {
        std::vector<uint8_t> flattened;

        Gdiplus::Rect rc(0, 0, screenWidth, screenHeight);
        Gdiplus::BitmapData bd;

        Win32::Gdiplus_Ok(bmp.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat24bppRGB, &bd));
        try
        {
          flattened.reserve(screenWidth * 3u * screenHeight);
          
          auto row = reinterpret_cast<std::uint8_t const *>(bd.Scan0);

          for (unsigned y = 0; y < screenHeight; ++y) 
          {
            std::copy_n(row, screenWidth*3, std::back_inserter(flattened));
            row += bd.Stride;
          }
        } 
        catch (...)
        {
          // ignore return
          bmp.UnlockBits(&bd);
  
          throw;
        }

        Win32::Gdiplus_Ok(bmp.UnlockBits(&bd));

        return std::move(flattened);
      }


      std::vector<std::uint8_t> flatten(Gdiplus::Bitmap & bitmap, std::uint16_t screenWidth, std::uint16_t screenHeight, Protocol::EncodingMode encodingMode)
      {        
        switch (encodingMode)
        {
          case Protocol::EncodingMode_1bit      : 
          case Protocol::EncodingMode_1bit_Zlib :
          case Protocol::EncodingMode_1bit_Bulk :
            return flattenMonochrome(bitmap, screenWidth, screenHeight);
          
          case Protocol::EncodingMode_16bit      : 
          case Protocol::EncodingMode_16bit_Bulk : 
            return flattenColor16(bitmap, screenWidth, screenHeight);

          case Protocol::EncodingMode_24bit      : 
          case Protocol::EncodingMode_24bit_Bulk : 
            return flattenColor24(bitmap, screenWidth, screenHeight);
        }
        throw std::out_of_range("invalid value for EncodingMode");
      }


    } // namespace ProtocolHelper

  } // namespace STU

} // namespace WacomGSS
