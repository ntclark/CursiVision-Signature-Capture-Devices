#include <WacomGSS/STU/ProtocolHelper.hpp>
#include <WacomGSS/STU/Interface.hpp> // to throw timeout_error
#include <WacomGSS/compatibility/thread.hpp>
#include <WacomGSS/STU/getUsbDevices.hpp>

#include <algorithm>
#include <iostream>

namespace WacomGSS
{
  namespace STU
  {

    namespace ProtocolHelper
    {




      InkState getNextInkState(InkState inkState, uint8_t rdy, uint16_t pressure, Protocol::InkThreshold const & inkThreshold) noexcept
      {
        if ((rdy == 0) ||
        (((inkState & InkState_isOff) == 0) && (pressure < inkThreshold.offPressureMark)) ||
        (((inkState & InkState_isOff) != 0) && (pressure < inkThreshold.onPressureMark))) {

          if ((inkState & InkState_isInk) != 0)
          {
            return InkState_Last;
          }
          else
          {
            return InkState_Up;
          }
        }
        else
        {
          if (inkState != InkState_Up)
          {
            if (inkState == InkState_Down)
            {
              return InkState_First;
            }
            else
            {
              return InkState_Inking;
            }
          }
          else
          {
            return InkState_Down;
          }
        }
      }



      static const Protocol::InkThreshold k_factoryDefaultInkThreshold =
      {
        21, 17
      };



      InkingState::InkingState() noexcept
      :
        m_inkThreshold(k_factoryDefaultInkThreshold),
        m_currentState(InkState_Up)
      {
      }



      void InkingState::setInkThreshold(Protocol::InkThreshold const & inkThreshold) noexcept
      {
        m_inkThreshold = inkThreshold;
      }



        Protocol::InkThreshold InkingState::getInkThreshold() const noexcept
      {
        return m_inkThreshold;
      }



      void InkingState::setState(InkState state) noexcept
      {
        m_currentState = state;
      }



      void InkingState::clear() noexcept
      {
        m_currentState = InkState_Up;
      }



      InkState InkingState::currentState() const noexcept
      {
        return m_currentState;
      }



      InkState InkingState::nextState(uint8_t rdy, uint16_t pressure) noexcept
      {
        m_currentState = getNextInkState(m_currentState, rdy, pressure, m_inkThreshold);
        return m_currentState;
      }



      enum OpStatus : uint8_t
      {
        OpStatus_Ready = 0x01,
        OpStatus_Image = 0x02,
        OpStatus_Capture = 0x04,
        OpStatus_Calculation = 0x08,
        OpStatus_Image_Boot = 0x10,
        OpStatus_RomBusy = 0x20,
        OpStatus_SystemReset = 0x40
      };



      OpStatus OpStatus_from_StatusCode(Protocol::StatusCode s)
      {
        switch (s)
        {
        case Protocol::StatusCode_Ready: return OpStatus_Ready;
        case Protocol::StatusCode_Image: return OpStatus_Image;
        case Protocol::StatusCode_Capture: return OpStatus_Capture;
        case Protocol::StatusCode_Calculation: return OpStatus_Calculation;
        case Protocol::StatusCode_Image_Boot: return OpStatus_Image_Boot;
        case Protocol::StatusCode_RomBusy: return OpStatus_RomBusy;
        case Protocol::StatusCode_SystemReset: return OpStatus_SystemReset;
        }
        throw std::logic_error("Invalid Protocol::Status value");
      }


      struct SendEntry
      {
        uint16_t first;
        uint8_t  second;
      };



