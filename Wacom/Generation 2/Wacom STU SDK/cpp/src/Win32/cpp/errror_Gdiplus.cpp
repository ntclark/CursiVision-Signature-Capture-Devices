#include <WacomGSS/Win32/Gdiplus.hpp>
#include <sstream>

namespace WacomGSS
{
  namespace Win32
  {

    class Gdiplus_error_category_impl : public std::error_category
    {
    public:
      char const * name() const noexcept override
      {
        return "Gdiplus";
      }

      std::string message(int ev) const override
      {
        std::stringstream s;
        s << static_cast<Gdiplus::Status>(ev);
        return s.str();
      }
    };

    static Gdiplus_error_category_impl ec;

    std::error_category const & Gdiplus_error_category() noexcept
    {      
      return ec;
    }


  }
}
