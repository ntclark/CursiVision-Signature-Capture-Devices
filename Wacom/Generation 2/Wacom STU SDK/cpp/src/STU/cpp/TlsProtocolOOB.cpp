#include <WacomGSS/STU/TlsProtocol.hpp>
#include <WacomGSS/STU/Interface.hpp>
#include <WacomGSS/compatibility/thread.hpp>
#include <iostream>

namespace WacomGSS
{
  namespace STU
  {

    TlsProtocolOOB::Descriptor TlsProtocolOOB::getDescriptor()
    {
      std::array<uint8_t, 26> cmd = { ReportId_Descriptor };

      getOOB(cmd);

      Descriptor data;

      data.descriptorFlags = static_cast<uint32_t>((cmd[1] << 24) | (cmd[2] << 16) | (cmd[3] << 8) | cmd[4]);
      data.idVendor = static_cast<uint16_t>((cmd[5] << 8) | cmd[6]);
      data.idProduct = static_cast<uint16_t>((cmd[7] << 8) | cmd[8]);
      data.firmwareRevisionMajor = static_cast<uint16_t>((cmd[9] << 8) | cmd[10]);
      data.firmwareRevisionMinor = static_cast<uint16_t>((cmd[11] << 8) | cmd[12]);
      //cmd[13]
      //cmd[14]
      data.modelName[0] = static_cast<char>(cmd[15]);
      data.modelName[1] = static_cast<char>(cmd[16]);
      data.modelName[2] = static_cast<char>(cmd[17]);
      data.modelName[3] = static_cast<char>(cmd[18]);
      data.modelName[4] = static_cast<char>(cmd[19]);
      data.modelName[5] = static_cast<char>(cmd[20]);
      data.modelName[6] = static_cast<char>(cmd[21]);
      data.modelNameNullTerminated[7] = 0;
      // cmd[22]
      // cmd[23]
      // cmd[24]
      // cmd[25]

      return data;
    }
    


    TlsProtocolOOB::Status TlsProtocolOOB::getStatus()
    {
      std::array<uint8_t, 7> cmd = { ReportId_Status };

      getOOB(cmd);

      Status data;

      data.oobStatus = static_cast<uint16_t>(cmd[1] << 8 | cmd[2]);
      data.oobExtendedStatus = static_cast<uint32_t>((cmd[3] << 24) | (cmd[4] << 16) | (cmd[5] << 8) | cmd[6]);

      return data;
    }



    uint32_t TlsProtocolOOB::getFactoryResetKey()
    {
      std::array<uint8_t, 5> cmd = { ReportId_FactoryResetKey };

      getOOB(cmd);

      uint32_t data = static_cast<uint32_t>(cmd[1] << 24 | cmd[2] << 16 | cmd[3] << 8 | cmd[4]);

      return data;
    }


    void TlsProtocolOOB::setFactoryReset(uint32_t factoryResetKey)
    {
      std::array<uint8_t, 5> cmd = 
      { 
        ReportId_FactoryResetKey,
        static_cast<uint8_t>((factoryResetKey >> 24) & 0xff),
        static_cast<uint8_t>((factoryResetKey >> 16) & 0xff),
        static_cast<uint8_t>((factoryResetKey >>  8) & 0xff),
        static_cast<uint8_t>((factoryResetKey      ) & 0xff)
      };

      setOOB(cmd);
    }



    void TlsProtocolOOB::setFirmwareUpdate(uint32_t factoryResetKey)
    {
      std::array<uint8_t, 5> cmd =
      {
        ReportId_FirmwareUpdate,
        static_cast<uint8_t>((factoryResetKey >> 24) & 0xff),
        static_cast<uint8_t>((factoryResetKey >> 16) & 0xff),
        static_cast<uint8_t>((factoryResetKey >>  8) & 0xff),
        static_cast<uint8_t>((factoryResetKey      ) & 0xff)
      };

      setOOB(cmd);
    }



    void TlsProtocolOOB::setReset(ResetFlag resetFlag)
    {
      std::array<uint8_t,2> cmd = { ReportId_Reset, resetFlag };

      setOOB(cmd);
    }
    


    Protocol::ReportSizeCollection TlsProtocolOOB::getReportSizeCollection()
    {
      std::array<uint8_t, 512> cmd = { ReportId_ReportSizeCollection };

      getOOB(cmd);

      Protocol::ReportSizeCollection data;

      data[0] = 0;
      for (unsigned int i = 1; i <= 255; ++i)
      {
        data[i] = static_cast<uint16_t>((cmd[0u + i * 2] << 8) | cmd[1u + i * 2]);
      }

      return std::move(data);
    }


    namespace ProtocolHelper
    {

      void waitForStatus(TlsProtocolOOB & protocol, std::uint16_t oobStatus, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        for (;;)
        {
          auto status = protocol.getStatus();
          if (status.oobStatus == oobStatus)
            break;

          if (!retries)
            throw Interface::timeout_error();

          --retries;

          this_thread::sleep_for(sleepBetweenRetries);
        }
      }



      namespace ostream_operators
      {
#define WacomGSS_enum_begin()         switch (value) {
#define WacomGSS_enum_case(TYPE,NAME)   case TYPE##_##NAME: return o << #NAME;
#define WacomGSS_enum_end(TYPE)       } return o << static_cast<TYPE>(value);


        std::ostream & operator << (std::ostream & o, TlsProtocolOOB::ReportId value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,Descriptor)
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,Status)
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,Reset)
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,FactoryResetKey)
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,FactoryReset)
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,FirmwareUpdate)
          WacomGSS_enum_case(TlsProtocolOOB::ReportId,ReportSizeCollection)
          WacomGSS_enum_end(std::uint16_t)
        }
               


        std::ostream & operator << (std::ostream & o, TlsProtocolOOB::OobStatus value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,Operational)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,Error_Hardware)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,Booting)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,Connected)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,Established)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,SessionBroken)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,TLS_InvalidFormat)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,TLS_NoCACertforCRL)
          WacomGSS_enum_case(TlsProtocolOOB::OobStatus,TLS_SizeOverflow)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, TlsProtocolOOB::ResetFlag value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(TlsProtocolOOB::ResetFlag,Connection)
          WacomGSS_enum_case(TlsProtocolOOB::ResetFlag,Software)
          WacomGSS_enum_case(TlsProtocolOOB::ResetFlag,Hardware)
          WacomGSS_enum_end(std::uint16_t)
        }


      } // ostream_operators

    } // namespace ProtocolHelper

  } // namespace STU

} // namespace WacomGSS
