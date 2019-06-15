#include <WacomGSS/Win32/variantToBitmap.hpp>
#include <WacomGSS/Win32/winhttp.hpp>
#include <WacomGSS/Win32/SafeArray.hpp>
#include <WacomGSS/Win32/wstringConvert.hpp>
#include <deque>
#include <sstream>
#include <iterator>
#include <vector>

namespace WacomGSS
{
  namespace Win32
  {


    com_ptr<IWICBitmapSource> variantToBitmap(IWICImagingFactory * pIWICImagingFactory, VARIANT image)
    {
      switch (image.vt)
      {
        case VT_BSTR:
          if (!image.bstrVal)
            throw_HRESULT_error(E_INVALIDARG, "variant string is null");
          return bstrToBitmap(pIWICImagingFactory, image.bstrVal);

        case VT_DISPATCH:
        case VT_UNKNOWN:
          {
            if (!image.punkVal)
              throw_HRESULT_error(E_INVALIDARG, "variant interface pointer is null");
   
            Win32::com_ptr<IPicture> pIPicture;
            if (SUCCEEDED(image.punkVal->QueryInterface(__uuidof(IPicture), &pIPicture)))
            {
              if (!pIPicture)
                Win32::throw_HRESULT_error(E_POINTER, "QueryInterface(IPicture) succeeded but returned nullptr");

              return ipictureToBitmap(pIWICImagingFactory, pIPicture.get());
            }
          }
          break;

        case VT_UI1|VT_ARRAY:
          if (!image.parray)
            throw_HRESULT_error(E_INVALIDARG, "variant byte array is null");
          return safearrayToBitmap(pIWICImagingFactory, image.parray);
      }
      throw_HRESULT_error(E_INVALIDARG, "Unable to convert variant to image");
    }


    com_ptr<IWICBitmapSource> istreamToBitmap(IWICImagingFactory * pIWICImagingFactory, IStream * image)
    {
      Win32::com_ptr<IWICBitmapDecoder> pIWICBitmapDecoder;
      Win32::hresult_succeeded(pIWICImagingFactory->CreateDecoderFromStream(image, nullptr, WICDecodeMetadataCacheOnDemand, &pIWICBitmapDecoder), "CreateDecoderFromStream");
      
      Win32::com_ptr<IWICBitmapFrameDecode> pIWICBitmapSource;  
      Win32::hresult_succeeded( pIWICBitmapDecoder->GetFrame(0, &pIWICBitmapSource), "GetFrame(0)");

      return std::move(pIWICBitmapSource);      
    }

    com_ptr<IWICBitmapSource> safearrayToBitmap(IWICImagingFactory * pIWICImagingFactory, uint8_t const * bytes, size_t length)
    {
      HGlobal hGlobal(::GlobalAlloc(GMEM_MOVEABLE, length));
      {
        HGlobalLock lock(hGlobal.get());        
        std::copy_n(bytes, length, stdext::make_checked_array_iterator(lock.get<std::uint8_t>(), length));
      }

      com_ptr<IStream> pIStream;
      {
        hresult_succeeded(CreateStreamOnHGlobal(hGlobal.get(), TRUE, &pIStream), "CreateStreamOnHGlobal");
        hGlobal.release();        
      }
      return std::move(istreamToBitmap(pIWICImagingFactory, pIStream.get()));
    }

    com_ptr<IWICBitmapSource> safearrayToBitmap(IWICImagingFactory * pIWICImagingFactory, SAFEARRAY * image)
    {
      SafeArrayAccess sa(image);
      
      if (!Win32::isByteArray(sa.sa()))
      {
        Win32::throw_HRESULT_error(E_INVALIDARG);        
      }
            
      return safearrayToBitmap(pIWICImagingFactory, sa.elements<std::uint8_t>(), sa.sa()->rgsabound[0].cElements);
    }



    com_ptr<IWICBitmapSource> ipictureToBitmap(IWICImagingFactory * pIWICImagingFactory, IPicture * image)
    {
      OLE_HANDLE hBitmap  = 0;
      OLE_HANDLE hPalette = 0;
      Win32::hresult_succeeded(image->get_Handle(&hBitmap), "IPicture.Handle");
      if (!hBitmap)
        Win32::throw_HRESULT_error(E_POINTER, "IPicture.Handle succeeded but returned nullptr");

      Win32::hresult_succeeded(image->get_hPal(&hPalette), "IPicture.hPal");
    
      Win32::com_ptr<IWICBitmap> pIWICBitmap;  
      Win32::hresult_succeeded(pIWICImagingFactory->CreateBitmapFromHBITMAP(reinterpret_cast<HBITMAP>(hBitmap), reinterpret_cast<HPALETTE>(hPalette), WICBitmapUseAlpha, &pIWICBitmap), "CreateBitmapFromHBITMAP");
      return std::move(pIWICBitmap);
    }



