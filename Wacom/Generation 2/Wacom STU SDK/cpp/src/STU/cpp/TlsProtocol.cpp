#include <WacomGSS/STU/TlsProtocol.hpp>
#include <WacomGSS/STU/Interface.hpp>
#include <iostream>

namespace WacomGSS
{
  namespace STU
  {

    uint32_t TlsProtocol::decodeReport_size(Report const & report)
    {
      uint32_t size = static_cast<uint32_t>((report[0] << 24) | (report[1] << 16) | (report[2] << 8) | report[3]);
      return size;
    }
    uint16_t TlsProtocol::decodeReport_packetId(Report const & report)
    {
      uint16_t packetId = static_cast<uint16_t>((report[4] << 8) | report[5]);
      return packetId;
    }
    uint16_t TlsProtocol::decodeReport_returnValueStatus(Report const & report)
    {
      uint16_t returnValueStatus = static_cast<uint16_t>((report[6] << 8) | report[7]);
      return returnValueStatus;
    }

    void TlsProtocol::decodeReport(Report & report, TlsProtocol::ReturnValue & value)
    {
      value.size = 0;
      value.packetId = 0;
      value.returnValueStatus = 0;

      if (report.size() < 8)
        throw std::runtime_error("unexpected return size");

      value.size = decodeReport_size(report);

      if (value.size < 8)
        throw std::runtime_error("unexpected return size");

      value.packetId = decodeReport_packetId(report);
      value.returnValueStatus = decodeReport_returnValueStatus(report);
    }

    void TlsProtocol::decodeReport(Report & report, TlsProtocol::ReturnValue_ProtocolVersion & value)
    {
      decodeReport(report, static_cast<TlsProtocol::ReturnValue &>(value));

      if (value.size < 12 || report.size() < value.size  || (value.size & 1) != 0)
        throw std::runtime_error("unexpected error in packet");

      value.activeLevel = static_cast<uint16_t>(report[8] << 8 | report[9]);
      for (auto i = report.begin() + 10; i != report.end();)
      {
        uint16_t v = static_cast<uint16_t>(*i << 16);
        ++i;
        v |= *i;
        ++i;
        value.supportedLevels.push_back(v);
      }
    }



    void TlsProtocol::decodeReport(Report & report, TlsProtocol::ReturnValue_Pending & value)
    {
      decodeReport(report, static_cast<TlsProtocol::ReturnValue &>(value));

      if (value.size != 11 || report.size() != 11)
        throw std::runtime_error("unexpected error in packet");

      value.counter = static_cast<uint16_t>((report[8] << 8) | report[9]);
      value.statusFlags = report[10];
    }


    void TlsProtocol::decodeReport(Report & report, TlsProtocol::ReturnValue_GenerateCSR & value)
    {
      decodeReport(report, static_cast<TlsProtocol::ReturnValue &>(value));

      if (value.size < 9 || report.size() < 9)
        throw std::runtime_error("unexpected error in packet");

      value.csr.clear();
      value.csr.resize(report.size() - 8);
      std::copy(report.begin() + 8, report.end(), value.csr.begin());
    }

    

    void TlsProtocol::decodeReport(Report & report, TlsProtocol::ReturnValue_CurrentCertificate & value)
    {
      decodeReport(report, static_cast<TlsProtocol::ReturnValue &>(value));

      if (value.size < 10)
        throw std::runtime_error("unexpected size");

      value.certificateIndex = report[8];
      value.certificateDER.clear();
      value.certificateDER.resize(report.size() - 9);
      std::copy(report.begin() + 9, report.end(), value.certificateDER.begin());
    }


    

    

    TlsProtocol::ReturnValue_ProtocolVersion TlsProtocol::sendProtocolVersion(uint16_t desiredProtocolLevel)
    {
      std::array<uint8_t, packetHeaderSize+2> cmd;
      setPacketHeader(cmd, PacketId_ProtocolVersion);
      cmd[6] = static_cast<uint8_t>((desiredProtocolLevel >> 8) & 0xff);
      cmd[7] = static_cast<uint8_t>((desiredProtocolLevel) & 0xff);
      
      auto report( send(cmd) );

      TlsProtocol::ReturnValue_ProtocolVersion value;
      decodeReport(report, value);            
      return value;
    }
        
    

