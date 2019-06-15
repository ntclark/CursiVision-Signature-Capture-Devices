/// @file      WacomGSS/STU/ReportHandler.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2012-09-06
/// @brief     Provides simplified decoding of reports.

#ifndef WacomGSS_STU_ReportHandler_hpp
#define WacomGSS_STU_ReportHandler_hpp

#include <WacomGSS/STU/Protocol.hpp>
#include <WacomGSS/STU/TlsProtocol.hpp>


namespace WacomGSS
{
  namespace STU
  {
    namespace ProtocolHelper
    {

      /// @brief  Base class that provides the methods to override.
      ///
      /// There is no automatic cascade for PenData if methods are not overriden. It is recommended that you override all 
      /// PenData* methods and cascade to onReport(PenData &).
      class ReportHandlerBase
      {
      public:
        virtual ~ReportHandlerBase();
  
        /// @brief  Called when pen data is received from the tablet.
        /// @param  penData Pen data received from the tablet.
        virtual void onReport(Protocol::PenData & penData);

        /// @brief  Called when pen data with an option value is received from the tablet.
        /// @param  penDataOption Pen data received from the tablet.
        virtual void onReport(Protocol::PenDataOption & penDataOption);

        /// @brief  Called when pen data with time count and sequence number is received from the tablet.
        /// @param  penDataTimeCountSequence Pen data received from the tablet.
        virtual void onReport(Protocol::PenDataTimeCountSequence & penDataTimeCountSequence);

        /// @brief  Called when encrypted pen data is received from the tablet.
        /// @param  penDataEncrypted Decrypted pen data received from the tablet.
        virtual void onReport(Protocol::PenDataEncrypted & penDataEncrypted);

        /// @brief  Called when encrypted pen data with an option value is received from the tablet.
        /// @param  penDataEncryptedOption Decrypted pen data received from the tablet.
        virtual void onReport(Protocol::PenDataEncryptedOption & penDataEncryptedOption);
        
        /// @brief  Called when pen data with time count and sequence number is received from the tablet.
        /// @param  penDataTimeCountSequenceEncrypted Pen data received from the tablet.
        virtual void onReport(Protocol::PenDataTimeCountSequenceEncrypted & penDataTimeCountSequenceEncrypted);

        /// @brief  Called when a public key is received from the tablet.
        /// @param  key Public key received from the tablet.
        virtual void onReport(Protocol::DevicePublicKey & key);

        /// @brief  Called when the encryption status is received from the tablet.
        /// @param  encryptionStatus  The current encryption status of the tablet.
        virtual void onReport(Protocol::EncryptionStatus & encryptionStatus);
                
        virtual void onReport(Protocol::EventDataPinPad & eventDataPinPad);
        virtual void onReport(Protocol::EventDataKeyPad & eventDataKeyPad);
        virtual void onReport(Protocol::EventDataSignature & eventDataSignature);
        virtual void onReport(Protocol::EventDataEncrypted & eventData);
        virtual void onReport(Protocol::EventDataPinPadEncrypted & eventDataPinPad);
        virtual void onReport(Protocol::EventDataKeyPadEncrypted & eventDataKeyPad);
        virtual void onReport(Protocol::EventDataSignatureEncrypted & eventDataSignature);

        /// @brief  Called to decrypt encrypted data received from the tablet.
        /// @param  data Encrypted data received from the tablet.
        virtual void decrypt(uint8_t data[Protocol::PenDataEncrypted::encryptedSize]);
      };


      /// @brief Class to simplify Report decoding.
      ///
      /// You should derive from this class and override the virtual functions inherited from base class ReportHandlerBase.
      class ReportHandler : public ReportHandlerBase
      {
        using ReportHandlerBase::onReport;

        void onReport(Protocol::EventData & eventData)
        {
          switch (eventData.operationMode)
          {
            case Protocol::OperationModeType_PinPad:    { auto data = Protocol::EventDataPinPad::initialize(eventData);    onReport(data); break; }
            case Protocol::OperationModeType_KeyPad:    { auto data = Protocol::EventDataKeyPad::initialize(eventData);    onReport(data); break; }
            case Protocol::OperationModeType_Signature: { auto data = Protocol::EventDataSignature::initialize(eventData); onReport(data); break; }
          }
        }
        
        void onReport(Protocol::EventDataEncrypted & eventData)
        {
          switch (eventData.operationMode)
          {
            case Protocol::OperationModeType_PinPad:    { auto data = Protocol::EventDataPinPadEncrypted::initialize(eventData);    onReport(data); break; }
            case Protocol::OperationModeType_KeyPad:    { auto data = Protocol::EventDataKeyPadEncrypted::initialize(eventData);    onReport(data); break; }
            case Protocol::OperationModeType_Signature: { auto data = Protocol::EventDataSignatureEncrypted::initialize(eventData); onReport(data); break; }
          }
        }

        template<class Report, typename Iterator>
        bool decode(Iterator & begin, Iterator end, std::input_iterator_tag)
        {
          Report report;
          try
          {
            ++begin;
            begin = Protocol::decodeReport(begin, end, report);
          }
          catch (std::range_error &)
          {
            return false;
          }
          onReport(report);
          return true;
        }
      