    com_ptr<IWICBitmapSource> bstrToBitmap(IWICImagingFactory * pIWICImagingFactory, BSTR image)
    {

      URL_COMPONENTS urlComponents = { sizeof(urlComponents) };
      urlComponents.dwSchemeLength = 0xffffffff;
  
      auto len = ::SysStringLen(image);
      if (::WinHttpCrackUrl(image, len, 0, &urlComponents) &&
        (urlComponents.nScheme == INTERNET_SCHEME_HTTP || urlComponents.nScheme == INTERNET_SCHEME_HTTPS))
      {
        return urlToBitmap(pIWICImagingFactory, image, len);
      }
      else
      {
        return fileNameToBitmap(pIWICImagingFactory, image);
      }
    }



    com_ptr<IWICBitmapSource> fileNameToBitmap(IWICImagingFactory * pIWICImagingFactory, wchar_t const * image)
    {
      Win32::com_ptr<IWICBitmapDecoder> pIWICBitmapDecoder;
      Win32::hresult_succeeded(pIWICImagingFactory->CreateDecoderFromFilename(image, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pIWICBitmapDecoder), "CreateDecoderFromFilename");

      Win32::com_ptr<IWICBitmapFrameDecode> pIWICBitmapSource;  
      Win32::hresult_succeeded( pIWICBitmapDecoder->GetFrame(0, &pIWICBitmapSource), "GetFrame(0)");

      return std::move(pIWICBitmapSource);
    }

    /*
    static std::vector<std::uint8_t> flatten(std::deque<std::vector<std::uint8_t>> & buffers)
    {
      typedef std::vector<std::uint8_t>::size_type size_type;

      // get total size needed
      size_type size = 0;      
      for (auto i = buffers.begin(); i != buffers.end(); ++i)
      {
        size += i->size();
      }

      std::vector<std::uint8_t> buffer(size);
      
      auto dst = buffer.begin();
      for (auto src = buffers.begin(); src != buffers.end(); ++src)
      {
        dst = std::copy(src->begin(), src->end(), dst);        
      }
      return std::move(buffer);
    }
    */

    static std::vector<wchar_t> queryHeader(HInternet & hRequest, DWORD dwInfoLevel)
    {
      DWORD dwBufferLength = 0;
      DWORD dwError;
      if (!WinHttpQueryHeaders(hRequest.get(), dwInfoLevel, WINHTTP_HEADER_NAME_BY_INDEX, WINHTTP_NO_OUTPUT_BUFFER, &dwBufferLength, WINHTTP_NO_HEADER_INDEX) && (dwError = GetLastError()) != ERROR_INSUFFICIENT_BUFFER)
      {
        throw_win32api_error(dwError, "WinHttpQueryHeaders");
      }

      dwBufferLength += 2;
      std::vector<wchar_t> buffer(dwBufferLength/sizeof(WCHAR));
      win32api_BOOL(WinHttpQueryHeaders(hRequest.get(), dwInfoLevel, WINHTTP_HEADER_NAME_BY_INDEX, buffer.data(), &dwBufferLength, WINHTTP_NO_HEADER_INDEX), "WinHttpQueryHeaders");

      // remove trailing nulls
      while (!buffer.empty() && buffer.back() == L'\0')
      {
        buffer.pop_back();
      }            
      return buffer;
    }