    InterfaceQueue TlsProtocol::sendGenerateCSR(uint16_t tlsCipherSuiteId, std::string countryName, std::string stateOrProvinceName, std::string localityName, std::string organisationName, std::string organisationalUnitName, std::string commonName, std::string emailAddress, std::string password)
    {
      size_t length = countryName.length() + stateOrProvinceName.length() + localityName.length() + organisationName.length() + organisationalUnitName.length() + commonName.length() + emailAddress.length() + password.length();

      std::vector<uint8_t> cmd(4 + 2 + 2 + 8 + length);
      setPacketHeader(cmd, PacketId_GenerateCSR);
      auto i = cmd.begin() + 6;
      *i++ = static_cast<uint8_t>((tlsCipherSuiteId >> 8) & 0xff);
      *i++ = static_cast<uint8_t>(tlsCipherSuiteId & 0xff);
      i = std::copy(countryName.begin(), countryName.end(), i);
      *i++ = 0;
      i = std::copy(stateOrProvinceName.begin(), stateOrProvinceName.end(), i);
      *i++ = 0;
      i = std::copy(localityName.begin(), localityName.end(), i);
      *i++ = 0;
      i = std::copy(organisationName.begin(), organisationName.end(), i);
      *i++ = 0;
      i = std::copy(organisationalUnitName.begin(), organisationalUnitName.end(), i);
      *i++ = 0;
      i = std::copy(commonName.begin(), commonName.end(), i);
      *i++ = 0;
      i = std::copy(emailAddress.begin(), emailAddress.end(), i);
      *i++ = 0;
      std::copy(password.begin(), password.end(), i);

      return m_interface->send(cmd.data(), cmd.size(), InterfaceTLS::InterfaceQueue_tag());
    }

    /*
    auto report{ send(cmd) };

    GenerateCSR_Return value;
    decodeReport(report, value);
    return value;
    */

    TlsProtocol::ReturnValue TlsProtocol::sendGenerateCSR_Cancel()
    {
      std::array<uint8_t, 6> cmd;
      setPacketHeader(cmd, PacketId_GenerateCSR_Cancel);      
      auto report(send(cmd));
      ReturnValue value;
      decodeReport(report, value);
      return value;
    }


    TlsProtocol::ReturnValue TlsProtocol::sendImportCertificate(ClientAuthentication clientAuthentication, uint8_t const * certificateDER, size_t length)
    {
      std::vector<uint8_t> cmd(6 + 1 + length);
      setPacketHeader(cmd, PacketId_ImportCertificate);
      cmd[6] = clientAuthentication;
      std::copy_n(certificateDER, length, cmd.begin() + 7);      
      auto report(send(cmd));
      ReturnValue value;
      decodeReport(report, value);
      return value;
    }


    TlsProtocol::ReturnValue TlsProtocol::sendDestroyCertificate()
    {
      std::array<uint8_t, 6> cmd;
      setPacketHeader(cmd, PacketId_DestroyCertificate);      
      auto report(send(cmd));
      ReturnValue value;
      decodeReport(report, value);
      return value;
    }


    InterfaceQueue TlsProtocol::sendUpdateCRL(uint8_t const * crlData, size_t length)
    {
      std::vector<uint8_t> cmd(6 + length);
      setPacketHeader(cmd, PacketId_UpdateCRL);      
      std::copy_n(crlData, length, cmd.begin() + 6);
      return m_interface->send(cmd.data(), cmd.size(), InterfaceTLS::InterfaceQueue_tag());
    }


    TlsProtocol::ReturnValue TlsProtocol::sendDeleteCRL()
    {
      std::array<uint8_t, 6> cmd;
      setPacketHeader(cmd, PacketId_DeleteCRL);
      
      auto report(send(cmd));
      ReturnValue value;
      decodeReport(report, value);
      return value;
    }

    TlsProtocol::ReturnValue_CurrentCertificate TlsProtocol::sendCurrentCertificate()
    {
      std::array<uint8_t, 6> cmd;
      setPacketHeader(cmd, PacketId_CurrentCertificate);
      
      auto report(send(cmd));
      ReturnValue_CurrentCertificate value;
      decodeReport(report, value);
      return value;
    }


    namespace ProtocolHelper
    {
      

      std::array<std::uint8_t, 7> makeLegacyGetReport(std::uint8_t legacyReportId)
      {
        std::array<std::uint8_t, 7> cmd;
        TlsProtocol::setPacketHeader(cmd, TlsProtocol::PacketId_Legacy_GetFeature);
        cmd[6] = legacyReportId;

        return cmd;
      }

