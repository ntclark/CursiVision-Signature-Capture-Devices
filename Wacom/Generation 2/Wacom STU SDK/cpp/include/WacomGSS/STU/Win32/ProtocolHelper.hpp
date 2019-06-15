/// @file      WacomGSS/STU/Win32/ProtocolHelper.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides higher level assistance to using the STU tablet protocol.

#ifndef WacomGSS_STU_Win32_ProtocolHelper_hpp
#define WacomGSS_STU_Win32_ProtocolHelper_hpp

#include <WacomGSS/STU/ProtocolHelper.hpp>
#include <WacomGSS/Win32/gdiplus.hpp>
#include <WacomGSS/Win32/wincodec.hpp>
#include <WacomGSS/Win32/com.hpp>
#include <vector>


namespace WacomGSS
{
  namespace STU
  {    

    namespace ProtocolHelper
    {
      /// @name Image support
      /// @{

      /// @brief  Takes a Gdiplus::Bitmap and converts it into the data to be sent to a tablet with a monochrome screen.
      ///
      /// @param  bmp           Gdiplus bitmap.
      /// @param  screenWidth   width of the tablet's LCD screen in pixels
      /// @param  screenHeight  height of the tablet's LCD screen in pixels
      /// @return image data suitable for sending to the tablet
      ///
      /// @remark It is beyond the scope of the SDK to implement compression. For Zlib-compressed monochrome encoding mode, 
      ///         the data returned by this method must be compressed by your own code or a third-party library.
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<uint8_t> flattenMonochrome(Gdiplus::Bitmap & bmp, std::uint16_t screenWidth, std::uint16_t screenHeight);

      /// @brief  Takes a Gdiplus Bitmap and converts it into the data to be sent to a tablet with a color screen.
      ///
      /// @param  bmp           Gdiplus bitmap.
      /// @param  screenWidth   width of the tablet's LCD screen in pixels
      /// @param  screenHeight  height of the tablet's LCD screen in pixels
      /// @return image data suitable for sending to the tablet
      ///
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<uint8_t> flattenColor16(Gdiplus::Bitmap & bmp, std::uint16_t screenWidth, std::uint16_t screenHeight);

      /// @deprecated Superseded by flattenColor16(Gdiplus::Bitmap &, std::uint16_t, std::uint16_t)
      WacomGSS_deprecated
      inline std::vector<uint8_t> flattenColor16_565(Gdiplus::Bitmap & bmp, std::uint16_t screenWidth, std::uint16_t screenHeight)
      {
        return flattenColor16(bmp, screenWidth, screenHeight);
      }

      /// @brief  Takes a Gdiplus Bitmap and converts it into the data to be sent to a tablet with a color screen.
      ///
      /// @param  bmp           Gdiplus bitmap.
      /// @param  screenWidth   width of the tablet's LCD screen in pixels
      /// @param  screenHeight  height of the tablet's LCD screen in pixels
      /// @return image data suitable for sending to the tablet
      ///
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<uint8_t> flattenColor24(Gdiplus::Bitmap & bmp, std::uint16_t screenWidth, std::uint16_t screenHeight);


      /// @brief  Takes a Gdiplus Bitmap and converts it into the data to be sent to a tablet with the specified encoding type.
      ///
      /// @param  bmp           Gdiplus bitmap.
      /// @param  screenWidth   width of the tablet's LCD screen in pixels
      /// @param  screenHeight  height of the tablet's LCD screen in pixels
      /// @param  encodingType  determines the type of bitmap to flatten to
      /// @return image data suitable for sending to the tablet
      ///
      /// @remark It is beyond the scope of the SDK to implement compression. For Zlib-compressed monochrome encoding mode, 
      ///         the data returned by this method must be compressed by your own code or a third-party library.
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<std::uint8_t> flatten(Gdiplus::Bitmap & bitmap, std::uint16_t screenWidth, std::uint16_t screenHeight, Protocol::EncodingMode encodingMode);

