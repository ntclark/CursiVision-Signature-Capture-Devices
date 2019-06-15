/// @file      WacomGSS/STU/Error.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Basis of %STU-specific errors

#ifndef WacomGSS_STU_Error_hpp
#define WacomGSS_STU_Error_hpp

#include <WacomGSS/config.hpp>
#include <stdexcept>

namespace WacomGSS
{
  namespace STU
  {    
  
    /// @brief Base class for %STU-specific exceptions 
    class runtime_error : public std::runtime_error
    {
    public:
      /// @cond
      runtime_error(char const * message)
      :
        std::runtime_error(message)
      {
      }
      /// @endcond
    };

#if !defined(WacomGSS_DOXYGEN)
    #define WacomGSS_STU_runtime_error_decl(NAME)        \
      class NAME : public runtime_error                  \
      {                                                  \
        static char const * s_message;                   \
                                                         \
      public:                                            \
        NAME()                                           \
        :                                                \
          runtime_error(s_message)                       \
        {                                                \
        }                                                \
                                                         \
        static char const * set_message(char const * m); \
      };                                                


    #define WacomGSS_STU_runtime_error_impl(NAME,MSG)    \
      char const * NAME::s_message = MSG;                \
      char const * NAME::set_message(char const * m)     \
      {                                                  \
        char const * s = s_message;                      \
        s_message = m;                                   \
        return s;                                        \
      }
#endif

  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Error_hpp
