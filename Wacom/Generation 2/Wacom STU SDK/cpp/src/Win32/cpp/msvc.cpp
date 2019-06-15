#include <WacomGSS/config.hpp>
#include <cstring>

namespace WacomGSS
{
  /// MSC06-CPP. Be aware of compiler optimization when dealing with sensitive data 
  /// https://www.securecoding.cert.org/confluence/display/cplusplus/MSC06-CPP.+Be+aware+of+compiler+optimization+when+dealing+with+sensitive+data
#pragma optimize("g",off)
  void * memset_s(void * dest, int value, std::size_t count)
  {
    return std::memset(dest, value, count);
  }
#pragma optimize("",on)
}