      /// @deprecated Superseded by flatten(Gdiplus::Bitmap &, std::uint16_t, std::uint16_t, Protocol::EncodingMode)
      WacomGSS_deprecated
      inline std::vector<std::uint8_t> flatten(Gdiplus::Bitmap & bitmap, std::uint16_t screenWidth, std::uint16_t screenHeight, bool color)
      {
        return color ? flattenColor16(bitmap, screenWidth, screenHeight)
                     : flattenMonochrome(bitmap, screenWidth, screenHeight);
      }

      /// @brief  Takes a WIC (Windows Imaging Component) bitmap and converts it into the data to be sent to a tablet with a monochrome screen.
      ///
      /// @param  pIWICImagingFactory pointer to an IWICImagingFactory object. Must not be null.
      /// @param  pIWICBitmapSource   pointer to the bitmap to convert. Must not be null.
      /// @param  screenWidth         width of the tablet's LCD screen in pixels
      /// @param  screenHeight        height of the tablet's LCD screen in pixels
      /// @return image data suitable for sending to the tablet
      /// @remark It is beyond the scope of the SDK to implement compression. For Zlib-compressed monochrome encoding mode, 
      ///         the data returned by this method must be compressed by your own code or a third-party library.
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<uint8_t> flattenMonochrome(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight);

      /// @brief  Takes a WIC (Windows Imaging Component) bitmap and converts it into the data to be sent to a tablet with a color screen.
      /// @param  pIWICImagingFactory pointer to an IWICImagingFactory object. Must not be null.
      /// @param  pIWICBitmapSource   pointer to the bitmap to convert. Must not be null.
      /// @param  screenWidth         width of the tablet's LCD screen in pixels
      /// @param  screenHeight        height of the tablet's LCD screen in pixels
      /// @return image data suitable for sending to the tablet
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<uint8_t> flattenColor16(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight);

      /// @deprecated Superseded by flattenColor16(IWICImagingFactory * , IWICBitmapSource * , std::uint16_t , std::uint16_t )
      WacomGSS_deprecated
      inline std::vector<uint8_t> flattenColor16_565(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight)
      {
        return flattenColor16(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight);
      }

      /// @brief  Takes a WIC (Windows Imaging Component) bitmap and converts it into the data to be sent to a tablet with a color screen.
      /// @param  pIWICImagingFactory pointer to an IWICImagingFactory object. Must not be null.
      /// @param  pIWICBitmapSource   pointer to the bitmap to convert. Must not be null.
      /// @param  screenWidth         width of the tablet's LCD screen in pixels
      /// @param  screenHeight        height of the tablet's LCD screen in pixels
      /// @return image data suitable for sending to the tablet
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<uint8_t> flattenColor24(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight);

      /// @brief  Takes a WIC (Windows Imaging Component) bitmap and converts it into the data to be sent to a tablet.
      ///
      /// @param  pIWICImagingFactory pointer to an IWICImagingFactory object. Must not be null.
      /// @param  pIWICBitmapSource   pointer to the bitmap to convert. Must not be null.
      /// @param  screenWidth         width of the tablet's LCD screen in pixels
      /// @param  screenHeight        height of the tablet's LCD screen in pixels
      /// @param  color               indicates whether the tablet has a color or monochrome LCD screen
      /// @return image data suitable for sending to the tablet
      ///
      /// @remark It is beyond the scope of the SDK to implement compression. For Zlib-compressed monochrome encoding mode, 
      ///         the data returned by this method must be compressed by your own code or a third-party library.
      /// @remark It is undefined what action is taken if the bitmap size does not match the supplied screen dimensions.
      std::vector<std::uint8_t> flatten(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, Protocol::EncodingMode encodingMode);