      static const std::array<SendEntry, 70> sendTable =
      { {
          // IN ReportId_PenData
        { OpDirection_Get | Protocol::ReportId_Status                      , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_Reset                       , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_HidInformation              , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_Information                 , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_Capability                  , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_Uid                         , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_Uid                         , OpStatus_Ready },
        { OpDirection_Get | Protocol::ReportId_Uid2                        , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_DefaultMode                 , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_DefaultMode                 , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_ReportRate                  , OpStatus_Ready | OpStatus_RomBusy},
        { OpDirection_Get | Protocol::ReportId_ReportRate                  , OpStatus_Ready | OpStatus_RomBusy},
        { OpDirection_Get | Protocol::ReportId_RenderingMode               , OpStatus_Ready | OpStatus_Capture | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_RenderingMode               , OpStatus_Ready | OpStatus_Capture | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_Eserial                     , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        // IN ReportId_PenDataEncrypted
        { OpDirection_Get | Protocol::ReportId_HostPublicKey               , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot },
        { OpDirection_Set | Protocol::ReportId_HostPublicKey               , OpStatus_Ready },
        { OpDirection_Get | Protocol::ReportId_DevicePublicKey             , OpStatus_Ready | OpStatus_Image | OpStatus_Capture /*OpStatus_Calculation*/ | OpStatus_Image_Boot },
        { OpDirection_Set | Protocol::ReportId_StartCapture                , OpStatus_Ready },
        { OpDirection_Set | Protocol::ReportId_EndCapture                  , OpStatus_Capture },
        { OpDirection_Get | Protocol::ReportId_DHprime                     , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot },
        { OpDirection_Set | Protocol::ReportId_DHprime                     , OpStatus_Ready },
        { OpDirection_Get | Protocol::ReportId_DHbase                      , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot },
        { OpDirection_Set | Protocol::ReportId_DHbase                      , OpStatus_Ready },
        { OpDirection_Set | Protocol::ReportId_ClearScreen                 , OpStatus_Ready },
        { OpDirection_Get | Protocol::ReportId_InkingMode                  , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_InkingMode                  , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_InkThreshold                , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_InkThreshold                , OpStatus_Ready },
        { OpDirection_Set | Protocol::ReportId_ClearScreenArea             , OpStatus_Ready },
        { OpDirection_Set | Protocol::ReportId_StartImageDataArea          , OpStatus_Ready /*OpStatus_Image*/ },
        { OpDirection_Set | Protocol::ReportId_StartImageData              , OpStatus_Ready /*OpStatus_Image*/ },
        { OpDirection_Set | Protocol::ReportId_ImageDataBlock              , OpStatus_Image },
        { OpDirection_Set | Protocol::ReportId_EndImageData                , OpStatus_Image },
        { OpDirection_Get | Protocol::ReportId_HandwritingThicknessColor   , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_HandwritingThicknessColor   , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_BackgroundColor             , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_BackgroundColor             , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_HandwritingDisplayArea      , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot },
        { OpDirection_Set | Protocol::ReportId_HandwritingDisplayArea      , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot },
        { OpDirection_Get | Protocol::ReportId_BacklightBrightness         , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_BacklightBrightness         , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_ScreenContrast              , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_ScreenContrast              , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Get | Protocol::ReportId_HandwritingThicknessColor24 , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_HandwritingThicknessColor24 , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_BackgroundColor24           , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_BackgroundColor24           , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_BootScreen                  , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_BootScreen                  , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_RomBusy },
        // IN ReportId_PenDataOption
        // IN ReportId_PenDataEncryptedOption
        { OpDirection_Get | Protocol::ReportId_PenDataOptionMode           , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_PenDataOptionMode           , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        // IN ReportId_PenDataTimeCountSequenceEncrypted
        // IN ReportId_PenDataTimeCountSequence
        { OpDirection_Get | Protocol::ReportId_EncryptionCommand           , OpStatus_Ready | OpStatus_Image | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_EncryptionCommand           , OpStatus_Ready },
        { OpDirection_Get | Protocol::ReportId_EncryptionStatus            , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_GetReport                   , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        // IN ReportId_SetResult
        // IN ReportId_PinPadData
        // IN ReportId_PinPadDataEncrypted
        { OpDirection_Get | Protocol::ReportId_PinOperationMode            , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Set | Protocol::ReportId_PinOperationMode            , OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy },
        { OpDirection_Get | Protocol::ReportId_OperationMode               , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_OperationMode               , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_RomStartImageData           , OpStatus_Ready                    },
        { OpDirection_Get | Protocol::ReportId_RomImageOccupancy           , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_RomImageOccupancy           , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Get | Protocol::ReportId_RomImageHash                , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_RomImageHash                , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Get | Protocol::ReportId_RomImageDelete              , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_RomImageDelete              , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Get | Protocol::ReportId_CurrentImageArea            , OpStatus_Ready | OpStatus_Capture },
        { OpDirection_Set | Protocol::ReportId_RomImageDisplay             , OpStatus_Ready | OpStatus_Capture },
        // IN ReportId_EventData
        // IN ReportId_EventDataEncrypted
        { OpDirection_Get | Protocol::ReportId_ReportSizeCollection, OpStatus_Ready | OpStatus_Image | OpStatus_Capture | OpStatus_Calculation | OpStatus_Image_Boot | OpStatus_RomBusy }
      } };



      static uint8_t opBitsFromReport(uint8_t reportId, OpDirection opDirection)
      {
        const uint16_t reportIdAndDirection = static_cast<uint16_t>(reportId | opDirection);

        for (auto i = sendTable.begin(); i != sendTable.end(); ++i)
        {
          if (i->first == reportIdAndDirection)
          {
            return i->second;
          }
        }
        throw std::logic_error("combination of reportId and opDirection is invalid");
      }



      bool statusCanSend(std::uint8_t statusCode, std::uint8_t reportId, OpDirection opDirection)
      {
        uint8_t bits = opBitsFromReport(reportId, opDirection);
        uint8_t opStatus = OpStatus_from_StatusCode(static_cast<Protocol::StatusCode>(statusCode));

        return (opStatus & bits) != 0;
      }



      Protocol::Status waitForStatusToSend(Protocol & protocol, uint8_t reportId, OpDirection opDirection, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        uint8_t bits = opBitsFromReport(reportId, opDirection);

        for (;;)
        {
          Protocol::Status status = protocol.getStatus();

          uint8_t opStatus = OpStatus_from_StatusCode(static_cast<Protocol::StatusCode>(status.statusCode));

          if ((opStatus & bits) != 0)
            return status;

          if (!retries)
            throw Interface::timeout_error();

          --retries;

#if WacomGSS_compatibility_thread == 0 // std
          this_thread::sleep_for(sleepBetweenRetries);
#elif WacomGSS_compatibility_thread == 1 // boost
          this_thread::sleep(::boost::posix_time::millisec(sleepBetweenRetries.count()));
#else
#error unexpected value for WacomGSS_compatibility_thread
#endif
        }
      }



      Protocol::Status waitForStatus(Protocol & protocol, std::uint8_t statusCode, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        for (;;)
        {
          Protocol::Status status = protocol.getStatus();
          if (status.statusCode == statusCode)
            return status;

          if (!retries)
            throw Interface::timeout_error();

          --retries;
#if WacomGSS_compatibility_thread == 0 // std
          this_thread::sleep_for(sleepBetweenRetries);
#elif WacomGSS_compatibility_thread == 1 // boost
          this_thread::sleep(::boost::posix_time::millisec(sleepBetweenRetries.count()));
#else
#error unexpected value for WacomGSS_compatibility_thread
#endif
        }
      }



      static void writeImageCommon(Protocol & protocol, Protocol::EncodingMode encodingMode, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        waitForStatusToSend(protocol, Protocol::ReportId_ImageDataBlock, OpDirection_Set, retries, sleepBetweenRetries);

        if ((encodingMode & Protocol::EncodingMode_Bulk) != 0)
        {
          // assumes protocol->supportsWrite() is true, will throw an exception if it isn't!
          protocol->write(data, length);
        }
        else
        {
          for (size_t m = 0; m < length;)
          {
            Protocol::ImageDataBlock dataBlock;

            size_t len = maxImageBlockSize;
            if ((length - m) < len)
            {
              len = length - m;
            }

            dataBlock.length = static_cast<uint16_t>(len & 0xffff);
            std::copy_n(data + m, len, dataBlock.data.begin());

            protocol.setImageDataBlock(dataBlock);

            m += len;
          }
        }

        protocol.setEndImageData(Protocol::EndImageDataFlag_Commit);

        // may require waitForStatus() here
      }



      void writeImage(Protocol & protocol, Protocol::EncodingMode encodingMode, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        waitForStatusToSend(protocol, Protocol::ReportId_StartImageData, OpDirection_Set, retries, sleepBetweenRetries);

        protocol.setStartImageData(encodingMode);

        writeImageCommon(protocol, encodingMode, maxImageBlockSize, data, length, retries, sleepBetweenRetries);
      }



      void writeImage(Protocol & protocol, Protocol::EncodingMode encodingMode, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        writeImage(protocol, encodingMode, Protocol::ImageDataBlock::maxLengthHID, data, length, retries, sleepBetweenRetries);
      }



      void writeImage(Protocol & protocol, uint8_t encodingMode, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        writeImage(protocol, static_cast<Protocol::EncodingMode>(encodingMode), Protocol::ImageDataBlock::maxLengthHID, data, length, retries, sleepBetweenRetries);
      }


      void writeRomImage(Protocol & protocol, Protocol::RomStartImageData const & romStartImageData, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        waitForStatusToSend(protocol, Protocol::ReportId_RomStartImageData, OpDirection_Set, retries, sleepBetweenRetries);

        protocol.setRomStartImageData(romStartImageData);

        writeImageCommon(protocol, romStartImageData.encodingMode, maxImageBlockSize, data, length, retries, sleepBetweenRetries);
      }


      void writeImageArea(Protocol & protocol, Protocol::EncodingMode encodingMode, Protocol::Rectangle const & area, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        waitForStatusToSend(protocol, Protocol::ReportId_StartImageDataArea, OpDirection_Set, retries, sleepBetweenRetries);
        
        protocol.setStartImageDataArea(encodingMode, area);

        writeImageCommon(protocol, encodingMode, maxImageBlockSize, data, length, retries, sleepBetweenRetries);        
      }



      void writeImageArea(Protocol & protocol, Protocol::EncodingMode encodingMode, Protocol::Rectangle const & area, uint8_t const * data, size_t length, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        writeImageArea(protocol, encodingMode, area, Protocol::ImageDataBlock::maxLengthHID, data, length, retries, sleepBetweenRetries);
      }



      bool supportsEncryption(Protocol::DHprime const & prime) noexcept
      {
        // returns true if prime is non-zero.
        return !std::all_of(prime.begin(), prime.end(), [] (uint8_t v) -> bool { return v == 0; });
      }



      bool supportsEncryption(Protocol & protocol)
      {
        std::array<std::uint16_t, 256> reportCountLengths;
        return (protocol->getReportCountLengths(reportCountLengths) && !reportCountLengths[Protocol::ReportId_DHprime]) ? false : supportsEncryption(protocol.getDHprime());
      }



      Protocol::DevicePublicKey waitForDevicePublicKey(InterfaceQueue & interfaceQueue, chrono::milliseconds timeout)
      {
        auto absTime = chrono::steady_clock::now() + timeout;

        Report report;
        while (interfaceQueue.wait_until_getReport(report, absTime))
        {
          if (report.front() == Protocol::DevicePublicKey::reportId && report.size()-1 >= Protocol::DevicePublicKey::reportSize) 
          {
            Protocol::DevicePublicKey devicePublicKey;
            Protocol::decodeReport_unsafe(report.begin()+1, devicePublicKey); // 'unsafe' can be used here as the report size has already been checked.
            return devicePublicKey;
          }
        }
        
        throw Interface::timeout_error();
      }



      Protocol::PublicKey setHostPublicKeyAndWaitForDevicePublicKey(Protocol & protocol, Protocol::PublicKey const & hostPublicKey, chrono::milliseconds timeout)
      {
        auto interfaceQueue = protocol->interfaceQueue();

        protocol.setHostPublicKey(hostPublicKey);

        return waitForDevicePublicKey(interfaceQueue, timeout);
      }



      Protocol::PublicKey setHostPublicKeyAndPollForDevicePublicKey(Protocol & protocol, Protocol::PublicKey const & hostPublicKey, int retries, chrono::milliseconds sleepBetweenRetries)
      {
        protocol.setHostPublicKey(hostPublicKey);
        waitForStatusToSend(protocol, Protocol::ReportId_DevicePublicKey, OpDirection_Get, retries, sleepBetweenRetries);        
        return protocol.getDevicePublicKey();
      }



      Protocol::EncryptionStatus generateSymmetricKeyAndWaitForEncryptionStatus(Protocol & protocol, int retries, chrono::milliseconds sleepBetweenRetries, chrono::milliseconds timeout)
      {
        waitForStatusToSend(protocol, Protocol::ReportId_EncryptionCommand, OpDirection_Set, retries, sleepBetweenRetries);

        auto interfaceQueue = protocol->interfaceQueue();
      
        protocol.setEncryptionCommand(Protocol::EncryptionCommand::initializeGenerateSymmetricKey());
        
        {
          auto encryptionStatus = protocol.getEncryptionStatus();

          if (encryptionStatus.lastResultCode != Protocol::ErrorCodeRSA_None)
          {
            return encryptionStatus;
          }
        }

        auto absTime = chrono::steady_clock::now() + /*5 **/ timeout * 5;

        Report report;
        while (interfaceQueue.wait_until_getReport(report, absTime))
        {
          if (report.front() == Protocol::EncryptionStatus::reportId && report.size()-1 >= Protocol::EncryptionStatus::reportSize) 
          {
            Protocol::EncryptionStatus encryptionStatus;
            Protocol::decodeReport_unsafe(report.begin()+1, encryptionStatus); // 'unsafe' can be used here as the report size has already been checked.
            return encryptionStatus;
          }
        }
        
        throw Interface::timeout_error();
      }



      ValidatingInterfaceImpl::ValidatingInterfaceImpl()
      :
        m_initialized(false),
        m_validationSupported(false)
      {
      }



      bool ValidatingInterfaceImpl::initialized() const noexcept
      {
        return m_initialized;
      }



      std::array<std::uint16_t,256> & ValidatingInterfaceImpl::reportCountLengths() noexcept
      {
        return m_reportCountLengths;
      }



      void ValidatingInterfaceImpl::initialize(bool validationSupported) noexcept
      {
        m_validationSupported = validationSupported;
        m_initialized = true;
      }



      bool ValidatingInterfaceImpl::validate(std::uint8_t const * data, std::size_t length, OpDirection opDirection)
      {
#if 0
        if (!m_initialized)
        {
          throw std::logic_error("initialize() has not been called");
        }
#endif
        if (data && length)
        {
          if (m_validationSupported)
          {
            std::uint8_t  reportId       = data[0];
            std::uint16_t expectedLength = m_reportCountLengths[reportId];
            if (expectedLength == 0)
            {
              return onFeatureNotSupported(data, length, opDirection);
            }

            if (expectedLength != length)
            {
              return onFeatureLengthMismatch(data, length, expectedLength, opDirection);
            }
          }
        }
        return true;
      }



      bool ValidatingInterfaceImpl::onFeatureNotSupported(std::uint8_t const *, std::size_t, OpDirection)
      {
        throw feature_not_supported();
      }



      bool ValidatingInterfaceImpl::onFeatureLengthMismatch(std::uint8_t const *, std::size_t, std::uint16_t, OpDirection)
      {
        throw feature_length_mismatch();
      }



      uint8_t simulateEncodingFlag(uint16_t idProduct, uint8_t encodingFlag)
      {
        if ((encodingFlag & (Protocol::EncodingFlag_24bit|Protocol::EncodingFlag_16bit|Protocol::EncodingFlag_1bit)) == 0)
        {
          switch (idProduct)
          {            
            case ProductId_541 :
            case ProductId_540 :
            case ProductId_530 : encodingFlag = Protocol::EncodingFlag_24bit|Protocol::EncodingFlag_16bit|Protocol::EncodingFlag_1bit|Protocol::EncodingFlag_Zlib; break;
            case ProductId_520A: encodingFlag = Protocol::EncodingFlag_16bit|Protocol::EncodingFlag_1bit; break;
            case ProductId_500 : encodingFlag = Protocol::EncodingFlag_1bit; break;
            case ProductId_430V:
            case ProductId_430 : encodingFlag = Protocol::EncodingFlag_1bit; break;
            case ProductId_300 : encodingFlag = Protocol::EncodingFlag_1bit; break;
            default:
              encodingFlag = Protocol::EncodingFlag_1bit;
          }
        }
        return encodingFlag;
      }



      bool encodingFlagSupportsColor(uint8_t encodingFlag) noexcept
      {
        return (encodingFlag & (Protocol::EncodingFlag_24bit|Protocol::EncodingFlag_16bit)) != 0;
      }



/*      
      template<bool>
      struct EncodingTypeImpl
      {
        static void handle_default();
                
        static bool supportedEncodingType(std::uint16_t idProduct, Protocol::EncodingType encodingType, std::uint8_t encodingFlags)
        {
          if ((encodingFlags & (Protocol::EncodingType_1bit|Protocol::EncodingType_16bit|Protocol::EncodingType_24bit)) != 0)
          {
            // encodingFlags is valid, use it!
            return (encodingType & encodingFlags) != 0;
          }

          // encodingFlags is not valid, use best guess by idProduct
          switch (encodingType)
          {
            case EncodingType_24bit:
              if (idProduct == ProductId_530)
                return true;
              break;

            case EncodingType_16bit:
              if (idProduct == ProductId_520A || idProduct == ProductId_530)
                return true;
              break;

            case EncodingType_1bit:
              return true;

            default:
              handle_default();
              
          }
          return false;        
        }

        static EncodingType initializeBestEncodingType(std::uint16_t idProduct)
        {
          switch (idProduct)
          {
            case ProductId_540:
            case ProductId_530: return EncodingType_24bit;
            case ProductId_520A: return EncodingType_16bit;
            case ProductId_500:
            case ProductId_430:
            case ProductId_300:
              break;
            default:
              handle_default();
          }
          return EncodingType_1bit;
        }      
      };

      template<> void EncodingTypeImpl<true>::handle_default()
      {
        throw std::out_of_range("unknown idProduct");
      }

      template<> void EncodingTypeImpl<false>::handle_default()
      {
      }

      bool supportedEncodingType(std::uint16_t idProduct, EncodingType encodingType)
      {
        return EncodingTypeImpl<true>::supportedEncodingType(idProduct, encodingType);
      }

      bool supportedEncodingType(std::uint16_t idProduct, EncodingType encodingType, nothrow_t) noexcept
      {
        return EncodingTypeImpl<false>::supportedEncodingType(idProduct, encodingType);
      }
            
         

      EncodingType initializeBestEncodingType(std::uint16_t idProduct)
      {
        return EncodingTypeImpl<true>::initializeBestEncodingType(idProduct);        
      }
      
      EncodingType initializeBestEncodingType(std::uint16_t idProduct, nothrow_t)
      {
        return EncodingTypeImpl<false>::initializeBestEncodingType(idProduct);        
      }
*/



      WacomGSS_STU_runtime_error_impl(ValidatingInterfaceImpl::feature_not_supported, "feature not supported")
      WacomGSS_STU_runtime_error_impl(ValidatingInterfaceImpl::feature_length_mismatch, "feature length mismatch")



      namespace ostream_operators
      {
#define WacomGSS_enum_begin()         switch (value) {
#define WacomGSS_enum_case(TYPE,NAME)   case TYPE##_##NAME: return o << #NAME;
#define WacomGSS_enum_end(TYPE)       } return o << static_cast<TYPE>(value);



        std::ostream & operator << (std::ostream & o, OpDirection value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(OpDirection,Get)
          WacomGSS_enum_case(OpDirection,Set)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::StatusCode value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::StatusCode,Ready      )
          WacomGSS_enum_case(Protocol::StatusCode,Image      )
          WacomGSS_enum_case(Protocol::StatusCode,Capture    )
          WacomGSS_enum_case(Protocol::StatusCode,Calculation)
          WacomGSS_enum_case(Protocol::StatusCode,Image_Boot )
          WacomGSS_enum_case(Protocol::StatusCode,RomBusy    )
          WacomGSS_enum_case(Protocol::StatusCode,SystemReset)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::ErrorCode value)
        {
          //Value >= 0x00 and value <= 0xff always evaluate to false
      //if (value >= 0x80 && value <= 0xff) return o << "Internal";

          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::ErrorCode,None                     )
          WacomGSS_enum_case(Protocol::ErrorCode,WrongReportId            )
          WacomGSS_enum_case(Protocol::ErrorCode,WrongState               )
          WacomGSS_enum_case(Protocol::ErrorCode,CRC                      )
          WacomGSS_enum_case(Protocol::ErrorCode,BadParameter             )
          WacomGSS_enum_case(Protocol::ErrorCode,GraphicsWrongEncodingType)
          WacomGSS_enum_case(Protocol::ErrorCode,GraphicsImageTooLong     )
          WacomGSS_enum_case(Protocol::ErrorCode,GraphicsZlibError        )
          WacomGSS_enum_case(Protocol::ErrorCode,GraphicsWrongParameters  )
          WacomGSS_enum_case(Protocol::ErrorCode,PadNotExist              )
          WacomGSS_enum_case(Protocol::ErrorCode,RomSizeOverflow          )
          WacomGSS_enum_case(Protocol::ErrorCode,RomInvalidParameter      )
          WacomGSS_enum_case(Protocol::ErrorCode,RomErrorCRC              )
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::ResetFlag value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::ResetFlag,Software)
          WacomGSS_enum_case(Protocol::ResetFlag,Hardware)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::DefaultMode value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::DefaultMode, HID)
          WacomGSS_enum_case(Protocol::DefaultMode, Serial)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::InkingMode value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::InkingMode,Off)
          WacomGSS_enum_case(Protocol::InkingMode,On )
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::ReportId value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::ReportId,PenData                          )
          WacomGSS_enum_case(Protocol::ReportId,Status                           )
          WacomGSS_enum_case(Protocol::ReportId,Reset                            )
          WacomGSS_enum_case(Protocol::ReportId,Information                      )
          WacomGSS_enum_case(Protocol::ReportId,HidInformation                   )
          WacomGSS_enum_case(Protocol::ReportId,Capability                       )
          WacomGSS_enum_case(Protocol::ReportId,Uid                              )
          WacomGSS_enum_case(Protocol::ReportId,Uid2                             )
          WacomGSS_enum_case(Protocol::ReportId,DefaultMode                      )
          WacomGSS_enum_case(Protocol::ReportId,ReportRate                       )
          WacomGSS_enum_case(Protocol::ReportId,RenderingMode                    )
          WacomGSS_enum_case(Protocol::ReportId,Eserial                          )
          WacomGSS_enum_case(Protocol::ReportId,PenDataEncrypted                 )
          WacomGSS_enum_case(Protocol::ReportId,HostPublicKey                    )
          WacomGSS_enum_case(Protocol::ReportId,DevicePublicKey                  )
          WacomGSS_enum_case(Protocol::ReportId,StartCapture                     )
          WacomGSS_enum_case(Protocol::ReportId,EndCapture                       )
          WacomGSS_enum_case(Protocol::ReportId,DHprime                          )
          WacomGSS_enum_case(Protocol::ReportId,DHbase                           )
          WacomGSS_enum_case(Protocol::ReportId,ClearScreen                      )
          WacomGSS_enum_case(Protocol::ReportId,InkingMode                       )
          WacomGSS_enum_case(Protocol::ReportId,InkThreshold                     )
          WacomGSS_enum_case(Protocol::ReportId,ClearScreenArea                  )
          WacomGSS_enum_case(Protocol::ReportId,StartImageDataArea               )
          WacomGSS_enum_case(Protocol::ReportId,StartImageData                   )
          WacomGSS_enum_case(Protocol::ReportId,ImageDataBlock                   )
          WacomGSS_enum_case(Protocol::ReportId,EndImageData                     )
          WacomGSS_enum_case(Protocol::ReportId,HandwritingThicknessColor        )
          WacomGSS_enum_case(Protocol::ReportId,BackgroundColor                  )
          WacomGSS_enum_case(Protocol::ReportId,HandwritingDisplayArea           )
          WacomGSS_enum_case(Protocol::ReportId,BacklightBrightness              )
          WacomGSS_enum_case(Protocol::ReportId,ScreenContrast                   )
          WacomGSS_enum_case(Protocol::ReportId,HandwritingThicknessColor24      )
          WacomGSS_enum_case(Protocol::ReportId,BackgroundColor24                )
          WacomGSS_enum_case(Protocol::ReportId,BootScreen                       )
          WacomGSS_enum_case(Protocol::ReportId,PenDataOption                    )
          WacomGSS_enum_case(Protocol::ReportId,PenDataEncryptedOption           )
          WacomGSS_enum_case(Protocol::ReportId,PenDataOptionMode                )
          WacomGSS_enum_case(Protocol::ReportId,PenDataTimeCountSequenceEncrypted)
          WacomGSS_enum_case(Protocol::ReportId,PenDataTimeCountSequence         )
          WacomGSS_enum_case(Protocol::ReportId,EncryptionCommand                )
          WacomGSS_enum_case(Protocol::ReportId,EncryptionStatus                 )
          WacomGSS_enum_case(Protocol::ReportId,GetReport                        )
          WacomGSS_enum_case(Protocol::ReportId,SetResult                        )
          WacomGSS_enum_case(Protocol::ReportId,PinPadData                       )
          WacomGSS_enum_case(Protocol::ReportId,PinPadDataEncrypted              )
          WacomGSS_enum_case(Protocol::ReportId,PinOperationMode                 )
          WacomGSS_enum_case(Protocol::ReportId,OperationMode                    )
          WacomGSS_enum_case(Protocol::ReportId,RomStartImageData                )          
          WacomGSS_enum_case(Protocol::ReportId,RomImageOccupancy                )
          WacomGSS_enum_case(Protocol::ReportId,RomImageHash                     )
          WacomGSS_enum_case(Protocol::ReportId,RomImageDelete                   )
          WacomGSS_enum_case(Protocol::ReportId,CurrentImageArea                 )
          WacomGSS_enum_case(Protocol::ReportId,EventData                        )
          WacomGSS_enum_case(Protocol::ReportId,EventDataEncrypted               )
          WacomGSS_enum_case(Protocol::ReportId,RomImageDisplay                  )
          WacomGSS_enum_case(Protocol::ReportId,ReportSizeCollection             )
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::EncodingMode value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::EncodingMode,1bit      )
          WacomGSS_enum_case(Protocol::EncodingMode,1bit_Zlib )          
          WacomGSS_enum_case(Protocol::EncodingMode,16bit     )
          WacomGSS_enum_case(Protocol::EncodingMode,24bit     )
          WacomGSS_enum_case(Protocol::EncodingMode,1bit_Bulk )
          WacomGSS_enum_case(Protocol::EncodingMode,16bit_Bulk)
          WacomGSS_enum_case(Protocol::EncodingMode,24bit_Bulk)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::EncodingFlag value)
        {
          WacomGSS_enum_begin()
            WacomGSS_enum_case(Protocol::EncodingFlag, Zlib)
            WacomGSS_enum_case(Protocol::EncodingFlag, 1bit)
            WacomGSS_enum_case(Protocol::EncodingFlag, 16bit)
            WacomGSS_enum_case(Protocol::EncodingFlag, 24bit)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::PenDataOptionMode value)
        {          
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::PenDataOptionMode,None             )
          WacomGSS_enum_case(Protocol::PenDataOptionMode,TimeCount        )
          WacomGSS_enum_case(Protocol::PenDataOptionMode,SequenceNumber   )
          WacomGSS_enum_case(Protocol::PenDataOptionMode,TimeCountSequence)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::EndImageDataFlag value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::EndImageDataFlag,Commit )
          WacomGSS_enum_case(Protocol::EndImageDataFlag,Abandon)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::StatusCodeRSA value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::StatusCodeRSA,Ready      )
          WacomGSS_enum_case(Protocol::StatusCodeRSA,Calculating)
          WacomGSS_enum_case(Protocol::StatusCodeRSA,Even       )
          WacomGSS_enum_case(Protocol::StatusCodeRSA,Long       )
          WacomGSS_enum_case(Protocol::StatusCodeRSA,Short      )
          WacomGSS_enum_case(Protocol::StatusCodeRSA,Invalid    )
          WacomGSS_enum_case(Protocol::StatusCodeRSA,NotReady   )
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::ErrorCodeRSA value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,None                  )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,BadParameter          )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,ParameterTooLong      )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,PublicKeyNotReady     )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,PublicExponentNotReady)
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,SpecifiedKeyInUse     )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,SpecifiedKeyNotInUse  )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,BadCommandCode        )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,CommandPending        )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,SpecifiedKeyExists    )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,SpecifiedKeyNotExist  )
          WacomGSS_enum_case(Protocol::ErrorCodeRSA,NotInitialized        )
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::SymmetricKeyType value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::SymmetricKeyType,AES128)
          WacomGSS_enum_case(Protocol::SymmetricKeyType,AES192)
          WacomGSS_enum_case(Protocol::SymmetricKeyType,AES256)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::AsymmetricKeyType value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::AsymmetricKeyType,RSA1024)
          WacomGSS_enum_case(Protocol::AsymmetricKeyType,RSA1536)
          WacomGSS_enum_case(Protocol::AsymmetricKeyType,RSA2048)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::AsymmetricPaddingType value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::AsymmetricPaddingType,None )
          WacomGSS_enum_case(Protocol::AsymmetricPaddingType,PKCS1)
          WacomGSS_enum_case(Protocol::AsymmetricPaddingType,OAEP )
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::EncryptionCommandNumber value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::EncryptionCommandNumber,SetEncryptionType   )
          WacomGSS_enum_case(Protocol::EncryptionCommandNumber,SetParameterBlock   )
          WacomGSS_enum_case(Protocol::EncryptionCommandNumber,GetStatusBlock      )
          WacomGSS_enum_case(Protocol::EncryptionCommandNumber,GetParameterBlock   )
          WacomGSS_enum_case(Protocol::EncryptionCommandNumber,GenerateSymmetricKey)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::EncryptionCommandParameterBlockIndex value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::EncryptionCommandParameterBlockIndex,RSAe)
          WacomGSS_enum_case(Protocol::EncryptionCommandParameterBlockIndex,RSAn)
          WacomGSS_enum_case(Protocol::EncryptionCommandParameterBlockIndex,RSAc)
          WacomGSS_enum_case(Protocol::EncryptionCommandParameterBlockIndex,RSAm)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::OperationModeType value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::OperationModeType,Normal   )
          WacomGSS_enum_case(Protocol::OperationModeType,PinPad   )
          WacomGSS_enum_case(Protocol::OperationModeType,SlideShow)
          WacomGSS_enum_case(Protocol::OperationModeType,KeyPad   )
          WacomGSS_enum_case(Protocol::OperationModeType,Signature)
          WacomGSS_enum_case(Protocol::OperationModeType,MessageBox)

          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::RomImageDeleteMode value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,All             )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,PinPad_All      )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,SlideShow_All   )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,KeyPad_All      )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,Signature_All   )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,MessageBox_All  )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,PinPad_Number   )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,SlideShow_Number)
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,KeyPad_Number   )
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,Signature_Number)
          WacomGSS_enum_case(Protocol::RomImageDeleteMode,MessageBox_Number)
          WacomGSS_enum_end(std::uint16_t)
        }



        std::ostream & operator << (std::ostream & o, Protocol::RenderingMode value)
        {
          WacomGSS_enum_begin()
          WacomGSS_enum_case(Protocol::RenderingMode,Legacy)
          WacomGSS_enum_case(Protocol::RenderingMode,WILL  )
          WacomGSS_enum_end(std::uint16_t)
        }


      } // namespace ostream_operators


    } // namespace ProtocolHelper

  } // namespace STU

} // namespace WacomGSS
