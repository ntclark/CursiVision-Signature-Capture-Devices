#include <WacomGSS/STU/Interface.hpp>


namespace WacomGSS
{
  namespace STU
  {



    void Interface::queueReport(Report && report)
    {
      if (!m_interfaceData->m_empty.load())
      {
        lock_guard<mutex> lock(m_interfaceData->m_mutex);
        if (m_queueReport_debug)
        {
          m_queueReport_debug(report, m_interfaceData->m_queues.size());
        }

        for (auto i = m_interfaceData->m_queues.begin(); i != m_interfaceData->m_queues.end(); ++i)
        {
          i->get()->push_back(report);
        }
      }
      else
      {
        if (m_queueReport_debug)
        {
          m_queueReport_debug(report, 0);
        }
      }
    }



    bool Interface::queueException(std::exception_ptr && ex) noexcept
    {
      try
      {
        if (!m_interfaceData->m_empty.load())
        {
          lock_guard<mutex> lock(m_interfaceData->m_mutex);
          for (auto i = m_interfaceData->m_queues.begin(); i != m_interfaceData->m_queues.end(); ++i)
          {
            i->get()->push_back(ex);
          }
        }
      }
      catch (...)
      {
        return false;
      }
      return true;
    }



    void Interface::queueClear()
    {
      if (!m_interfaceData->m_empty.load())
      {
        lock_guard<mutex> lock(m_interfaceData->m_mutex);
        for (auto i = m_interfaceData->m_queues.begin(); i != m_interfaceData->m_queues.end(); ++i)
        {
          i->get()->clear();
        }
      }
    }


    void Interface::queueNotifyAll()
    {
      if (!m_interfaceData->m_empty.load())
      {
        lock_guard<mutex> lock(m_interfaceData->m_mutex);
        for (auto i = m_interfaceData->m_queues.begin(); i != m_interfaceData->m_queues.end(); ++i)
        {
          i->get()->notify_all();
        }
      }
    }



    void Interface::queueSetPredicateAll(bool predicate)
    {
      if (!m_interfaceData->m_empty.load())
      {
        lock_guard<mutex> lock(m_interfaceData->m_mutex);
        for (auto i = m_interfaceData->m_queues.begin(); i != m_interfaceData->m_queues.end(); ++i)
        {
          i->get()->set_predicate(predicate);
        }
      }
    }



    //-------------------------------------------------------------------------



    Interface::Interface()
    :
      m_interfaceData(std::make_shared<InterfaceImpl>())
    {
    }



    Interface::~Interface()
    {
    }

    

    InterfaceQueue Interface::interfaceQueue()
    {
      return InterfaceQueue(m_interfaceData);
    }



    bool Interface::getReportCountLengths(std::array<std::uint16_t, 256> &) const
    {
      return false;
    }


    WacomGSS_STU_runtime_error_impl( Interface::not_connected_error       , "not_connected_error"       )
    WacomGSS_STU_runtime_error_impl( Interface::device_removed_error      , "device_removed_error"      )
    WacomGSS_STU_runtime_error_impl( Interface::write_not_supported_error , "write_not_supported_error" )
    WacomGSS_STU_runtime_error_impl( Interface::io_error                  , "io_error"                  )
    WacomGSS_STU_runtime_error_impl( Interface::timeout_error             , "timeout_error"             )
    WacomGSS_STU_runtime_error_impl( Interface::set_error                 , "set_error"                 )    
    WacomGSS_STU_runtime_error_impl( Interface::send_error, "send_error")

  } // namespace STU

} // namespace WacomGSS
