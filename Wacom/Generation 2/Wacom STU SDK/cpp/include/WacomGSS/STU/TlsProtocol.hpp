/// @file      WacomGSS/STU/TlsProtocol.hpp
/// @copyright Copyright (c) 2016 Wacom Company Limited
/// @author    mholden
/// @date      2016-11-22
/// @brief     Definition of the WacomGSS::STU::TlsProtocol class

#ifndef WacomGSS_STU_TlsProtocol_hpp
#define WacomGSS_STU_TlsProtocol_hpp

#include <WacomGSS/STU/Protocol.hpp>

namespace WacomGSS
{
  namespace STU
  {


    /// @brief Class to encapsulate the TLS OOB protocol.
    class TlsProtocolOOB
    {
      InterfaceTLS * m_interface; // will never be null

      template<size_t N>
      void getOOB(std::array<uint8_t, N> & data)
      {
        m_interface->getOOB(data);
      }


      template<size_t N>
      void setOOB(std::array<uint8_t, N> const & data)
      {
        m_interface->setOOB(data);
      }

    public:
      /// @brief Constructs the object.
      ///
      /// @param intf A reference to the Interface to use.
      TlsProtocolOOB(InterfaceTLS & intf) noexcept
        :
        m_interface(&intf)
      {
      }


      /// @brief Rebinds the object to a different Interface.
      ///
      /// @param intf A reference to the Interface to use.
      TlsProtocolOOB & rebind(InterfaceTLS & intf) noexcept
      {
        m_interface = &intf;
        return *this;
      }


      /// @brief Provides access to the stored Interface reference.
      InterfaceTLS * operator -> () const noexcept
      {
        return m_interface;
      }

      /// @name USB level values
      /// @{
      static const std::uint8_t  ControlTransfer_RequestType_Set = 0x41;
      static const std::uint8_t  ControlTransfer_RequestType_Get = 0xC1;

      static const std::uint8_t  ControlTransfer_Request = 0xF0;
      static const std::uint16_t ControlTransfer_Index = 0x0005;
      /// @}

      /// @brief Report Identifier values.
      enum ReportId : uint8_t
      {
        ReportId_Descriptor           = 0x01,
        ReportId_Status               = 0x02,
        ReportId_Reset                = 0x03,
        ReportId_FactoryResetKey      = 0x0e,
        ReportId_FactoryReset         = 0x0f,
        ReportId_FirmwareUpdate       = 0x80,
        ReportId_ReportSizeCollection = 0xFF
      };


      /// @brief Brief description of the TLS tablet without having to establish a full SSL connection.
      struct Descriptor
      {
        uint32_t descriptorFlags;       ///< reserved, zero.
        uint16_t idVendor;              ///< Vendor ID (assigned by the USB-IF) [Wacom = 0x056a]
        uint16_t idProduct;             ///< Product ID (assigned by the manfacturer)
        uint16_t firmwareRevisionMajor; ///< tablet firmware version
        uint16_t firmwareRevisionMinor; ///< tablet firmware revision
        union
        {
          char modelName[7];                   ///< friendly name
          char modelNameNullTerminated[7 + 1];
        };
      };
      

      /// @brief Retrieves the basic HID-type information for the device.
      Descriptor getDescriptor();
      
      /// @name Status
      /// @{

      /// @see Status::oobStatus
      enum OobStatus : uint16_t
      {
        OobStatus_Operational = 0x0000, ///< The tablet is ready for use
        
        OobStatus_Booting= 0x0001, ///< The tablet is in the process of booting or restarting
        OobStatus_Connected= 0x0002, ///< An application is communicating with the tablet
        OobStatus_Established= 0x0003, ///< An encrypted session is in progress
        OobStatus_SessionBroken= 0x8000, ///< There has been an unrecoverable error in the encrypted session

        OobStatus_TLS_InvalidFormat = 0xF001, ///< Format is not matched
        OobStatus_TLS_NoCACertforCRL = 0xF002, ///< Not installed CA certificate for CRL
        OobStatus_TLS_SizeOverflow = 0xF003, ///< Size overflow

        OobStatus_Error_Hardware = 0xFFFF ///< Tablet has detected a fault
      };

