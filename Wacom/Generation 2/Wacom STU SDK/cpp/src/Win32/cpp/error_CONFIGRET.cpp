#include <WacomGSS/Win32/cfgmgr32.hpp>
#include <sstream>

namespace WacomGSS
{
  namespace Win32
  {

    class CONFIGRET_error_category_impl : public std::error_category    
    {
      
    public:
      char const * name() const noexcept override
      {
        return "CONFIGRET";
      }
      
      std::string message(int ev) const override
      {
        std::stringstream s;
        s << static_cast<CONFIGRET>(ev);
        return s.str();
      }
    };



    static CONFIGRET_error_category_impl ec;

    std::error_category const & CONFIGRET_error_category() noexcept
    {      
      return ec;
    }


  }
}
