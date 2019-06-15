/// @file      WacomGSS/noncopyable.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     implements the noncopyable pattern

#ifndef WacomGSS_noncopyable_hpp
#define WacomGSS_noncopyable_hpp

#include <WacomGSS/config.hpp>

namespace WacomGSS
{

#if defined(_MSC_VER) && _MSC_VER < 1900
  class noncopyable
  {
    noncopyable(noncopyable const &) = delete;
    noncopyable & operator = (noncopyable const &) = delete;

  protected:
    noncopyable() = default;        
    noncopyable(noncopyable &&) {}
  };
#else
  struct noncopyable
  {
    noncopyable() = default;
    noncopyable(noncopyable &&) = default;
    noncopyable(noncopyable const &) = delete;
    noncopyable & operator = (noncopyable const &) = delete;
    noncopyable & operator = (noncopyable &&) = default;
  };
#endif

}

#endif // WacomGSS_noncopyable_hpp