      /// @brief Describes the state of the tablet
      /// @see getStatus()
      struct Status
      {
        uint16_t oobStatus; ///< Describes the currrent state of the tablet
        uint32_t oobExtendedStatus; ///< reserved, 0
      };

      /// @brief Retrieves the generic state of the tablet
      Status getStatus();

      /// @}


      /// @name Reset
      /// @{

      /// @see setReset()
      enum ResetFlag : uint8_t
      {
        ResetFlag_Connection = 0x00, ///< Drops and resets the TLS connection.
        ResetFlag_Software = 0x02, ///< Perform a soft reset.
        ResetFlag_Hardware = 0x04  ///< Perform a hard reset.
      };

      /// @brief Provide control of the tablet outside of the TLS connection.
      void setReset(ResetFlag flag);

      /// @}


      /// @name FactoryResetKey
      /// @{

      /// @brief Retrieves the factory reset key.
      /// The return of this value is passed to setFactoryReset(). This measure is a guard against an accidental call to setFactoryReset().
      /// @note This value is volatile and changes on each call and each reset.
      uint32_t getFactoryResetKey();

      /// @brief Reinitializes the tablet back to factory defaults.
      /// This call removes any user-level private key, certificate and CRL.
      /// @note You must call getFactoryResetKey() first in order to provide the correct key to validate this call.
      void setFactoryReset(uint32_t factoryResetKey);

      /// @brief Enables the control to allow firmware update to begin.
      void setFirmwareUpdate(uint32_t factoryResetKey);

      /// @}



      typedef Protocol::ReportSizeCollection ReportSizeCollection;

      /// @brief Retrieves the size of each supported report.
      /// @see ReportSizeCollection
      ReportSizeCollection getReportSizeCollection();
    };



    /// @brief Abstracts the documented %STU tablet protocol.
    ///
    /// This class provides an abstraction to send and receive
    /// data packets to and from the tablet using the Interface class.
    class TlsProtocol
    {
      InterfaceTLS * m_interface; // will never be null

      template<size_t N>
      Report send(std::array<uint8_t, N> const & data)
      {
        return m_interface->send(data);
      }

      Report send(std::vector<uint8_t> const & data)
      {
        return m_interface->send(data.data(), data.size());
      }

    public:
      /// @brief Constructs the object.
      ///
      /// @param intf A reference to the Interface to use.
      TlsProtocol(InterfaceTLS & intf) noexcept
        :
        m_interface(&intf)
      {
      }


      /// @brief Rebinds the object to a different Interface.
      ///
      /// @param intf A reference to the Interface to use.
      TlsProtocol & rebind(InterfaceTLS & intf) noexcept
      {
        m_interface = &intf;
        return *this;
      }


      /// @brief Provides access to the stored Interface reference.
      InterfaceTLS * operator -> () const noexcept
      {
        return m_interface;
      }

      /// @brief Packet type identifier values.
      enum PacketId : std::uint16_t
      {
        PacketId_ProtocolVersion = 0x0001,
        PacketId_GenerateCSR = 0x0010,
        PacketId_GenerateCSR_Cancel = 0x0011,
        PacketId_ImportCertificate = 0x0012,
        PacketId_DestroyCertificate = 0x0013,
        PacketId_UpdateCRL = 0x0014,
        PacketId_DeleteCRL = 0x0015,
        PacketId_CurrentCertificate = 0x0016,
        
        PacketId_ReturnValue = 0x0080,

        PacketId_Legacy_GetFeature = 0x0102,
        PacketId_Legacy_SetFeature = 0x0103,
        PacketId_Legacy_PenData = 0x0104,
        PacketId_Legacy_EventData = 0x0105,
        PacketId_Legacy_Write    = 0x0106
      };

      static const uint16_t ReturnValueStatus_Error = 0x8000; ///< Errors always have the high bit set.

