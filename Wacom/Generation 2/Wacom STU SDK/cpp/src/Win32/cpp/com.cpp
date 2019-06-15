#include <WacomGSS/Win32/com.hpp>
#include <WacomGSS/Win32/wstringConvert.hpp>
#include <iostream>



namespace WacomGSS
{
  namespace Win32
  {
    namespace Bstr_ostream_operator_utf8
    {


      std::ostream & operator << (std::ostream & o, Bstr const & bstr)
      {
        o << wstring_to_string(bstr.get());
        return o;
      }


    } // namespace Bstr_ostream_operator_utf8
  } // namespace Win32
} // namespace WacomGSS
