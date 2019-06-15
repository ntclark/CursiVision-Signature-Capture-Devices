#include <WacomGSS/STU/TlsInterface_Debug.hpp>
#include <WacomGSS/STU/TlsProtocol.hpp>
#include <WacomGSS/STU/ProtocolHelper.hpp>


#include <sstream>
#include <iomanip>



namespace WacomGSS
{
  namespace STU
  {


    TlsInterface_Debug::TlsInterface_Debug()
    :
      m_data(new Data)
    {
    }



    TlsInterface_Debug::~TlsInterface_Debug()
    {
    }



    void TlsInterface_Debug::operator() (uint8_t const * data, size_t length)
    {
      m_data->m_lastReportId = 0;

      using namespace std;
      using namespace ProtocolHelper::ostream_operators;


      stringstream o;
      o << "send     bytes=" << setw(4) << length;

      if (data && length > 5)
      {
        auto packetId(static_cast<TlsProtocol::PacketId>((data[4] << 8) | data[5]));
 
        o << " packetId=" << packetId;

        if (packetId == TlsProtocol::PacketId_Legacy_GetFeature || packetId == TlsProtocol::PacketId_Legacy_SetFeature)
        {
          m_data->m_lastReportId = data[6];

          o << " " << static_cast<Protocol::ReportId>(data[6]);
        }
      }
      o << "\r\n";

      output(o.str().c_str());
    }



    void TlsInterface_Debug::operator() (Report const & report, size_t numberofQueues)
    {
      using namespace std;
      using namespace ProtocolHelper::ostream_operators;

      std::stringstream o;
      o << "recv q=" << numberofQueues << " bytes=" << setw(4) << report.size();

      if (report.size() >= 6)
      {
        auto size(TlsProtocol::decodeReport_size(report));

        if (size != report.size())
        {
          o << " size=" << size;
        }

        auto packetId(static_cast<TlsProtocol::PacketId>(TlsProtocol::decodeReport_packetId(report)));
        o << " packetId=" << setw(17) << left << packetId << right;

        if (packetId == TlsProtocol::PacketId_ReturnValue)
        {
          o << " returnValue=" << static_cast<TlsProtocol::ReturnValueStatus>(TlsProtocol::decodeReport_returnValueStatus(report));

          if (m_data->m_lastReportId == Protocol::ReportId_Status)
          {
            o << " statusCode=" << static_cast<Protocol::StatusCode>(report[9]) << " lastResultCode=" << static_cast<Protocol::ErrorCode>(report[10]) << " statusWord=" << static_cast<unsigned>(report[11] | (report[12] << 8));
          }
        }
      }
      o << "\r\n";

      output(o.str().c_str());
    }


  } // namespace STU
  
} // namespace WacomGSS