    com_ptr<IWICBitmapSource> urlToBitmap(IWICImagingFactory * pIWICImagingFactory, wchar_t const * image, size_t len)
    {
      if (!WinHttpCheckPlatform())
        throw_win32api_error(ERROR_NOT_SUPPORTED, "This platform does not support the WinHttp API");
            
      std::deque<std::vector<std::uint8_t>> buffers;
      std::vector<std::uint8_t>::size_type size = 0;

      {
        URL_COMPONENTS urlComponents = { sizeof(urlComponents) };
        urlComponents.dwHostNameLength = urlComponents.dwUrlPathLength = urlComponents.dwSchemeLength = 0xffffffff;
  
        win32api_BOOL(WinHttpCrackUrl(image, len, 0, &urlComponents), "WinHttpCrackUrl");

        std::wstring hostName(urlComponents.lpszHostName, urlComponents.dwHostNameLength);
        std::wstring urlPath(urlComponents.lpszUrlPath, urlComponents.dwUrlPathLength);
      
        HInternet hOpen(WinHttpOpen(L"WacomGSS.urlToBitmap/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0));

        HInternet hConnect(WinHttpConnect(hOpen.get(), hostName.c_str(), urlComponents.nPort, 0), "WinHttpConnect");

        wchar_t const * acceptTypes[] = { L"image/*", nullptr };

        DWORD dwFlags = (urlComponents.nScheme == INTERNET_SCHEME_HTTPS) ? DWORD(WINHTTP_FLAG_SECURE) : 0;

        HInternet hRequest(WinHttpOpenRequest(hConnect.get(), L"GET", urlPath.c_str(), nullptr, nullptr, acceptTypes, dwFlags), "WinHttpOpenRequest");

        win32api_BOOL(WinHttpSendRequest(hRequest.get(), WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, WINHTTP_IGNORE_REQUEST_TOTAL_LENGTH, 0), "WinHttpSendRequest");

        win32api_BOOL(WinHttpReceiveResponse(hRequest.get(), nullptr), "WinHttpReceiveResponse");

        // validate response
        {
          DWORD statusCode = 0;
          DWORD dwStatusCode = sizeof(DWORD);
          win32api_BOOL(WinHttpQueryHeaders(hRequest.get(), WINHTTP_QUERY_STATUS_CODE|WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &dwStatusCode, WINHTTP_NO_HEADER_INDEX), "WinHttpQueryHeaders(STATUS_CODE)");

          if (dwStatusCode == sizeof(DWORD) && statusCode != HTTP_STATUS_OK)
          {
            std::vector<wchar_t> statusText     = queryHeader(hRequest, WINHTTP_QUERY_STATUS_TEXT);
            std::string          statusTextUTF8 = wstring_to_string(statusText.data());

            std::stringstream o;
            o << "HTTP request failed with status " << statusCode << ": " << statusTextUTF8;

            throw std::runtime_error(o.str());          
          }

          std::vector<wchar_t> contentType = queryHeader(hRequest, WINHTTP_QUERY_CONTENT_TYPE);
            
          if (contentType.size() < 5 ||
              (  (contentType[0] != L'I' && contentType[0] != L'i')
              || (contentType[1] != L'M' && contentType[1] != L'm')
              || (contentType[2] != L'A' && contentType[2] != L'a')
              || (contentType[3] != L'G' && contentType[3] != L'g')
              || (contentType[4] != L'E' && contentType[4] != L'e')
              )
             )
          {
            std::string contentTypeUTF8 = wstring_to_string(contentType.data());
            std::stringstream o;
            o << "unexpected Content-Type: requested \"image/*\" but received \"" << contentTypeUTF8 << "\"";

            throw std::runtime_error(o.str());
            //return nullptr;
          }
        }

        for (;;)
        {
          DWORD dwBytesAvailable = 0;
          win32api_BOOL(WinHttpQueryDataAvailable(hRequest.get(), &dwBytesAvailable), "WinHttpQueryDataAvailable");
          if (dwBytesAvailable == 0)
            break;

          DWORD dwBytesRead = 0;
          std::vector<std::uint8_t> buffer1(dwBytesAvailable);
          win32api_BOOL(WinHttpReadData(hRequest.get(), &buffer1.front(), dwBytesAvailable, &dwBytesRead), "WinHttpReadData");
          if (dwBytesRead == 0)
            break;
          
          size += buffer1.size();
          buffers.push_back(std::move(buffer1));          
        }
      }

      HGlobal hGlobal(::GlobalAlloc(GMEM_MOVEABLE, size));
      {
        HGlobalLock lock(hGlobal.get());
        
        auto dst = stdext::make_unchecked_array_iterator(lock.get<std::uint8_t>());

        for (auto src = buffers.begin(); src != buffers.end(); ++src)
        {
          dst = std::copy(src->begin(), src->end(), dst);
        }
      }

      com_ptr<IStream> pIStream;
      {
        hresult_succeeded(CreateStreamOnHGlobal(hGlobal.get(), TRUE, &pIStream), "CreateStreamOnHGlobal");
        hGlobal.release();        
      }
      return std::move(istreamToBitmap(pIWICImagingFactory, pIStream.get()));
    }


  }
}