      /// @deprecated Superseded by flatten(IWICImagingFactory * , IWICBitmapSource * , std::uint16_t , std::uint16_t , Protocol::EncodingMode )
      WacomGSS_deprecated
      inline std::vector<std::uint8_t> flatten(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, bool color)
      {
        return color ? flattenColor16(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight)
                     : flattenMonochrome(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight);
      }
      

      /// @brief Resizes an image ready for flattening.
      ///
      /// @param  pIWICImagingFactory pointer to an IWICImagingFactory object. Must not be null.
      /// @param  pIWICBitmapSource   pointer to the bitmap to resize. Must not be null.
      /// @param  screenWidth         width of the tablet's LCD screen in pixels
      /// @param  screenHeight        height of the tablet's LCD screen in pixels
      /// @param  pRetVal             a pointer that receives a pointer to the resized image
      /// @param  scale               specifies scaling mode. See #Scale.
      /// @param  border              color to use for any added borders. See #Border
      /// @param  clipMode            combination of #Clip values specifying which portion of the image to use if scale is
      ///                             Scale_Clip and the image (or rectangle) is larger than the tablet's display.
      ///
      /// @remark This method does nothing (except AddRef pIWICBitmapSource) if scale == Scale_Stretch as #resizeImageAndFlatten can
      ///         automatically handle that case.
      void resizeImage(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, IWICBitmapSource * * pRetVal, Scale scale = Scale_Fit, Border border = Border_White, std::uint8_t clipMode = Clip_Left|Clip_Top);


      /// @brief Resizes and flattens an image in one operation.
      ///
      /// @param  pIWICImagingFactory pointer to an IWICImagingFactory object. Must not be null.
      /// @param  pIWICBitmapSource   pointer to the bitmap to resize. Must not be null.
      /// @param  screenWidth         width of the tablet's LCD screen in pixels
      /// @param  screenHeight        height of the tablet's LCD screen in pixels
      /// @param  color               indicates whether the tablet has a color or monochrome LCD screen
      /// @param  scale               specifies scaling mode. See #Scale.
      /// @param  border              color to use for any added borders. See #Border
      /// @param  clipMode            combination of #Clip values specifying which portion of the image to use if scale is
      ///                             #Scale_Clip and the image (or rectangle) is larger than the tablet's display.
      /// @return image data suitable for sending to the tablet
      ///
      /// @remark It is beyond the scope of the SDK to implement compression. For Zlib-compressed monochrome encoding mode, 
      ///         the data returned by this method must be compressed by your own code or a third-party library.
      std::vector<std::uint8_t> resizeImageAndFlatten(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, Protocol::EncodingMode encodingMode, Scale scale = Scale_Fit, Border border = Border_White, std::uint8_t clipMode = Clip_Left|Clip_Top);

      /// @deprecated Superseded by resizeImageAndFlatten(IWICImagingFactory * , IWICBitmapSource * , std::uint16_t , std::uint16_t , Protocol::EncodingMode , Scale , Border , std::uint8_t )
      WacomGSS_deprecated
      inline std::vector<std::uint8_t> resizeImageAndFlatten(IWICImagingFactory * pIWICImagingFactory, IWICBitmapSource * pIWICBitmapSource, std::uint16_t screenWidth, std::uint16_t screenHeight, bool color, Scale scale = Scale_Fit, Border border = Border_White, std::uint8_t clipMode = Clip_Left|Clip_Top)
      {
        Win32::com_ptr<IWICBitmapSource> bitmap;
        resizeImage(pIWICImagingFactory, pIWICBitmapSource, screenWidth, screenHeight, &bitmap, scale, border, clipMode);
        return color ? flattenColor16(pIWICImagingFactory, bitmap.get(), screenWidth, screenHeight)
                     : flattenMonochrome(pIWICImagingFactory, bitmap.get(), screenWidth, screenHeight);
      }
      

      /// @}

    } // namespace ProtocolHelper

  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Win32_ProtocolHelper_hpp
