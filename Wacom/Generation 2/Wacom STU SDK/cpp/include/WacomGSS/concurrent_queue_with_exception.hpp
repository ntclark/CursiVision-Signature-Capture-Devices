/// @file      WacomGSS/concurrent_queue_with_exception.hpp
/// @copyright Copyright (c) 2012 Wacom Company Limited
/// @author    mholden
/// @date      2012-02-03
/// @brief     template class that provides thread-safe producer/consumer pattern.
///
/// With thanks to: http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html

#ifndef WacomGSS_concurrent_queue_with_exception_hpp
#define WacomGSS_concurrent_queue_with_exception_hpp

#include <WacomGSS/compatibility/condition_variable.hpp>
#include <WacomGSS/compatibility/mutex.hpp>
#include <WacomGSS/compatibility/chrono.hpp>

namespace WacomGSS
{

  /// @brief A template class for handling a queue along with an exception, in thread-safe manner.
  template<typename Container, typename T>
  class concurrent_queue_with_exception
  {
    condition_variable  m_cv;
    mutex mutable       m_mutex;
    Container           m_container;
    std::exception_ptr  m_exception;
    bool                m_predicate;
 
    /// @brief Helper function to throw the held exception and to clear it.
    /// @pre m_mutex must be locked.
    void throw_exception_if_set()
    {
      if (m_exception != nullptr)
      {
        std::exception_ptr ex;
        std::swap(ex, m_exception);
        std::rethrow_exception(ex);
      }
    }

    /// @brief Helper function provide a common pattern of checking the to throw the held exception and to clear it.
    /// @param[out] value Object popped from queue if return is true.
    /// @return true if an object is returned.
    /// @pre m_mutex must be locked.
    bool pop_front_if_not_empty(T & value)
    {
      if (m_container.empty())
      {
        return false;
      }

      value = std::move(m_container.front());
      m_container.pop_front();
      return true;
    }

  public:
    concurrent_queue_with_exception() 
    :
      m_predicate(false)
    {
    }

    typedef Container container;
    typedef T         value_type;

    /// @brief Append an object by moving on to the queue.
    /// @param[in] value Object to be added.
    void push_back(T && value)
    {
      {
        lock_guard<mutex> lock(m_mutex);
        m_container.push_back(value);
      }
      m_cv.notify_all();
    }


    /// @brief Append an object by copying it to the queue.
    /// @param[in] value Object to be added.
    void push_back(T const & value)
    {
      {
        lock_guard<mutex> lock(m_mutex);
        m_container.push_back(value);
      }
      m_cv.notify_all();
    }
    
    
    /// @brief Sets queue to an exception state.
    ///
    /// @param[in] ex Exception to be set.
    /// @note This ignores the current state of the queue. If an exception was already set, then it is ignored and replaced by this exception.
    /// @returns true if the exception was set and signal or false if some error occurred.
    bool push_back(std::exception_ptr const & ex) noexcept
    {
      try
      {
        {
          lock_guard<mutex> lock(m_mutex);
          m_exception = ex;
        }
        m_cv.notify_all();
      }
      catch (...)
      {
        return false;
      }
      return true;
    }


    /// @brief Empties the queue and removes any pending exception.
    void clear()
    {
      {
        lock_guard<mutex> lock(m_mutex);
        m_container.clear();
        m_exception = nullptr;
      }
      m_cv.notify_all();
    }


    /// @brief Explicitly wakes a waiting consumer.
    void notify_one() noexcept
    {
      m_cv.notify_one();
    }


    /// @brief Explicitly wakes all waiting consumers.
    void notify_all() noexcept
    {
      m_cv.notify_all();
    }

    
    /// @brief Checks the contents of the queue.
    ///
    /// @note Does not check the state of any pending exception
    /// @return true if the queue is empty or false if there is at least one item in it.
    /// @warning When used in a multi-threaded environment, take care to avoid race conditions, as another thread may clear the queue after this has been called.
    bool empty() const
    {
      lock_guard<mutex> lock(m_mutex);
      return m_container.empty();
    }
   

    /// @brief Attempts to remove the front value from the queue, if present.
    /// @param[out] value The returned object if set.
    ///
    /// @return true if @param value is set.
    /// @throw Any exception set by the producer or any standard exception.
    bool try_pop(T & value)
    {
      lock_guard<mutex> lock(m_mutex);

      throw_exception_if_set();

      return pop_front_if_not_empty(value);
    }


    /// @brief Wait for a value to become available and returns it.
    /// @param[out] value The returned object.
    ///
    /// @throw Any exception set by the producer or any standard exception.
    void wait_pop(T & value)
    {
      unique_lock<mutex> lock(m_mutex);
      while (m_exception == nullptr && m_container.empty())
      {
        m_cv.wait(lock);
      }

      throw_exception_if_set();

      value = std::move(m_container.front());
      m_container.pop_front();
    }