      // @see ReturnValue::returnValueStatus
      enum ReturnValueStatus : std::uint16_t
      {
        ReturnValueStatus_Success = 0x0000,
        ReturnValueStatus_Pending = 0x4001,
        ReturnValueStatus_Pending_CSR = 0x4002,
        ReturnValueStatus_CSR = 0x4003,
        ReturnValueStatus_Pending_CRL = 0x4004,
        ReturnValueStatus_Error_Busy = 0x8001,
        ReturnValueStatus_Error_Unspecified = 0xffff
      };

      /// @brief Holds the values of a decoded return packet
      struct ReturnValue
      {
        uint32_t size; ///< size of the entire data packet
        uint16_t packetId; ///< @see enum PacketId
        uint16_t returnValueStatus; ///< @see enum ReturnValueStatus
      };

      static const uint8_t packetHeaderSize = 6; ///< minimum size of a packet (4 bytes size + 2 bytes packet id)

      /// @brief sets the first 6 bytes of the packet
      template<typename T>
      static void setPacketHeader(T & cmd, uint32_t size, uint16_t packetId)
      {        
        //static_assert(size >= 6, "invalid Header size");

        cmd[0] = static_cast<uint8_t>((size >> 24) & 0xff);
        cmd[1] = static_cast<uint8_t>((size >> 16) & 0xff);
        cmd[2] = static_cast<uint8_t>((size >> 8) & 0xff);
        cmd[3] = static_cast<uint8_t>((size) & 0xff);
        cmd[4] = static_cast<uint8_t>((packetId >> 8) & 0xff);
        cmd[5] = static_cast<uint8_t>((packetId) & 0xff);
      }


      /// @brief sets the first 6 bytes of the packet, assuming that cmd meets the concept of Container
      template<typename T>
      static void setPacketHeader(T & cmd, uint16_t packetId)
      {        
        setPacketHeader<T>(cmd, cmd.size(), packetId);
      }

      /// @brief Retrieves the size of the data packet.
      /// Typically it is easier to use decodeReport().
      static uint32_t decodeReport_size(Report const & report);

      /// @brief Retrieves the packetId of the data packet.
      /// Typically it is easier to use decodeReport().
      static uint16_t decodeReport_packetId(Report const & report);

      /// @brief Retrieves the return status of the data packet.
      /// Typically it is easier to use decodeReport().
      /// @note This call is only valid if the report is a return packet.
      static uint16_t decodeReport_returnValueStatus(Report const & report);
      
      /// @brief Retrieves the header values from a received report.
      static void decodeReport(Report & report, ReturnValue & data);


      /// @name ProtocolLevel
      /// @{

      struct ReturnValue_ProtocolVersion : ReturnValue
      {        
        uint16_t activeLevel;
        std::vector<uint16_t> supportedLevels;
      };

      static void decodeReport(Report & report, ReturnValue_ProtocolVersion & data);

      /// @brief Retrieves the current or sets a new protocol level.
      /// Set desiredProtocolLevel to 0 to retrieve the current level along with the
      /// list of available levels.
      ///
      /// Typically, this will be the first call to use once a full connect is established.
      ReturnValue_ProtocolVersion sendProtocolVersion(uint16_t desiredProtocolLevel);

      /// @}


      /// @name GenerateCSR
      /// @{

      ///@ brief Return type during calculation of a new key pair.
      struct ReturnValue_Pending : ReturnValue
      {
        uint16_t counter; ///< Incrementing counter to confirm the tablet is still working.
        uint8_t statusFlags;
      };
      static void decodeReport(Report & report, ReturnValue_Pending & data);

      ///@ brief The completed CSR to be sent to the CA.
      struct ReturnValue_GenerateCSR : ReturnValue
      {
        std::vector<uint8_t> csr;
      };
      static void decodeReport(Report & report, ReturnValue_GenerateCSR & data);

      /// @brief sends the request to generate a new key pair and return a CSR for signing.
      /// Strings must not have embedded nulls and should be compliant to X509 standards.
      /// @note this returns an InterfaceQueue as there will be multiple ReturnValue_Pending reports returned while the calculation takes place.
      InterfaceQueue sendGenerateCSR(uint16_t tlsCipherSuiteId, std::string countryName, std::string stateOrProvinceName, std::string localityName, std::string organisationName, std::string organisationalUnitName, std::string commonName, std::string emailAddress, std::string password);

