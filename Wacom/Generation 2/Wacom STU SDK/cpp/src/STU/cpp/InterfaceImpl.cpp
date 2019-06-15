#include <WacomGSS/STU/InterfaceImpl.hpp>

namespace WacomGSS
{
  namespace STU
  {


    InterfaceImpl::InterfaceImpl()
    :
      m_empty(true)
    {
    }


    InterfaceImpl::shared_queue InterfaceImpl::createInterfaceQueue()
    {
      shared_queue queue = std::make_shared<shared_queue::element_type>();      
      {
        lock_guard<mutex> lock(m_mutex);
        m_queues.push_back(queue);
        m_empty.store(false);
      }
      return queue;
    }



    void InterfaceImpl::removeInterfaceQueue(shared_queue && queue)
    {
      lock_guard<mutex> lock(m_mutex);
      for (auto i = m_queues.begin(); i != m_queues.end(); ++i)
      {
        if (*i == queue)
        {
          m_queues.erase(i);
          m_empty.store(m_queues.empty());
          break;
        }
      }
    }
    

  } // namespace STU

} // namespace WacomGSS
