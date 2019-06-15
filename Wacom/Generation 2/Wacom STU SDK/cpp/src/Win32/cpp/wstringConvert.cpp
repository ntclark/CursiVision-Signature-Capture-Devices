/// @file      wstringConvert.cpp
/// @copyright Copyright(c) 2018 Wacom Co., Ltd.
/// @author    KParkes
/// @date      2018-11-01
/// @brief     

// std::wstring_convert is deprecated in C++17 and the C++ Standard doesn't provide equivalent
// non-deprecated functionality

#if _MSC_VER > 1900

#include <yvals.h> //yvals_core

#if !_HAS_CXX17
#define USE_CODECVT
#endif

#endif

#if defined(USE_CODECVT)
#include <codecvt>
#include <locale>
#else
#include <WacomGSS/Win32/windows.hpp>
#include <vector>
#include <winnls.h> 
#endif

namespace WacomGSS
{

  namespace Win32
  {
    std::string wstring_to_string(wchar_t const * wstr) {
#if defined(USE_CODECVT)

      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> conv;

      return conv.to_bytes(wstr);
#else
      auto len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, NULL, NULL);
      std::vector<char> data((unsigned)len);

      WideCharToMultiByte(CP_UTF8, 0, wstr, -1, data.data(), len, NULL, NULL);
      return std::string(data.begin(), data.end());
#endif
    }

    std::string wstring_to_string(std::wstring const & wstr) {
      return wstring_to_string(wstr.c_str());
    }


    std::wstring string_to_wstring(char const * str) {
#if defined(USE_CODECVT)

      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> conv;

      return conv.from_bytes(str);
#else
      auto len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
      std::vector<wchar_t>  data((unsigned)len);      

      MultiByteToWideChar(CP_UTF8, 0, str, -1, data.data(), len);
      return std::wstring(data.begin(), data.end());
#endif
    }

    std::wstring string_to_wstring(std::string const & str) {
      return string_to_wstring(str.c_str());
    }


  }
}


