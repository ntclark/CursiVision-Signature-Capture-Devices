/// @file      WacomGSS/compatibility/optional.hpp
/// @copyright Copyright (c) 2018 Wacom Company Limited
/// @author    mholden
/// @date      2018-04-25
/// @brief     enables use of C++17 optional

#pragma once
#ifndef WacomGSS_optional_hpp
#define WacomGSS_optional_hpp

#include <WacomGSS/config.hpp>
#include <stdexcept>


#if defined(WacomGSS_compatibility_optional)

#if WacomGSS_compatibility_optional == 2 // emulation

#include <initializer_list>


namespace WacomGSS
{


  struct in_place_t { explicit in_place_t() = default; };


  constexpr in_place_t in_place{};



  template <class T> struct in_place_type_t { explicit in_place_type_t() = default; };



  //template <class T> constexpr in_place_type_t<T> in_place_type{};



  template <size_t I> struct in_place_index_t { explicit in_place_index_t() = default; };
  //template <size_t I> constexpr in_place_index_t<I> in_place_index{};



  class bad_optional_access : public std::exception { };



  struct nullopt_t 
  {
    explicit constexpr nullopt_t(int) {}
  };



  constexpr nullopt_t nullopt{0};



  template<typename T>
  class optional
  {
    bool m_present;
    T    m_value;

  public:
    typedef T value_type;

    optional() noexcept
    :
      m_present(false)
    {
    } 


    optional(nullopt_t) noexcept;


    optional(optional const & other)
      :
      m_present(other.m_present),
      m_value(m_value)
    {
    }


    template<class U>
    explicit optional(optional<U> const &);

    template<class U>
    explicit optional(optional<U> &&);

    template<class... Args>
    explicit optional(in_place_t, Args &&...);

    template<class U, class... Args>
    explicit optional(in_place_t, std::initializer_list<U>, Args &&...);


    template<class U = value_type>
    optional(U&& other)
    :
      m_present(true),
      m_value(std::forward<U>(other))
    {
    }
    

    optional& operator=(nullopt_t) noexcept
    {
      m_present = false;
      m_value = std::move(value_type());
      return *this;
    }


    optional& operator=(const optional& other)
    {
      m_present = other.m_present;
      m_value = other.m_value;
      return *this;
    }
    
    
    optional& operator=(optional&& other);
    
    
    template< class U = T >
    optional& operator=(U&& value)
    {
      m_present = true;
      m_value = std::move(value);
      return *this;
    }
    
   
    template< class U >
    optional& operator=(const optional<U>& other);
    
    template< class U >
    optional& operator=(optional<U>&& other);


    const T* operator -> () const
    {
      if (!m_present) throw bad_optional_access();
      return &m_value;
    }

    T* operator -> ()
    {
      if (!m_present) throw bad_optional_access();
      return &m_value;
    }

    const T& operator * () const { return value(); }
    T& operator * () { return value(); }

    //const T&& operator * () const;
    //T&& operator * ();


    operator bool () const noexcept
    {
      return m_present;
    }


    bool has_value() const noexcept
    {
      return m_present;
    }


    T& value()
    {
      if (!m_present) throw bad_optional_access();
      return m_value;
    }
    
    
    const T & value() const
    {
      if (!m_present) throw bad_optional_access();
      return m_value;
    }


    //T&& value();
    //const T&& value() const;
    

    template<class U> 
    T value_or(U&& default_value) const
    {
      if (m_present)
        return m_value;
      return default_value;
    }


    template< class U > 
    T value_or(U&& default_value)
    {
      if (m_present)
        return m_value;
      return std::forward<U>(default_value); 
    }
  };

} // namespace WacomGSS

#elif WacomGSS_compatibility_optional == 0

#pragma warning(push, 3)
#pragma warning(disable: 4582 4583)  // constructor/destructor not implicitly called
#include <optional>
#pragma warning(pop)

namespace WacomGSS
{
  using std::optional;
}


#else 

#error WacomGSS: macro 'WacomGSS_compatibility_optional' has not been set to an unexpected value. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif

#else

#error WacomGSS: macro 'WacomGSS_compatibility_optional' has not been defined. Check the compiler/platform configuration in <WacomGSS/config.hpp>

#endif


#endif // WacomGSS_optional_hpp
