/// @file      WacomGSS/STU/InterfaceImpl.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Internal implementation of the Interface class.

#ifndef WacomGSS_STU_InterfaceImpl_hpp
#define WacomGSS_STU_InterfaceImpl_hpp

#include <WacomGSS/concurrent_queue_with_exception.hpp>
#include <WacomGSS/noncopyable.hpp>
#include <WacomGSS/compatibility/atomic.hpp>
#include <memory>
#include <vector>
#include <deque>
#include <list>


namespace WacomGSS
{
  namespace STU
  {
    class Interface;
    class InterfaceQueue;

    /// @brief Standardised container of a Report
#if 1
    // simplified Report
    typedef std::vector<std::uint8_t> Report;
#else
    // Report with exception
    class Report
    {
      typedef std::vector<std::uint8_t> Container;
      Container          m_report;
      std::exception_ptr m_exception;

    public:
      Report()
      {
      }
      
      Report(std::exception_ptr && exception)
      :
        m_exception(exception)
      {
      }
      
      Report(std::vector<std::uint8_t> const & report)
        :
        m_report(report)
      {
      }
      
      Report(std::vector<std::uint8_t> && report)
      :
        m_report(report)        
      {
      }

      bool has_exception() const noexcept { return m_exception != nullptr; }
      void reset_exception() { m_exception = nullptr; }
      void rethrow_exception() const 
      {
        if (has_exception())
        {
          std::rethrow_exception(m_exception);
        }
      }
            
      bool empty() const noexcept { return m_report.empty(); }
      Container::size_type size() const noexcept { return m_report.size(); }
      Container::reference operator[] (Container::size_type pos) { return m_report[pos]; }
      Container::const_reference operator[] (Container::size_type pos) const { return m_report[pos]; }
      Container::pointer data() { return m_report.data(); }
      Container::const_pointer data() const { return m_report.data(); }      
      Container::iterator begin() { return m_report.begin(); }
      Container::const_iterator begin() const { return m_report.begin(); }
      Container::iterator end() { return m_report.end(); }      
      Container::const_iterator end() const { return m_report.end(); }
      Container::reference front() { return m_report.front(); }   
      Container::const_reference front() const { return m_report.front(); }
      void pop_back() { m_report.pop_back(); }
      void push_back(Container::value_type const & value) { m_report.push_back(value); }
      
      operator Container & () noexcept { return m_report; }
      operator Container const & () const noexcept { return m_report; }      
    };
#endif

    /// @internal
    class InterfaceImpl : WacomGSS::noncopyable
    {
      friend class Interface;
      friend class InterfaceQueue;
            
      typedef std::shared_ptr<concurrent_queue_with_exception<std::deque<Report>,Report>> shared_queue;

      atomic<bool>            m_empty; // cached value of m_queues.empty()
      mutex                   m_mutex;
      std::list<shared_queue> m_queues;

      shared_queue createInterfaceQueue();
      void         removeInterfaceQueue(shared_queue && queue);

    public:
      InterfaceImpl();
    };


  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_InterfaceImpl_hpp