    /// @brief  Gets the current predicate value used during wait
    bool get_predicate() const noexcept
    {
      return m_predicate;
    }


    /// @brief  Sets the predicate value used during wait
    ///
    /// @param  pred    Predicate to check during wait
    void set_predicate(bool predicate)
    {
      lock_guard<mutex> lock(m_mutex);
      m_predicate = predicate;
    }
        

    /// @brief Wait for a value to become available or pred returns false.
    /// @param[out] value The returned object if set.
    /// @param pred A user-defined predicate to stop waiting on.
    ///
    /// @return true if @param value is set.
    /// @throw Any exception set by the producer or any standard exception.    
    bool wait_pop_predicate(T & value)
    {
      unique_lock<mutex> lock(m_mutex);
      while (!m_predicate && m_exception == nullptr && m_container.empty())
      {
        m_cv.wait(lock);
      }

      throw_exception_if_set();

      return pop_front_if_not_empty(value);
    }

    template<typename Predicate>
    bool wait_pop_BROKEN_DO_NOT_USE(T & value, Predicate pred)
    {
      unique_lock<mutex> lock(m_mutex);
      while (m_exception == nullptr && m_container.empty() && !pred())
      {
        m_cv.wait(lock);
      }

      throw_exception_if_set();

      return pop_front_if_not_empty(value);
    }


    /// @brief Wait for a value to become available and returns it.
    /// @param[out] value The returned object if set.
    /// @param abs_time An absolute time to wait until.
    ///
    /// @return true if @param value is set.
    /// @throw Any exception set by the producer or any standard exception.
    template <typename Clock, typename Duration>
    cv_status wait_until_pop(T & value, chrono::time_point<Clock, Duration> const & abs_time)
    {
      unique_lock<mutex> lock(m_mutex);
      while (m_exception == nullptr && m_container.empty())
      {
        if (m_cv.wait_until(lock, abs_time) == cv_status::timeout)
        {
          return cv_status::timeout;
        }
      }
    
      throw_exception_if_set();
    
      value = std::move(m_container.front());
      m_container.pop_front();
      
      return cv_status::no_timeout;
    }

    /// @brief Wait for a value to become available or and returns it.
    /// @param[out] value The returned object if set.
    /// @param abs_time An absolute time to wait until.
    /// @param pred A user-defined predicate to stop waiting on.
    ///
    /// @return true if @param value is set.
    /// @throw Any exception set by the producer or any standard exception.
    template <typename Clock, typename Duration, typename Predicate>
    bool wait_until_pop_BROKEN_DO_NOT_USE(T & value, chrono::time_point<Clock, Duration> const & abs_time, Predicate pred);


    /// @brief Wait for a value to become available or and returns it.
    /// @param[out] value The returned object if set.
    /// @param rel_time An relative time to wait until for.
    ///
    /// @return true if @param value is set.
    /// @throw Any exception set by the producer or any standard exception.
    template <typename Rep, typename Period>
    cv_status wait_for_pop(T & value, chrono::duration<Rep, Period> const & rel_time);
    
        
    /// @brief Wait for a value to become available or and returns it.
    /// @param[out] value The returned object if set.
    /// @param rel_time An relative time to wait until for.
    /// @param pred A user-defined predicate to stop waiting on.
    ///
    /// @return true if @param value is set.
    /// @throw Any exception set by the producer or any standard exception.
    template <typename Rep, typename Period, typename Predicate>
    bool wait_for_pop_BROKEN_DO_NOT_USE(T & value, chrono::duration<Rep, Period> const & rel_time, Predicate pred);
  };
  
  //template <typename Rep, typename Period>
  //bool wait_for_getReport(Report & report, chrono::duration<Rep,Period> const & relTime)
  //{
  //  boost::system_time expiryTime = boost::get_system_time() + relTime;
  //
  //  unique_lock<mutex> lock(m_dataMutex);
  //  
  //  while (m_dataException == nullptr && m_dataQueue.empty())
  //  {
  //    if (!m_cv.timed_wait(lock, expiryTime))
  //      return false;
  //  }
  //
  //  if (m_dataException != nullptr)
  //  {
  //    std::exception_ptr ex;
  //    std::swap(ex, m_dataException);
  //    std::rethrow_exception(ex);
  //  }
  //
  //  report = std::move(m_dataQueue.front());
  //  m_dataQueue.pop_front();
  //
  //  return true;
  //}


} // namespace WacomGSS

#endif // WacomGSS_concurrent_queue_with_exception_hpp