      std::vector<std::uint8_t> makeLegacySetReport(std::uint8_t const * data, std::size_t length)
      {
        std::vector<std::uint8_t> cmd(length + 6);
        TlsProtocol::setPacketHeader(cmd, TlsProtocol::PacketId_Legacy_SetFeature);
        std::copy_n(data, length, cmd.begin() + 6);

        return cmd;
      }

      std::vector<std::uint8_t> makeLegacyWrite(std::uint8_t const * data, std::size_t length)
      {
        std::vector<std::uint8_t> cmd(length + 6);
        TlsProtocol::setPacketHeader(cmd, TlsProtocol::PacketId_Legacy_Write);
        std::copy_n(data, length, cmd.begin() + 6);

        return cmd;
      }

      std::pair<uint16_t, Report> legacyReportFromReport(Report && report)
      {
        if (report.size() < 6)
          throw Interface::io_error();

        uint32_t size = static_cast<uint32_t>((report[0] << 24) | (report[1] << 16) | (report[2] << 8) | report[3]);
        if (size != report.size())
          throw Interface::io_error();

        uint16_t packetId = static_cast<uint16_t>((report[4] << 8) | report[5]);

        report.erase(report.begin(), report.begin() + 6);
        return std::make_pair(packetId, std::move(report));
      }

      
      
      Report waitForReturn(InterfaceQueue & interfaceQueue, chrono::milliseconds timeout, SendHint sendHint)
      {
        auto absTime = chrono::steady_clock::now() + timeout;

        Report report;
        while (interfaceQueue.wait_until_getReport(report, absTime))
        {
          TlsProtocol::ReturnValue value;
          TlsProtocol::decodeReport(report, value);
          
          if (value.packetId == TlsProtocol::PacketId_ReturnValue)
          {
            if (value.returnValueStatus < TlsProtocol::ReturnValueStatus_Error)
            {              
              return report;
            }
            else
            {
              throw Interface::send_error(value.returnValueStatus, sendHint.packetId, sendHint.reportId);
            }
          }          
        }

        throw Interface::timeout_error();

      }

      Report waitForLegacyReturn(InterfaceQueue & interfaceQueue, chrono::milliseconds timeout)
      {
        auto report( waitForReturn(interfaceQueue, timeout) );

        report.erase(report.begin(), report.begin() + 8);
        return report;
      }


      namespace ostream_operators
      {
#define WacomGSS_enum_begin()         switch (value) {
#define WacomGSS_enum_case(TYPE,NAME)   case TYPE##_##NAME: return o << #NAME;
#define WacomGSS_enum_end(TYPE)       } return o << static_cast<TYPE>(value);


        std::ostream & operator << (std::ostream & o, TlsProtocol::ReturnValueStatus value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, Success)
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, Pending)
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, Pending_CSR)
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, CSR)
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, Pending_CRL)
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, Error_Busy)
          WacomGSS_enum_case(TlsProtocol::ReturnValueStatus, Error_Unspecified)
          WacomGSS_enum_end(std::uint16_t)
        }

        std::ostream & operator << (std::ostream & o, TlsProtocol::PacketId value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(TlsProtocol::PacketId, ProtocolVersion)
          WacomGSS_enum_case(TlsProtocol::PacketId, GenerateCSR)
          WacomGSS_enum_case(TlsProtocol::PacketId, GenerateCSR_Cancel)
          WacomGSS_enum_case(TlsProtocol::PacketId, ImportCertificate)
          WacomGSS_enum_case(TlsProtocol::PacketId, DestroyCertificate)
          WacomGSS_enum_case(TlsProtocol::PacketId, UpdateCRL)
          WacomGSS_enum_case(TlsProtocol::PacketId, DeleteCRL)
          WacomGSS_enum_case(TlsProtocol::PacketId, CurrentCertificate)
          WacomGSS_enum_case(TlsProtocol::PacketId, ReturnValue)
          WacomGSS_enum_case(TlsProtocol::PacketId, Legacy_GetFeature)
          WacomGSS_enum_case(TlsProtocol::PacketId, Legacy_SetFeature)
          WacomGSS_enum_case(TlsProtocol::PacketId, Legacy_PenData)
          WacomGSS_enum_case(TlsProtocol::PacketId, Legacy_EventData)
          WacomGSS_enum_case(TlsProtocol::PacketId, Legacy_Write)
          WacomGSS_enum_end(std::uint16_t)
        }

      } // ostream_operators


      


    } // namespace ProtocolHelper



  } // namespace STU

} // namespace WacomGSS
