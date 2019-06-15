#include <WacomGSS/Win32/com.hpp>
#include <sstream>
#include <iomanip>
//#include <locale>
//#include <codecvt>

namespace WacomGSS
{
  namespace Win32
  {

    class HRESULT_error_category_impl : public std::error_category
    {
    public:
      char const * name() const noexcept override
      {
        return "HRESULT";
      }
      
      std::string message(int ev) const override;
    };

    
    /*
    static std::wstring get_error()
    {
      std::wstring description;

      IErrorInfo * pIErrorInfo = nullptr;
      HRESULT hr = ::GetErrorInfo(0, &pIErrorInfo);
      if (SUCCEEDED(hr))
      {
        if (pIErrorInfo) 
        {
          if (hr == S_OK) 
          { 
            BSTR bstr = nullptr;
            hr = pIErrorInfo->GetDescription(&bstr);
            if (SUCCEEDED(hr)) 
            {
              if (bstr) 
              {
                try
                {
                  description.append(bstr);
                }
                catch (...)
                {
                }
                ::SysFreeString(bstr);
              }

            }
          }
          pIErrorInfo->Release();
        }
      }
      
      return description;
    }
    */


    std::string HRESULT_error_category_impl::message(int ev) const
    {    
      PSTR pBuffer = nullptr;
      try
      {       
        std::string ret;
        
        //std::wstring wret = get_error();
        //if (!wret.empty())
        //{
        //  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        //  ret = conv.to_bytes(wret);
        //}
        //else
        {
          DWORD dw = ::FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
            nullptr,
            static_cast<DWORD>( static_cast<HRESULT>(ev) ),
            0,
            reinterpret_cast<PSTR>(&pBuffer),
            0,
            nullptr);
          if (dw != 0)
          {
            ret = pBuffer;
          }
          else
          {
            std::stringstream ss;
            ss << std::showbase << std::setfill('0') << std::setw(8) << std::hex << static_cast<HRESULT>(ev);
            ret = ss.str();
          }

          if (pBuffer)
          {
            ::LocalFree(pBuffer);
            pBuffer = nullptr;
          }
        }

        return ret;
      }
      catch (...)
      {
        if (pBuffer)
        {
          ::LocalFree(pBuffer);
        }
        throw;
      }
    }


    
    static HRESULT_error_category_impl ec;

    std::error_category const & HRESULT_error_category() noexcept
    {      
      return ec;
    }

  }
}
