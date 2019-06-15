#include <WacomGSS/STU/TlsInterface_Debug.hpp>
#include <WacomGSS/Win32/windows.hpp>


namespace WacomGSS
{
  namespace STU
  {


    void TlsInterface_Debug_OutputDebugString::output(char const * text)
    {
      ::OutputDebugStringA(text);
    }


  } // namespace STU
  
} // namespace WacomGSS

