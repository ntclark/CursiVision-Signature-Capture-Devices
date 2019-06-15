/// @file      wstringConvert.hpp
/// @copyright Copyright(c) 2018 Wacom Co., Ltd.
/// @author    KParkes
/// @date      2018-11-01
/// @brief     

#ifndef wstringConvert_hpp
#define wstringConvert_hpp

#pragma once
#include <string>

namespace WacomGSS
{
  namespace Win32
  {
    std::string wstring_to_string(std::wstring const & wstr);
    std::string wstring_to_string(wchar_t const * wstr);

    std::wstring string_to_wstring(std::string const & str);
    std::wstring string_to_wstring(char const * str);
  }
}

#endif    // wstringConvert_hpp