        template<class Report, typename Iterator>
        bool decode(Iterator & begin, Iterator end, std::forward_iterator_tag)
        {
          auto d = std::distance(begin+1,end);
          if (d < 0 || static_cast<size_t>(d) < Report::reportSize) 
            return false;

          Report report;
          ++begin;
          begin = Protocol::decodeReport(begin, end, report);
          onReport(report);
          return true;
        }


        template<class Report, typename InputIterator>
        bool decodeEncrypted(InputIterator & begin, InputIterator end, std::input_iterator_tag)
        {
          std::array<uint8_t, Report::reportSize> buffer;
          try
          {
            CheckedInputIteratorReference<InputIterator> checked(begin, end);
            ++checked;
            std::copy_n(checked, buffer.size(), buffer.begin());
          }
          catch (std::range_error &)
          {
            return false;
          }

          static_assert(Report::reportSize >= Protocol::PenDataEncrypted::encryptedSize, "Encryption data block size mismatch");
          decrypt(buffer.data());

          Report report;
          WacomGSS::STU::Protocol::decodeReport(buffer.begin(), buffer.end(), report);
      
          memset_s(buffer.data(), 0x00, buffer.size());
      
          onReport(report);

          memset_s(&report, 0x00, sizeof(report));

          return true;
        }


        template<class Report, typename Iterator>
        bool decodeEncrypted(Iterator & begin, Iterator end, std::forward_iterator_tag)
        {
          Iterator begin1(begin);
          ++begin1;

          auto d = std::distance(begin1,end);
          if (d < 0 || static_cast<size_t>(d) < Report::reportSize) 
            return false;

          std::array<uint8_t, Report::reportSize> buffer;
          std::copy(begin1, begin1 + Report::reportSize, buffer.begin());

          static_assert(Report::reportSize >= Protocol::PenDataEncrypted::encryptedSize, "Encryption data block size mismatch");
          decrypt(buffer.data());

          Report report;
          WacomGSS::STU::Protocol::decodeReport(buffer.begin(), buffer.end(), report);
      
          memset_s(buffer.data(), 0x00, buffer.size());
      
          onReport(report);

          memset_s(&report, 0x00, sizeof(report));

          begin += 1 + Report::reportSize;
          return true;
        }


        template<typename Iterator>
        bool decodeTls(Iterator & begin, Iterator end, std::forward_iterator_tag)
        {
          auto d = std::distance(begin, end);
          if (d >= 6)
          {
            Iterator begin1(begin);
            uint8_t header[6];
            for (auto & b : header)
            {
              b = *begin1;
              ++begin1;
            }
            int size = (header[0] << 24) | (header[1] << 16) | (header[2] << 8) | header[3];
            if (d >= size)
            {
              uint16_t packetId = static_cast<uint16_t>((header[4] << 8) | header[5]);
              switch (packetId)
              {
                case TlsProtocol::PacketId_Legacy_PenData:
                {                  
                  Protocol::PenDataTimeCountSequence report;
                  begin = Protocol::decodeReport(begin1, end, report);
                  onReport(report);
                  return true;
                }

                case TlsProtocol::PacketId_Legacy_EventData:
                {
                  Protocol::EventData report;
                  begin = Protocol::decodeReport(begin1, end, report);
                  onReport(report);
                  return true;
                }

                default:
                  return false;
              }
            }
          }
          return false;
        }

      public:
        /// @brief  Provides simplified decoding of Reports.
        /// @param  begin Iterator specifying start of data to process.
        /// @param  end Iterator specifying end of data to process.
        /// @return a pair which contains an Iterator specifying where processing finished, and a bool indicating if all data has been consumed.
        template<typename Iterator>
        std::pair<Iterator, bool> handleReport(Iterator begin, Iterator end, bool tlsDecode)
        {
          bool success = true;

          while (success && begin != end)
          {
            if (tlsDecode)
            {
              success = decodeTls(begin, end, typename std::iterator_traits<Iterator>::iterator_category());
            }
            else
            {
              switch (*begin)
              {
                case Protocol::PenData::reportId:                           success = decode<Protocol::PenData                        >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::PenDataOption::reportId:                     success = decode<Protocol::PenDataOption                  >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::PenDataTimeCountSequence::reportId:          success = decode<Protocol::PenDataTimeCountSequence       >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::PenDataEncrypted::reportId:                  success = decodeEncrypted<Protocol::PenDataEncrypted      >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::PenDataEncryptedOption::reportId:            success = decodeEncrypted<Protocol::PenDataEncryptedOption>(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::PenDataTimeCountSequenceEncrypted::reportId: success = decodeEncrypted<Protocol::PenDataTimeCountSequenceEncrypted>(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::DevicePublicKey::reportId:                   success = decode<Protocol::DevicePublicKey                >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::EncryptionStatus::reportId:                  success = decode<Protocol::EncryptionStatus               >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::EventData::reportId:                         success = decode<Protocol::EventData                      >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                case Protocol::EventDataEncrypted::reportId:                success = decodeEncrypted<Protocol::EventDataEncrypted    >(begin, end, typename std::iterator_traits<Iterator>::iterator_category()); break;
                default:
                  return std::pair<Iterator, bool>(begin, false);
              }
            }
          }
          return std::pair<Iterator,bool>(begin,true);
        }


        
        template<typename Iterator>
        WacomGSS_deprecated
        std::pair<Iterator, bool> handleReport(Iterator begin, Iterator end)
        {
          return handleReport(begin, end, false);
        }

      };


    }
  }
}

#endif // WacomGSS_STU_ReportHandler_hpp