      /// @}

      /// @brief Cancels the request of generating a new key pair.
      /// @note It is recommended to call setReset(ResetFlag_Hardware) after this call has been made.
      ReturnValue sendGenerateCSR_Cancel();

      /// @name ImportCertificate
      /// @{

      /// @see sendImportCertificate
      enum ClientAuthentication : uint8_t
      {
        ClientAuthentication_Single = 0x00,
        ClientAuthentication_Mutual = 0x01
      };

      /// @brief uploads the signed certificate that has been return from your CA from the CSR that was previously requested.
      ///
      /// @note Although mutual client authentication is provided as an option, it is not recommended. A thorough review of the full
      /// end-to-end security of the entire system is recommended before deciding upon the additional overhead of using this feature.
      ReturnValue sendImportCertificate(ClientAuthentication clientAuthentication, uint8_t const * certificateDER, size_t length);

      /// @}

      /// @brief Deletes the stored user key-pair and reverts to using the inbuilt factory key-pair.
      ReturnValue sendDestroyCertificate();

      /// @brief Updates the CRL which is required if mutual authentication is requested.
      /// Returns a number of ReturnValue_Pending while it is processing, then a standard ReturnValue.
      ///
      /// @note Tablet memory is limited and it is possible for the crl to be too large to process. For long term safety to avoid this situation,
      /// it is preferred to avoid mutual authentication.
      InterfaceQueue sendUpdateCRL(uint8_t const * crlData, size_t length);

      /// @brief removes the current CRL from tablet memory.
      ReturnValue sendDeleteCRL();

      /// @name CurrentCertificate
      /// @{

      /// @see sendCurrentCertificate
      struct ReturnValue_CurrentCertificate : ReturnValue
      {
        /// Index 0 = pre-factory
        /// Index 1 = factory
        /// Index 2 = user
        uint8_t certificateIndex; 
        std::vector<uint8_t> certificateDER;
      };

      /// @see ReturnValue_CurrentCertificate
      static void decodeReport(Report & report, ReturnValue_CurrentCertificate & data);

      /// @brief retreives the certificate that will be used for TLS connections.
      /// The index refers to whether it is the factory installed or a user-created certificate.
      ReturnValue_CurrentCertificate sendCurrentCertificate();

      /// @}

    };

    namespace ProtocolHelper
    {
      std::array<std::uint8_t, 7> makeLegacyGetReport(uint8_t reportId);
      std::vector<std::uint8_t>   makeLegacySetReport(std::uint8_t const * data, std::size_t length);
      std::vector<std::uint8_t>   makeLegacyWrite(std::uint8_t const * data, std::size_t length);
    

      std::pair<uint16_t, Report> legacyReportFromReport(Report && report);
      
      struct SendHint
      {
        TlsProtocol::PacketId packetId;
        Protocol::ReportId reportId;
      };

      Report waitForReturn(InterfaceQueue & interfaceQueue, chrono::milliseconds timeout, SendHint = { static_cast<TlsProtocol::PacketId>(0),static_cast<Protocol::ReportId>(0) });

      /// @throws return_error
      //Report waitForLegacyReturn(InterfaceQueue & interfaceQueue, chrono::milliseconds timeout);

      
    
      namespace ostream_operators
      {
        std::ostream & operator << (std::ostream &, TlsProtocolOOB::ReportId);
        std::ostream & operator << (std::ostream &, TlsProtocolOOB::OobStatus);
        std::ostream & operator << (std::ostream &, TlsProtocolOOB::ResetFlag);
        std::ostream & operator << (std::ostream &, TlsProtocol::ReturnValueStatus);
        std::ostream & operator << (std::ostream &, TlsProtocol::PacketId);
      }
          
    }


    namespace ProtocolHelper
    {
      /// @brief Suspends the thread until the device is in the specified state, or until the function times out.
      ///
      /// @param protocol             Protcol object representing the device to wait for.
      /// @param oobStatus            Status to wait for.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      ///
      /// @throws Interface::timeout_error
      void waitForStatus(TlsProtocolOOB & protocol, std::uint16_t oobStatus, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));
    }


  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_TlsProtocol_hpp
