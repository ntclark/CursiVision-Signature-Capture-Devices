#include <WacomGSS/Win32/ntstatus.hpp>
#include <sstream>

namespace WacomGSS
{
  namespace Win32
  {

    class NTSTATUS_error_category_impl : public std::error_category
    {
    public:
      char const * name() const noexcept override
      {
        return "NTSTATUS";
      }
      
      std::string message(int ev) const override;
    };

    std::string NTSTATUS_error_category_impl::message(int ev) const
    {    
      std::stringstream s;
      s << static_cast<NTSTATUS>(ev);
      return s.str();
    }


    static NTSTATUS_error_category_impl ec;

    std::error_category const & NTSTATUS_error_category() noexcept
    {      
      return ec;
    }

  }
}
