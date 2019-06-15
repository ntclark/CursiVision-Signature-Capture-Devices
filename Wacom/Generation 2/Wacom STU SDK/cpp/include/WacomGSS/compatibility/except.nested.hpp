/// @file      WacomGSS/config/except.nested.hpp
/// @copyright Copyright (c) 2013 Wacom Company Limited
/// @author    mholden
/// @date      2013-08-08
/// @brief     std::nested_exception for compilers that don't have it.

#ifndef WacomGSS_config_except_nested_hpp
#define WacomGSS_config_except_nested_hpp

#include <exception>
#include <type_traits>

namespace std
{

  // 18.8.6 [except.nested]

  class nested_exception 
  {
    exception_ptr ptr;

  public:
    nested_exception() noexcept
    :
      ptr(current_exception())
    {
    }
    // [3] Effects: The constructor calls current_exception() and stores the returned value.


    nested_exception(const nested_exception& e) noexcept // = default
    :
      ptr(e.ptr)
    {
    } 


    nested_exception& operator=(const nested_exception& e) noexcept // = default
    {
      ptr = e.ptr;
      return *this;
    }


    virtual ~nested_exception() // = default
    {
    }


    // access functions

    WacomGSS_noreturn void rethrow_nested() const
    {
      if (ptr == nullptr)
      {
        terminate();
      }

      rethrow_exception(ptr);
    }
    // [4] Effects: If nested_ptr() returns a null pointer, the function calls terminate(). Otherwise, it throws
    //     the stored exception captured by *this.


    exception_ptr nested_ptr() const noexcept
    {
      return ptr;
    }
    // [5] Returns: The stored exception captured by this nested_exception object.
  };




  namespace WacomGSS_implementation
  {
    template<bool> struct throw_with_nested;

    template<> struct throw_with_nested<false>
    {
      template<class T> WacomGSS_noreturn static void apply(T&& t)
      {
        throw forward<T>(t);
      }
    };




    template<class T, class U> 
    struct nested_exception_t : nested_exception, U
    {
      nested_exception_t(T&& t)
      :
        U(std::forward<T>(t))
      {
      }
    };

    template<> struct throw_with_nested<true>
    {
      template<class T> WacomGSS_noreturn static void apply(T&& t)
      {
        throw nested_exception_t<T,typename remove_reference<T>::type>(forward<T>(t));
      }
    };

  }


  template<class T> WacomGSS_noreturn void throw_with_nested(T&& t)
  {
    typedef remove_reference<T>::type U;

    WacomGSS_implementation
      ::throw_with_nested< !is_base_of<nested_exception,U>::value >
        ::apply<T>(forward<T>(t));
  }
  //     Let U be remove_reference<T>::type.
  // [6] Requires: U shall be CopyConstructible.
  // [7] Throws: if U is a non-union class type not derived from nested_exception, an exception of unspecified
  //     type that is publicly derived from both U and nested_exception and constructed from
  //     std::forward<T>(t), otherwise std::forward<T>(t).



  template<class E> void rethrow_if_nested(const E& e)
  {
    const auto p = dynamic_cast<const nested_exception*>(&e);
    if (p)
    {
      p->rethrow_nested();
    }
  }
  // [8] Effects: If the dynamic type of e is publicly and unambiguously derived from nested_exception, calls
  //     dynamic_cast<const nested_exception&>(e).rethrow_nested().

} // namespace std


#endif
