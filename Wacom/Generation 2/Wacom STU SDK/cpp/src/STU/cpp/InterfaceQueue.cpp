#include <WacomGSS/STU/InterfaceQueue.hpp>

namespace WacomGSS
{
  namespace STU
  {

    InterfaceQueue::InterfaceQueue()
    {
    }

    InterfaceQueue::InterfaceQueue(std::shared_ptr<InterfaceImpl> & interfaceData)
    :
      m_interfaceData(interfaceData),
      m_interfaceQueue(interfaceData->createInterfaceQueue())
    {
    }



    InterfaceQueue::InterfaceQueue(InterfaceQueue && other) noexcept
    :
      m_interfaceData(std::move(other.m_interfaceData)),
      m_interfaceQueue(std::move(other.m_interfaceQueue))
    {
    }


    InterfaceQueue::~InterfaceQueue()
    {
      if (auto interfaceData = m_interfaceData.lock())
      {
        interfaceData->removeInterfaceQueue(std::move(m_interfaceQueue));
      }
      m_interfaceData.reset();
    }



    InterfaceQueue & InterfaceQueue::operator = (InterfaceQueue && other)
    {
      if (auto interfaceData = m_interfaceData.lock())
      {
        interfaceData->removeInterfaceQueue(std::move(m_interfaceQueue));
      }

      m_interfaceData  = std::move(other.m_interfaceData);
      m_interfaceQueue = std::move(other.m_interfaceQueue);

      return *this;
    }

    void reset();


  } // namespace STU

} // namespace WacomGSS
