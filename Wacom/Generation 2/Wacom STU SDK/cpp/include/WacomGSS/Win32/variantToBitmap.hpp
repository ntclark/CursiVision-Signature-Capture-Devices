#pragma once

#include <WacomGSS/Win32/com.hpp>
#include <WacomGSS/Win32/wincodec.hpp>

namespace WacomGSS
{
  namespace Win32
  {
    com_ptr<IWICBitmapSource> variantToBitmap(IWICImagingFactory * pIWICImagingFactory, VARIANT image);
    
    com_ptr<IWICBitmapSource> safearrayToBitmap(IWICImagingFactory * pIWICImagingFactory, uint8_t const * bytes, size_t length);
    com_ptr<IWICBitmapSource> safearrayToBitmap(IWICImagingFactory * pIWICImagingFactory, SAFEARRAY * image);
    com_ptr<IWICBitmapSource> ipictureToBitmap(IWICImagingFactory * pIWICImagingFactory, IPicture * image);
    com_ptr<IWICBitmapSource> bstrToBitmap(IWICImagingFactory * pIWICImagingFactory, BSTR image);

    com_ptr<IWICBitmapSource> istreamToBitmap(IWICImagingFactory * pIWICImagingFactory, IStream * image);
    com_ptr<IWICBitmapSource> fileNameToBitmap(IWICImagingFactory * pIWICImagingFactory, wchar_t const * image);
    com_ptr<IWICBitmapSource> urlToBitmap(IWICImagingFactory * pIWICImagingFactory, wchar_t const * image, size_t imageLength);

  }
}
