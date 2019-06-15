/// @file      WacomGSS/unique_handle.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     provides the unique_handle pattern

#ifndef WacomGSS_unique_handle_hpp
#define WacomGSS_unique_handle_hpp

#include <WacomGSS/noncopyable.hpp>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4548) // expression before comma has no effect; expected expression with side-effect
#endif
#include <utility>
#include <exception>
#include <new>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace WacomGSS
{


  using std::nothrow_t;
  using std::nothrow;



  template<typename Type, typename Container>
  class pointer_initialize : noncopyable
  {
    Type        m_value;
    Container & m_container;

  public:
#if !defined(_MSC_VER) || (_MSC_VER>=1900)
    pointer_initialize(pointer_initialize&&) = default;
#endif
    pointer_initialize(Type const & value, Container & container) noexcept
    :
      m_value(value),
      m_container(container)
    {
    }

    pointer_initialize(Type && value, Container & container) noexcept
    :
      m_value(std::forward<Type &&>(value)),
      m_container(container)
    {
    }


    ~pointer_initialize() noexcept
    {
      m_container.reset(m_value, nothrow);
    }

    operator Type * () noexcept
    {
      return &m_value;
    }
  };



  template <typename Type, typename Traits>
  class unique_handle : noncopyable
  {
    Type m_value;

    bool operator == (unique_handle const &) const noexcept;
    bool operator != (unique_handle const &) const noexcept;

    void close() noexcept
    {
      if (*this)
      {
        auto value = m_value;
        m_value = Traits::invalid();
        Traits::close(value);
      }
    }
  
    struct boolean_value { int value; }; typedef int boolean_value::* boolean_type;

    typedef pointer_initialize<Type, unique_handle> pointer;

  public:
    typedef Type value_type;

    unique_handle() noexcept
    :
      m_value(Traits::invalid())
    {
    }

    explicit unique_handle(Type value, nothrow_t) noexcept 
    :
      m_value(value)
    {
    }
  
    explicit unique_handle(Type value)
    :
      m_value(value)
    {
      if (!*this) 
      {
        Traits::throw_invalid_handle_exception();
      }
    }

    template<typename Ty>
    explicit unique_handle(Type value, Ty _Message)
    :
      m_value(value)
    {
      if (!*this) 
      {
        Traits::throw_invalid_handle_exception(_Message);
      }
    }


    unique_handle(unique_handle && other) noexcept
    :
      m_value(other.release())
    {
    }

    ~unique_handle() noexcept
    {
      close();
    }

    unique_handle & operator = (unique_handle && other) noexcept
    {
      reset(other.release(), nothrow);
      return *this;
    }

    operator boolean_type () const noexcept
    {
      return Traits::is_valid(m_value) ? &boolean_value::value : nullptr;
    }

    Type get() const noexcept
    {
      return m_value;
    }

    void reset() noexcept
    {
      if (m_value != Traits::invalid())
      {
        close();
      }
    }

    void reset(Type value, nothrow_t) noexcept
    {
      if (m_value != value)
      {
        close();
        m_value = value;
      }
    }

    void reset(Type value)
    {
      if (m_value != value) 
      {
        close();
        m_value = value;
      }
      if (!*this)
      {
        Traits::throw_invalid_handle_exception();
      }
    }


    template<typename Ty>
    void reset(Type value, Ty _Message)
    {
      if (m_value != value) 
      {
        close();
        m_value = value;
      }
      if (!*this)
      {
        Traits::throw_invalid_handle_exception(_Message);
      }
    }

    Type release() noexcept
    {
      auto value = m_value;
      m_value = Traits::invalid();
      return value;
    }

    pointer initialize(nothrow_t) noexcept
    {
      return pointer(get(), *this);
    }

  };


} // namespace WacomGSS


#endif // WacomGSS_unique_handle_hpp
