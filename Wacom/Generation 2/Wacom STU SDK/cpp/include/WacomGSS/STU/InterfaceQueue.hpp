/// @file      WacomGSS/STU/InterfaceQueue.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides access to data that the tablet sends.
#ifndef WacomGSS_STU_InterfaceQueue_hpp
#define WacomGSS_STU_InterfaceQueue_hpp

#include <WacomGSS/STU/InterfaceImpl.hpp>


namespace WacomGSS
{
  namespace STU
  {

    /// @brief Provides a report queue bound to an Interface.
    ///
    /// @see class Interface for more details.
    class InterfaceQueue : WacomGSS::noncopyable
    {
      std::weak_ptr<InterfaceImpl> m_interfaceData;
      InterfaceImpl::shared_queue  m_interfaceQueue;
      
    public:
      InterfaceQueue();
      InterfaceQueue(std::shared_ptr<InterfaceImpl> & data);
      InterfaceQueue(InterfaceQueue && other) noexcept;
      ~InterfaceQueue();
            
      InterfaceQueue & operator = (InterfaceQueue && other);
      
      /// @brief Adds a packet or sets the exception in the queue.
      void push_back(Report && report)
      {
        m_interfaceQueue->push_back(report);
      }
      
      void push_back(Report const & report)      
      {
        m_interfaceQueue->push_back(report);
      }

      /// @brief Clears all data and exception pointer if present.
      void clear()
      {
        m_interfaceQueue->clear();
      }      

      /// @brief States whether there are Reports waiting to be retrieved or the object is in an error state.
      bool empty()
      {
        return m_interfaceQueue->empty();
      }
      
      /// @brief Releases the embedded condition variable so that any supplied predicate can be checked.
      void notify_one()
      {
        m_interfaceQueue->notify_one();
      }

      /// @brief Releases the embedded condition variable so that any supplied predicate can be checked.
      void notify_all()
      {
        m_interfaceQueue->notify_all();
      }
            
      /// @brief  Retrieves the next Report in the queue.
      ///
      /// @param  report  Report object in which to return retrieved report
      /// @return true if a report was returned; false if no report is available.
      ///
      /// @remark If the object is in an error state, this will throw the pending
      ///         exception.
      bool try_getReport(Report & report)
      {
        return m_interfaceQueue->try_pop(report);
      }
                  
      /// @brief  Retrieves the next Report in the queue.
      ///
      /// @param  report  Report object in which to return retrieved report
      ///
      /// @throw  std::exception where the internal reader codes errors.
      /// @remark If the object is in an error state, this will throw the pending exception.
      void wait_getReport(Report & report)
      {
        m_interfaceQueue->wait_pop(report);
      }
      

      /// @brief  Retrieves the current predicate value used during wait
      bool get_predicate() const noexcept
      {
        return m_interfaceQueue->get_predicate();
      }


      /// @brief  Sets the predicate value used during wait
      ///
      /// @param  predicate  Predicate to check during wait
      void set_predicate(bool predicate)
      {
        m_interfaceQueue->set_predicate(predicate);
      }

      /// @brief  Waits for a report or the predicate to be set
      ///
      /// @param  report  Report object in which to return retrieved report
      ///
      /// @throw  std::exception where the internal reader codes errors.
      /// @remark If the object is in an error state, this will throw the pending exception.      
      bool wait_getReport_predicate(Report & report)
      {
        return m_interfaceQueue->wait_pop_predicate(report);
      }

      /// @brief Waits for a report or user-defined predicate
      /// @warning Maintained for backwards compatibility. This functionality is broken and can result in a hang.
      template<typename Predicate>
      bool wait_getReport_BROKEN_DO_NOT_USE(Report & report, Predicate pred)
      {
        return m_interfaceQueue->wait_pop_BROKEN_DO_NOT_USE(report, pred);
      }

      /// @brief  Waits for a report or a timeout
      ///
      /// @param  report  Report object in which to return retrieved report
      /// @param  absTime boost::chrono::time_point specifying time at which to abandon wait
      ///
      /// @return true if a report was returned; false if wait timed out.
      ///
      /// @throw  std::exception where the internal reader codes errors.
      /// @remark If the object is in an error state, this will throw the pending exception.
      template<typename Clock, typename Duration>
      bool wait_until_getReport(Report & report, chrono::time_point<Clock,Duration> const & absTime)
      {
        return m_interfaceQueue->wait_until_pop(report, absTime) == cv_status::no_timeout;
      }

      /// @brief  Waits for a report or a timeout
      ///
      /// @param  report  Report object in which to return retrieved report
      /// @param  relTime boost::chrono::duration specifying period of time to wait for
      ///
      /// @return true if a report was returned; false if wait timed out.
      ///
      /// @throw  std::exception where the internal reader codes errors.
      /// @remark If the object is in an error state, this will throw the pending exception.
      template <typename Rep, typename Period>
      bool wait_for_getReport(Report & report, chrono::duration<Rep,Period> const & relTime)
      {
        return m_interfaceQueue->wait_for_pop(report, relTime) == cv_status::no_timeout;
      }
    };


  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_InterfaceQueue_hpp
