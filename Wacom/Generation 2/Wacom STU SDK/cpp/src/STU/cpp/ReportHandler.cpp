#include <WacomGSS/STU/ReportHandler.hpp>

namespace WacomGSS
{
  namespace STU
  {
    namespace ProtocolHelper
    {


      ReportHandlerBase::~ReportHandlerBase()
      {
      }



      void ReportHandlerBase::onReport(Protocol::PenData &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::PenDataOption &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::PenDataTimeCountSequence &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::PenDataEncrypted &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::PenDataEncryptedOption &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::PenDataTimeCountSequenceEncrypted &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::DevicePublicKey &)
      {
      }



      void ReportHandlerBase::onReport(Protocol::EncryptionStatus &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataPinPad &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataKeyPad &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataSignature &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataEncrypted &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataPinPadEncrypted &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataKeyPadEncrypted &)
      {
      }


      void ReportHandlerBase::onReport(Protocol::EventDataSignatureEncrypted &)
      {
      }



      void ReportHandlerBase::decrypt(uint8_t [Protocol::PenDataEncrypted::encryptedSize])
      {
      }


    }
  }
}
