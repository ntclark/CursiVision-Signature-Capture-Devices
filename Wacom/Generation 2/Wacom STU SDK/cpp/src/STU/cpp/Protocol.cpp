#include <WacomGSS/STU/Protocol.hpp>
#include <WacomGSS/STU/Interface.hpp>
#include <WacomGSS/compatibility/thread.hpp>

namespace WacomGSS
{
  namespace STU
  {
    const size_t Protocol::PenData::reportSize;
    const size_t Protocol::PenDataOption::reportSize;
    const size_t Protocol::EncryptionStatus::reportSize;
    const size_t Protocol::PenDataTimeCountSequence::reportSize;
    const size_t Protocol::PenDataTimeCountSequenceEncrypted::reportSize;
    const size_t Protocol::EventDataPinPad::reportSize;
    const size_t Protocol::EventDataKeyPad::reportSize;
    const size_t Protocol::EventDataSignature::reportSize;
    const size_t Protocol::EventDataPinPadEncrypted::reportSize;
    const size_t Protocol::EventDataKeyPadEncrypted::reportSize;
    const size_t Protocol::EventDataSignatureEncrypted::reportSize;
    const size_t Protocol::PenDataEncryptedOption::reportSize;
    const size_t Protocol::PenDataEncrypted::reportSize;
    const size_t Protocol::PenDataEncrypted::encryptedSize;
    const size_t Protocol::PenDataTimeCountSequenceEncrypted::encryptedSize;


    Protocol::Status Protocol::getStatus()
    {
      std::array<uint8_t,5> cmd = { ReportId_Status };

      get(cmd);

      Status data;

      data.statusCode     = static_cast<StatusCode>(cmd[1]);
      data.lastResultCode = static_cast<ErrorCode>(cmd[2]);
      data.statusWord     = static_cast<uint16_t>(cmd[3] | cmd[4] << 8);

      return std::move(data);
    }



    void Protocol::setReset(ResetFlag resetFlag)
    {
      std::array<uint8_t,2> cmd = { ReportId_Reset, resetFlag };

      set(cmd);
    }



    Protocol::HidInformation Protocol::getHidInformation()
    {
      std::array<uint8_t, 9> cmd = { ReportId_HidInformation };

      get(cmd);

      HidInformation data;

      data.idVendor  = static_cast<uint16_t>(cmd[1] << 8 | cmd[2]);
      data.idProduct = static_cast<uint16_t>(cmd[3] << 8 | cmd[4]);
      data.bcdDevice = static_cast<uint16_t>(cmd[5] << 8 | cmd[6]);
      // reserved (padding 0) = static_cast<uint16_t>(cmd[7] << 8 | cmd[8])

      return std::move(data);
    }


    Protocol::Information Protocol::getInformation()
    {
      std::array<uint8_t,17> cmd = { ReportId_Information };

      get(cmd);

      Information data;

      data.modelName[0]               = static_cast<char>(cmd[ 1]);
      data.modelName[1]               = static_cast<char>(cmd[ 2]);
      data.modelName[2]               = static_cast<char>(cmd[ 3]);
      data.modelName[3]               = static_cast<char>(cmd[ 4]);
      data.modelName[4]               = static_cast<char>(cmd[ 5]);
      data.modelName[5]               = static_cast<char>(cmd[ 6]);
      data.modelName[6]               = static_cast<char>(cmd[ 7]);
      data.modelName[7]               = static_cast<char>(cmd[ 8]);
      data.modelName[8]               = static_cast<char>(cmd[ 9]);
      data.modelNameNullTerminated[9] = 0;
      data.firmwareMajorVersion       = cmd[10];
      data.firmwareMinorVersion       = cmd[11];
      data.secureIc                   = cmd[12]; // & 0x01U;
      data.secureIcVersion[0]         = cmd[13];
      data.secureIcVersion[1]         = cmd[14];
      data.secureIcVersion[2]         = cmd[15];
      data.secureIcVersion[3]         = cmd[16];

      return std::move(data);
    }



    Protocol::Capability Protocol::getCapability()
    {
      std::array<uint8_t,17> cmd = { ReportId_Capability };

      get(cmd);

      Capability data;

      data.tabletMaxX        = static_cast<uint16_t>(cmd[ 1] << 8 | cmd[ 2]);
      data.tabletMaxY        = static_cast<uint16_t>(cmd[ 3] << 8 | cmd[ 4]);
      data.tabletMaxPressure = static_cast<uint16_t>(cmd[ 5] << 8 | cmd[ 6]);
      data.screenWidth       = static_cast<uint16_t>(cmd[ 7] << 8 | cmd[ 8]);
      data.screenHeight      = static_cast<uint16_t>(cmd[ 9] << 8 | cmd[10]);
      data.maxReportRate     = cmd[11];
      data.resolution        = static_cast<uint16_t>(cmd[12] << 8 | cmd[13]);
      data.encodingFlag      = cmd[14];
      //cmd[15] reserved
      //cmd[16] reserved

      return std::move(data);
    }



    uint32_t Protocol::getUid()
    {
      std::array<uint8_t,5> cmd = { ReportId_Uid };

      get(cmd);

      uint32_t data = static_cast<uint32_t>( (cmd[1] << 24) | (cmd[2] << 16) | (cmd[3] << 8) | cmd[4] );

      return data;
    }



    void Protocol::setUid(uint32_t value)
    {
      std::array<uint8_t,5> cmd =
      {
        ReportId_Uid,
        static_cast<uint8_t>((value>>24) & 0xffu),
        static_cast<uint8_t>((value>>16) & 0xffu),
        static_cast<uint8_t>((value>> 8) & 0xffu),
        static_cast<uint8_t>((value    ) & 0xffu)
      };

      set(cmd);
    }



    Protocol::Uid2 Protocol::getUid2()
    {
      std::array<uint8_t,11> cmd = { ReportId_Uid2 };

      get(cmd);

      Uid2 data;

      data.uid2[ 0]               = static_cast<char>(cmd[ 1]);
      data.uid2[ 1]               = static_cast<char>(cmd[ 2]);
      data.uid2[ 2]               = static_cast<char>(cmd[ 3]);
      data.uid2[ 3]               = static_cast<char>(cmd[ 4]);
      data.uid2[ 4]               = static_cast<char>(cmd[ 5]);
      data.uid2[ 5]               = static_cast<char>(cmd[ 6]);
      data.uid2[ 6]               = static_cast<char>(cmd[ 7]);
      data.uid2[ 7]               = static_cast<char>(cmd[ 8]);
      data.uid2[ 8]               = static_cast<char>(cmd[ 9]);
      data.uid2[ 9]               = static_cast<char>(cmd[10]);
      data.uid2NullTerminated[10] = 0;

      return data;
    }


    Protocol::Eserial Protocol::getEserial()
    {
      std::array<uint8_t, 14> cmd = { ReportId_Eserial };

      get(cmd);

      Eserial data;

      data.eSerial[0] = static_cast<char>(cmd[1]);
      data.eSerial[1] = static_cast<char>(cmd[2]);
      data.eSerial[2] = static_cast<char>(cmd[3]);
      data.eSerial[3] = static_cast<char>(cmd[4]);
      data.eSerial[4] = static_cast<char>(cmd[5]);
      data.eSerial[5] = static_cast<char>(cmd[6]);
      data.eSerial[6] = static_cast<char>(cmd[7]);
      data.eSerial[7] = static_cast<char>(cmd[8]);
      data.eSerial[8] = static_cast<char>(cmd[9]);
      data.eSerial[9] = static_cast<char>(cmd[10]);
      data.eSerial[10] = static_cast<char>(cmd[11]);
      data.eSerial[11] = static_cast<char>(cmd[12]);
      data.eSerial[12] = static_cast<char>(cmd[13]);
      data.eSerialNullTerminated[13] = 0;

      return data;
    }


    void Protocol::setDefaultMode(DefaultMode defaultMode)
    {
      std::array<uint8_t, 3> cmd =
      {
        ReportId_DefaultMode,
        static_cast<uint8_t>(defaultMode)
      };

      set(cmd);
    }


    uint8_t Protocol::getDefaultMode()
    {
      std::array<uint8_t, 3> cmd = { ReportId_DefaultMode };

      get(cmd);

      uint8_t data = cmd[1];

      return data;
    }


    void Protocol::setReportRate(uint8_t reportRate)
    {
      std::array<uint8_t, 2> cmd =
      {
        ReportId_ReportRate,
        reportRate
      };

      set(cmd);
    }


    uint8_t Protocol::getReportRate()
    {
      std::array<uint8_t, 2> cmd = { ReportId_ReportRate };

      get(cmd);

      uint8_t data = cmd[1];

      return data;
    }


    Protocol::PublicKey Protocol::getHostPublicKey()
    {
      std::array<uint8_t,17> cmd = { ReportId_HostPublicKey };

      get(cmd);

      PublicKey data =
      {
        cmd[ 1],
        cmd[ 2],
        cmd[ 3],
        cmd[ 4],
        cmd[ 5],
        cmd[ 6],
        cmd[ 7],
        cmd[ 8],
        cmd[ 9],
        cmd[10],
        cmd[11],
        cmd[12],
        cmd[13],
        cmd[14],
        cmd[15],
        cmd[16]
      };

      return std::move(data);
    }



    void Protocol::setHostPublicKey(PublicKey const & hostPublicKey)
    {
      std::array<uint8_t,17> cmd =
      {
        ReportId_HostPublicKey,
        hostPublicKey[ 0],
        hostPublicKey[ 1],
        hostPublicKey[ 2],
        hostPublicKey[ 3],
        hostPublicKey[ 4],
        hostPublicKey[ 5],
        hostPublicKey[ 6],
        hostPublicKey[ 7],
        hostPublicKey[ 8],
        hostPublicKey[ 9],
        hostPublicKey[10],
        hostPublicKey[11],
        hostPublicKey[12],
        hostPublicKey[13],
        hostPublicKey[14],
        hostPublicKey[15]
      };

      set(cmd);
    }



    Protocol::PublicKey Protocol::getDevicePublicKey()
    {
      std::array<uint8_t,17> cmd = { ReportId_DevicePublicKey };

      get(cmd);

      PublicKey data =
      {
        cmd[ 1],
        cmd[ 2],
        cmd[ 3],
        cmd[ 4],
        cmd[ 5],
        cmd[ 6],
        cmd[ 7],
        cmd[ 8],
        cmd[ 9],
        cmd[10],
        cmd[11],
        cmd[12],
        cmd[13],
        cmd[14],
        cmd[15],
        cmd[16]
      };

      return std::move(data);
    }



    void Protocol::setStartCapture(uint32_t sessionId)
    {
      std::array<uint8_t,5> cmd =
      {
        ReportId_StartCapture,
        static_cast<uint8_t>((sessionId>>24) & 0xffu),
        static_cast<uint8_t>((sessionId>>16) & 0xffu),
        static_cast<uint8_t>((sessionId>> 8) & 0xffu),
        static_cast<uint8_t>((sessionId    ) & 0xffu)
      };

      set(cmd);

      //::boost::this_thread::sleep(::boost::posix_time::millisec(20)); // boost
      this_thread::sleep_for(chrono::milliseconds(20)); // C++11
    }



    void Protocol::setEndCapture()
    {
      std::array<uint8_t,2> cmd = { ReportId_EndCapture };

      set(cmd);

      //::boost::this_thread::sleep(::boost::posix_time::millisec(20)); // boost
      this_thread::sleep_for(chrono::milliseconds(20)); // C++11
    }



    Protocol::DHprime Protocol::getDHprime()
    {
      std::array<uint8_t,17> cmd = { ReportId_DHprime };

      get(cmd);

      Protocol::DHprime data;
      std::copy(cmd.begin()+1, cmd.end(), data.begin());

      return std::move(data);
    }



    void Protocol::setDHprime(DHprime const & prime)
    {
      std::array<uint8_t,17> cmd =
      {
        ReportId_DHprime,
        prime[ 0],
        prime[ 1],
        prime[ 2],
        prime[ 3],
        prime[ 4],
        prime[ 5],
        prime[ 6],
        prime[ 7],
        prime[ 8],
        prime[ 9],
        prime[10],
        prime[11],
        prime[12],
        prime[13],
        prime[14],
        prime[15]
      };

      set(cmd);
    }



    Protocol::DHbase Protocol::getDHbase()
    {
      std::array<uint8_t,3> cmd = { ReportId_DHbase };

      get(cmd);

      Protocol::DHbase data;
      data[ 0] = cmd[ 1];
      data[ 1] = cmd[ 2];

      return std::move(data);
    }



    void Protocol::setDHbase(DHbase const & base)
    {
      std::array<uint8_t,3> cmd =
      {
        ReportId_DHbase,
        base[ 0],
        base[ 1]
      };

      set(cmd);
    }



    void Protocol::setClearScreen()
    {
      std::array<uint8_t,2> cmd = { ReportId_ClearScreen };

      set(cmd);
    }



    void Protocol::setClearScreenArea(Protocol::Rectangle const & area)
    {
      std::array<uint8_t,11> cmd =
      {
        ReportId_ClearScreenArea,
        0,
        1,
        static_cast<uint8_t>( area.upperLeftXpixel      & 0xffu),
        static_cast<uint8_t>((area.upperLeftXpixel >>8) & 0xffu),
        static_cast<uint8_t>( area.upperLeftYpixel      & 0xffu),
        static_cast<uint8_t>((area.upperLeftYpixel >>8) & 0xffu),
        static_cast<uint8_t>( area.lowerRightXpixel     & 0xffu),
        static_cast<uint8_t>((area.lowerRightXpixel>>8) & 0xffu),
        static_cast<uint8_t>( area.lowerRightYpixel     & 0xffu),
        static_cast<uint8_t>((area.lowerRightYpixel>>8) & 0xffu)
      };

      set(cmd);
    }


    uint8_t Protocol::getInkingMode()
    {
      std::array<uint8_t,2> cmd = { ReportId_InkingMode };

      get(cmd);

      return cmd[1];
    }



    void Protocol::setInkingMode(InkingMode inkingMode)
    {
      std::array<uint8_t,2> cmd = { ReportId_InkingMode, inkingMode };

      set(cmd);
    }



    Protocol::InkThreshold Protocol::getInkThreshold()
    {
      std::array<uint8_t,5> cmd = { ReportId_InkThreshold };

      get(cmd);

      InkThreshold data;
      data.onPressureMark  = static_cast<uint16_t>(cmd[ 1] | cmd[ 2] << 8);
      data.offPressureMark = static_cast<uint16_t>(cmd[ 3] | cmd[ 4] << 8);

      return std::move(data);
    }



    void Protocol::setInkThreshold(InkThreshold const & data)
    {
      std::array<uint8_t,5> cmd =
      {
        ReportId_InkThreshold,
        static_cast<uint8_t>((data.onPressureMark      ) & 0xffu),
        static_cast<uint8_t>((data.onPressureMark  >> 8) & 0xffu),
        static_cast<uint8_t>((data.offPressureMark     ) & 0xffu),
        static_cast<uint8_t>((data.offPressureMark >> 8) & 0xffu)
      };

      set(cmd);
    }



    void Protocol::setStartImageData(EncodingMode encodingMode)
    {
      std::array<uint8_t,2> cmd = { ReportId_StartImageData, encodingMode };

      set(cmd);
    }



    void Protocol::setStartImageDataArea(EncodingMode encodingMode, Rectangle const & area)
    {
      std::array<uint8_t,11> cmd =
      {
        ReportId_StartImageDataArea,
        encodingMode,
        1,
        static_cast<uint8_t>((area.upperLeftXpixel      ) & 0xffu),
        static_cast<uint8_t>((area.upperLeftXpixel  >> 8) & 0xffu),
        static_cast<uint8_t>((area.upperLeftYpixel      ) & 0xffu),
        static_cast<uint8_t>((area.upperLeftYpixel  >> 8) & 0xffu),
        static_cast<uint8_t>((area.lowerRightXpixel     ) & 0xffu),
        static_cast<uint8_t>((area.lowerRightXpixel >> 8) & 0xffu),
        static_cast<uint8_t>((area.lowerRightYpixel     ) & 0xffu),
        static_cast<uint8_t>((area.lowerRightYpixel >> 8) & 0xffu)
      };

      set(cmd);
    }


    void Protocol::setImageDataBlock(ImageDataBlock const & data)
    {
      if (data.length > sizeof(data.data))
        throw std::range_error("length is out of range");

      if (data.length > data.maxLengthHID)
      {
        std::array<uint8_t, 2560> cmd = { ReportId_ImageDataBlock };

        cmd[1] = static_cast<uint8_t>((data.length) & 0xffu);
        cmd[2] = static_cast<uint8_t>((data.length >> 8) & 0xffu);
        std::copy_n(data.data.begin(), data.length, cmd.begin() + 3);

        set(cmd);
      }
      else
      {
        std::array<uint8_t, 256> cmd = { ReportId_ImageDataBlock };

        cmd[1] = static_cast<uint8_t>((data.length) & 0xffu);
        cmd[2] = static_cast<uint8_t>((data.length >> 8) & 0xffu);
        std::copy_n(data.data.begin(), data.length, cmd.begin() + 3);

        set(cmd);
      }
    }



    void Protocol::setEndImageData(EndImageDataFlag endImageDataFlag)
    {
      std::array<uint8_t,2> cmd = { ReportId_EndImageData, endImageDataFlag };

      set(cmd);
    }



    Protocol::HandwritingThicknessColor Protocol::getHandwritingThicknessColor()
    {
      std::array<uint8_t,4> cmd = { ReportId_HandwritingThicknessColor };

      get(cmd);

      HandwritingThicknessColor data;

      data.penColor     = static_cast<uint16_t>(cmd[1] | cmd[2] << 8);
      data.penThickness = cmd[3];

      return std::move(data);
    }



    void Protocol::setHandwritingThicknessColor(HandwritingThicknessColor const & handwritingThicknessColor)
    {
      std::array<uint8_t,4> cmd =
      {
        ReportId_HandwritingThicknessColor,
        static_cast<uint8_t>( handwritingThicknessColor.penColor     & 0xffu),
        static_cast<uint8_t>((handwritingThicknessColor.penColor>>8) & 0xffu),
        handwritingThicknessColor.penThickness
      };

      set(cmd);
    }




    Protocol::HandwritingThicknessColor24 Protocol::getHandwritingThicknessColor24()
    {
      std::array<uint8_t,5> cmd = { ReportId_HandwritingThicknessColor24 };

      get(cmd);

      HandwritingThicknessColor24 data;

      data.penColor     = static_cast<uint32_t>(cmd[1]) | (static_cast<uint32_t>(cmd[2])<<8) | static_cast<uint32_t>((cmd[3])<<16);
      data.penThickness = cmd[4];

      return std::move(data);
    }



    void Protocol::setHandwritingThicknessColor24(HandwritingThicknessColor24 const & handwritingThicknessColor)
    {
      std::array<uint8_t,5> cmd =
      {
        ReportId_HandwritingThicknessColor24,
        static_cast<uint8_t>( handwritingThicknessColor.penColor      & 0xffu),
        static_cast<uint8_t>((handwritingThicknessColor.penColor>>8)  & 0xffu),
        static_cast<uint8_t>((handwritingThicknessColor.penColor>>16) & 0xffu),
        handwritingThicknessColor.penThickness
      };

      set(cmd);
    }




    uint16_t Protocol::getBackgroundColor()
    {
      std::array<uint8_t,3> cmd = { ReportId_BackgroundColor };

      get(cmd);

      uint16_t data = static_cast<uint16_t>(cmd[1] | cmd[2] << 8);

      return data;
    }



    void Protocol::setBackgroundColor(uint16_t backgroundColor)
    {
      std::array<uint8_t,3> cmd =
      {
        ReportId_BackgroundColor,
        static_cast<uint8_t>( backgroundColor     & 0xffu),
        static_cast<uint8_t>((backgroundColor>>8) & 0xffu)
      };

      set(cmd);
    }




    uint32_t Protocol::getBackgroundColor24()
    {
      std::array<uint8_t,4> cmd = { ReportId_BackgroundColor24 };

      get(cmd);

      uint32_t data = static_cast<uint32_t>(cmd[1]) | (static_cast<uint32_t>(cmd[2]) << 8) | (static_cast<uint32_t>(cmd[3]) << 16);

      return data;
    }



    void Protocol::setBackgroundColor24(uint32_t backgroundColor)
    {
      std::array<uint8_t,4> cmd =
      {
        ReportId_BackgroundColor24,
        static_cast<uint8_t>( backgroundColor      & 0xffu),
        static_cast<uint8_t>((backgroundColor>>8)  & 0xffu),
        static_cast<uint8_t>((backgroundColor>>16) & 0xffu)
      };

      set(cmd);
    }



    Protocol::Rectangle Protocol::getHandwritingDisplayArea()
    {
      std::array<uint8_t,9> cmd = { ReportId_HandwritingDisplayArea };

      get(cmd);

      Rectangle data;

      data.upperLeftXpixel  = static_cast<uint16_t>(cmd[1] | cmd[2] << 8);
      data.upperLeftYpixel  = static_cast<uint16_t>(cmd[3] | cmd[4] << 8);
      data.lowerRightXpixel = static_cast<uint16_t>(cmd[5] | cmd[6] << 8);
      data.lowerRightYpixel = static_cast<uint16_t>(cmd[7] | cmd[8] << 8);

      return std::move(data);
    }



    void Protocol::setHandwritingDisplayArea(Rectangle const & handwritingDisplayArea)
    {
      std::array<uint8_t,9> cmd =
      {
        ReportId_HandwritingDisplayArea,
        static_cast<uint8_t>( handwritingDisplayArea.upperLeftXpixel      & 0xffu),
        static_cast<uint8_t>((handwritingDisplayArea.upperLeftXpixel >>8) & 0xffu),
        static_cast<uint8_t>( handwritingDisplayArea.upperLeftYpixel      & 0xffu),
        static_cast<uint8_t>((handwritingDisplayArea.upperLeftYpixel >>8) & 0xffu),
        static_cast<uint8_t>( handwritingDisplayArea.lowerRightXpixel     & 0xffu),
        static_cast<uint8_t>((handwritingDisplayArea.lowerRightXpixel>>8) & 0xffu),
        static_cast<uint8_t>( handwritingDisplayArea.lowerRightYpixel     & 0xffu),
        static_cast<uint8_t>((handwritingDisplayArea.lowerRightYpixel>>8) & 0xffu)
      };

      set(cmd);
    }



    uint16_t Protocol::getBacklightBrightness()
    {
      std::array<uint8_t,3> cmd = { ReportId_BacklightBrightness };

      get(cmd);

      uint16_t data = static_cast<uint16_t>(cmd[1] | cmd[2]<<8);

      return data;
    }



    void Protocol::setBacklightBrightness(uint16_t backlightBrightness)
    {
      std::array<uint8_t,3> cmd =
      {
        ReportId_BacklightBrightness,
        static_cast<uint8_t>( backlightBrightness     & 0xffu),
        static_cast<uint8_t>((backlightBrightness>>8) & 0xffu)
      };

      set(cmd);
    }



    uint16_t Protocol::getScreenContrast()
    {
      std::array<uint8_t,3> cmd = { ReportId_ScreenContrast };

      get(cmd);

      uint16_t data = static_cast<uint16_t>(cmd[1] | cmd[2]<<8);

      return data;
    }



    void Protocol::setScreenContrast(uint16_t screenContrast)
    {
      std::array<uint8_t,3> cmd =
      {
        ReportId_ScreenContrast,
        static_cast<uint8_t>( screenContrast     & 0xffu),
        static_cast<uint8_t>((screenContrast>>8) & 0xffu)
      };

      set(cmd);
    }


    void Protocol::setBootScreen(BootScreenFlag bootScreenFlag)
    {
      std::array<uint8_t, 2> cmd =
      {
        ReportId_BootScreen,
        static_cast<uint8_t>(bootScreenFlag)
      };

      set(cmd);
    }

    uint8_t Protocol::getBootScreen()
    {
      std::array<uint8_t, 2> cmd = { ReportId_BootScreen };

      get(cmd);

      return cmd[1];
    }


    uint8_t Protocol::getPenDataOptionMode()
    {
      std::array<uint8_t,2> cmd = { ReportId_PenDataOptionMode };

      get(cmd);

      return cmd[1];
    }



    void Protocol::setPenDataOptionMode(PenDataOptionMode penDataOptionMode)
    {
      std::array<uint8_t,2> cmd = { ReportId_PenDataOptionMode, penDataOptionMode };

      set(cmd);
    }



    Protocol::EncryptionStatus Protocol::getEncryptionStatus()
    {
      std::array<uint8_t,17> cmd = { ReportId_EncryptionStatus };

      get(cmd);

      EncryptionStatus data;

      data.symmetricKeyType      = cmd[1];
      data.asymmetricPaddingType = static_cast<uint8_t>((cmd[2] >> 6) & 0x03);
      data.asymmetricKeyType     = static_cast<uint8_t>(cmd[2] & 0x3F);
      // cmd[3] reserved
      data.statusCodeRSAe = cmd[4];
      data.statusCodeRSAn = cmd[5];
      data.statusCodeRSAc = cmd[6];
      data.lastResultCode = cmd[7];
      data.rng            = (cmd[8] & 0x4) != 0;
      data.sha1           = (cmd[8] & 0x2) != 0;
      data.aes            = (cmd[8] & 0x1) != 0;

      return std::move(data);
    }



    Protocol::EncryptionCommand Protocol::EncryptionCommand::initializeSetEncryptionType(Protocol::SymmetricKeyType symmetricKeyType, Protocol::AsymmetricPaddingType asymmetricPaddingType, Protocol::AsymmetricKeyType asymmetricKeyType)
    {
      EncryptionCommand encryptionCommand =
      {
        EncryptionCommandNumber_SetEncryptionType,
        symmetricKeyType,
        static_cast<uint8_t>((((asymmetricPaddingType & 0x3) << 6) | (asymmetricKeyType & 0x3f)) & 0xffu)
      };

      return std::move(encryptionCommand);
    }



    Protocol::EncryptionCommand Protocol::EncryptionCommand::initializeSetParameterBlock(EncryptionCommandParameterBlockIndex index, uint8_t length, uint8_t const * data)
    {
      EncryptionCommand encryptionCommand =
      {
        EncryptionCommandNumber_SetParameterBlock,
        index,
        length
      };
      if (data)
      {
        std::copy_n(data, length, encryptionCommand.data.begin());
      }

      return std::move(encryptionCommand);
    }



    Protocol::EncryptionCommand Protocol::EncryptionCommand::initializeGenerateSymmetricKey()
    {
      EncryptionCommand encryptionCommand =
      {
        EncryptionCommandNumber_GenerateSymmetricKey
      };

      return std::move(encryptionCommand);
    }


    Protocol::EncryptionCommand Protocol::EncryptionCommand::initializeGetParameterBlock(EncryptionCommandParameterBlockIndex index, uint8_t offset)
    {
      EncryptionCommand encryptionCommand =
      {
        EncryptionCommandNumber_GetParameterBlock,
        index,
        offset
      };

      return std::move(encryptionCommand);
    }


#if 0
    void Protocol::getEncryptionCommand(EncryptionCommand & data)
    {
      std::array<uint8_t,68> cmd = { ReportId_EncryptionCommand };

      cmd[1] = data.encryptionCommandNumber;
      cmd[2] = data.parameter;
      cmd[3] = data.lengthOrIndex;
      std::copy_n(data.data, 64, cmd.begin()+4);

      get(cmd);

      data.encryptionCommandNumber = static_cast<EncryptionCommandNumber>(cmd[1]);
      data.parameter               = cmd[2];
      data.lengthOrIndex           = cmd[3];
      std::copy(cmd.begin()+4, cmd.end(), data.data);
    }
#else
    Protocol::EncryptionCommand Protocol::getEncryptionCommand(Protocol::EncryptionCommandNumber encryptionCommandNumber)
    {
      std::array<uint8_t,68> cmd = { ReportId_EncryptionCommand, encryptionCommandNumber };

      get(cmd);

      EncryptionCommand data;

      data.encryptionCommandNumber = static_cast<EncryptionCommandNumber>(cmd[1]);
      data.parameter               = cmd[2];
      data.lengthOrIndex           = cmd[3];

      std::copy(cmd.begin()+4, cmd.end(), data.data.begin());

      return std::move(data);
    }
#endif



    void Protocol::setEncryptionCommand(EncryptionCommand const & data)
    {
      std::array<uint8_t,68> cmd = { ReportId_EncryptionCommand };

      cmd[1] = data.encryptionCommandNumber;
      cmd[2] = data.parameter;
      cmd[3] = data.lengthOrIndex;
      std::copy(data.data.begin(), data.data.end(), cmd.begin()+4);

      set(cmd);
    }



    Protocol::ReportSizeCollection Protocol::getReportSizeCollection()
    {
      std::array<uint8_t, 512> cmd = { ReportId_ReportSizeCollection };

      get(cmd);

      ReportSizeCollection data;

      data[0] = 0;
      for (unsigned int i = 1; i <= 255; ++i)
      {
        data[i] = static_cast<uint16_t>((cmd[0u + i*2] << 8) | cmd[1u + i*2]);
      }

      return std::move(data);
    }



    Protocol::OperationMode Protocol::OperationMode::initializeNormal()
    {
      OperationMode operationMode { OperationModeType_Normal };

      return operationMode;
    }



    Protocol::OperationMode Protocol::OperationMode::initializePinPad(OperationMode_PinPad const & pinPad)
    {
      OperationMode operationMode { OperationModeType_PinPad };

      operationMode.data[0] = pinPad.screenSelect;
      operationMode.data[1] = pinPad.pinBypass;
      operationMode.data[2] = pinPad.minDigit;
      operationMode.data[3] = pinPad.maxDigit;
      operationMode.data[4] = pinPad.numberHiddenMode;
      operationMode.data[5] = pinPad.idAfterEnter;
      operationMode.data[6] = pinPad.idAfterCancel;
      //operationMode.data[7] = 0; // reserved
      //operationMode.data[8] = 0; // reserved
      //operationMode.data[9] = 0; // reserved
      //operationMode.data[10] = 0; // reserved

      return operationMode;
    }



    Protocol::OperationMode Protocol::OperationMode::initializeSlideShow(OperationMode_SlideShow const & slideShow)
    {
      OperationMode operationMode{ OperationModeType_SlideShow };

      operationMode.data[0] = slideShow.workingMode;
      operationMode.data[1] = slideShow.numberOfSlides;
      operationMode.data[2] = ((slideShow.slideNumber[0] & 0x0fu) << 4) | (slideShow.slideNumber[1] & 0x0fu);
      operationMode.data[3] = ((slideShow.slideNumber[2] & 0x0fu) << 4) | (slideShow.slideNumber[3] & 0x0fu);
      operationMode.data[4] = ((slideShow.slideNumber[4] & 0x0fu) << 4) | (slideShow.slideNumber[5] & 0x0fu);
      operationMode.data[5] = ((slideShow.slideNumber[6] & 0x0fu) << 4) | (slideShow.slideNumber[7] & 0x0fu);
      operationMode.data[6] = ((slideShow.slideNumber[8] & 0x0fu) << 4) | (slideShow.slideNumber[9] & 0x0fu);
      operationMode.data[7] = ((slideShow.interval >> 24) & 0xffu);
      operationMode.data[8] = ((slideShow.interval >> 16) & 0xffu);
      operationMode.data[9] = ((slideShow.interval >> 8) & 0xffu);
      operationMode.data[10] = ((slideShow.interval) & 0xffu);

      return operationMode;
    }



    Protocol::OperationMode Protocol::OperationMode::initializeKeyPad(OperationMode_KeyPad const & keyPad)
    {
      OperationMode operationMode { OperationModeType_KeyPad };
      operationMode.data[0] = keyPad.screenSelect;
      operationMode.data[1] = keyPad.idAfterSelect;
      //operationMode.data[2] = 0; // reserved
      //operationMode.data[3] = 0; // reserved
      //operationMode.data[4] = 0; // reserved
      //operationMode.data[5] = 0; // reserved
      //operationMode.data[6] = 0; // reserved
      //operationMode.data[7] = 0; // reserved
      //operationMode.data[8] = 0; // reserved
      //operationMode.data[9] = 0; // reserved
      //operationMode.data[10] = 0; // reserved
      return operationMode;

    }


    Protocol::OperationMode Protocol::OperationMode::initializeSignature(OperationMode_Signature const & signature)
    {
      OperationMode operationMode{ OperationModeType_Signature };
      operationMode.data[0] = signature.signatureScreen;
      operationMode.data[1] = signature.keyDefinition[0];
      operationMode.data[2] = signature.keyDefinition[1];
      operationMode.data[3] = signature.keyDefinition[2];
      operationMode.data[4] = signature.idAfterEnter;
      operationMode.data[5] = signature.idAfterCancel;
      //operationMode.data[6] = 0; // reserved
      //operationMode.data[7] = 0; // reserved
      //operationMode.data[8] = 0; // reserved
      //operationMode.data[9] = 0; // reserved
      //operationMode.data[10] = 0; // reserved
      return operationMode;
    }



    void Protocol::setOperationMode(OperationMode const & data)
    {
      std::array<uint8_t, 13> cmd = { ReportId_OperationMode };

      cmd[1] = data.operationMode;
      std::copy(data.data.begin(), data.data.end(), cmd.begin() + 2);

      set(cmd);
    }



    Protocol::OperationMode Protocol::getOperationMode()
    {
      std::array<uint8_t, 13> cmd = { ReportId_OperationMode };

      get(cmd);

      OperationMode data;
      data.operationMode = cmd[1];
      std::copy(cmd.begin()+2, cmd.end(), data.data.begin());
      return data;
    }



    void Protocol::setRomStartImageData(RomStartImageData const & romStartImage)
    {
      std::array<uint8_t, 7> cmd = { ReportId_RomStartImageData };

      cmd[1] = romStartImage.encodingMode;
      cmd[2] = romStartImage.operationMode;
      cmd[3] = static_cast<uint8_t>((romStartImage.imageType ? 0x80u : 0) | (romStartImage.imageNumber & 0x7f));
      cmd[4] = romStartImage.data[0];
      cmd[5] = romStartImage.data[1];
      cmd[6] = romStartImage.data[2];

      set(cmd);
    }



    void Protocol::setRomImageHash(OperationModeType operationModeType, bool imageType, uint8_t imageNumber)
    {
      std::array<uint8_t, 20> cmd = { ReportId_RomImageHash };

      cmd[1] = static_cast<uint8_t>(operationModeType);
      cmd[2] = static_cast<uint8_t>((imageType ? 0x80u : 0) | (imageNumber & 0x7f));
      //cmd3~cmd7 reserved

      set(cmd);
    }



    Protocol::RomImageHash Protocol::getRomImageHash()
    {
      std::array<uint8_t, 20> cmd = { ReportId_RomImageHash };

      get(cmd);

      RomImageHash data;

      data.operationMode = cmd[1];
      data.imageType     = (cmd[2] & 0x80) != 0;
      data.imageNumber   = static_cast<uint8_t>(cmd[2] & 0x7fu);
      data.result        = cmd[3];
      std::copy(cmd.begin() + 4, cmd.end(), data.hash.begin());

      return data;
    }



    void Protocol::setRomImageDelete(RomImageDeleteMode romDeleteImageMode, bool imageType, uint8_t imageNumber)
    {
      std::array<uint8_t, 3> cmd = { ReportId_RomImageDelete };

      cmd[1] = static_cast<uint8_t>(romDeleteImageMode);
      cmd[2] = static_cast<uint8_t>((imageType ? 0x80u : 0) | (imageNumber & 0x7fu));

      set(cmd);
    }



    Protocol::Rectangle Protocol::getCurrentImageArea()
    {
      std::array<uint8_t, 9> cmd = { ReportId_CurrentImageArea };

      get(cmd);

      Rectangle data;

      data.upperLeftXpixel = static_cast<uint16_t>(cmd[1] | cmd[2] << 8);
      data.upperLeftYpixel = static_cast<uint16_t>(cmd[3] | cmd[4] << 8);
      data.lowerRightXpixel = static_cast<uint16_t>(cmd[5] | cmd[6] << 8);
      data.lowerRightYpixel = static_cast<uint16_t>(cmd[7] | cmd[8] << 8);

      return std::move(data);
    }



    void Protocol::setRomImageDisplay(OperationModeType operationModeType, bool imageType, uint8_t imageNumber)
    {
      std::array<uint8_t, 3> cmd = { ReportId_RomImageDisplay };

      cmd[1] = operationModeType;
      cmd[2] = static_cast<uint8_t>((imageType ? 0x80u : 0) | (imageNumber & 0x7fu));

      set(cmd);
    }



    void Protocol::setRenderingMode(RenderingMode renderingMode)
    {
      std::array<uint8_t, 2> cmd = { ReportId_RenderingMode };

      cmd[1] = static_cast<uint8_t>(renderingMode);

      set(cmd);
    }


    uint8_t Protocol::getRenderingMode()
    {
      std::array<uint8_t, 2> cmd = { ReportId_RenderingMode };

      get(cmd);

      uint8_t data = cmd[1];

      return data;
    }





    Protocol::EventDataPinPad Protocol::EventDataPinPad::initialize(EventData const & eventData)
    {
      EventDataPinPad data;

      if (eventData.operationMode != data.operationModeType)
        throw std::bad_cast();

      data.keyInput = eventData.data[0];
      data.pin[0] = (eventData.data[1] >> 4) & 0xfu;
      data.pin[1] = (eventData.data[1]     ) & 0xfu;
      data.pin[2] = (eventData.data[2] >> 4) & 0xfu;
      data.pin[3] = (eventData.data[2]     ) & 0xfu;
      data.pin[4] = (eventData.data[3] >> 4) & 0xfu;
      data.pin[5] = (eventData.data[3]     ) & 0xfu;
      data.pin[6] = (eventData.data[4] >> 4) & 0xfu;
      data.pin[7] = (eventData.data[4]     ) & 0xfu;
      data.pin[8] = (eventData.data[5] >> 4) & 0xfu;
      data.pin[9] = (eventData.data[5]     ) & 0xfu;
      data.pin[10] = (eventData.data[6] >> 4) & 0xfu;
      data.pin[11] = (eventData.data[6]     ) & 0xfu;
      data.pin[12] = (eventData.data[7] >> 4) & 0xfu;
      // (eventData.data[7]) & 0xfu reserved

      return data;
    }



    Protocol::EventDataKeyPad Protocol::EventDataKeyPad::initialize(EventData const & eventData)
    {
      EventDataKeyPad data;

      if (eventData.operationMode != data.operationModeType)
        throw std::bad_cast();

      data.screenSelected = eventData.data[0];
      data.keyNumber = eventData.data[1];

      return data;
    }


    Protocol::EventDataSignature Protocol::EventDataSignature::initialize(EventData const & eventData)
    {
      EventDataSignature data;

      if (eventData.operationMode != data.operationModeType)
        throw std::bad_cast();

      data.keyValue = eventData.data[0];

      return data;
    }


    Protocol::EventDataPinPadEncrypted Protocol::EventDataPinPadEncrypted::initialize(EventDataEncrypted const & eventData)
    {
      EventDataPinPadEncrypted data;

      if (eventData.operationMode != data.operationModeType)
        throw std::bad_cast();

      data.sessionId = eventData.sessionId;
      data.keyInput = eventData.data[0];
      data.pin[0] = (eventData.data[1] >> 4) & 0xfu;
      data.pin[1] = (eventData.data[1]) & 0xfu;
      data.pin[2] = (eventData.data[2] >> 4) & 0xfu;
      data.pin[3] = (eventData.data[2]) & 0xfu;
      data.pin[4] = (eventData.data[3] >> 4) & 0xfu;
      data.pin[5] = (eventData.data[3]) & 0xfu;
      data.pin[6] = (eventData.data[4] >> 4) & 0xfu;
      data.pin[7] = (eventData.data[4]) & 0xfu;
      data.pin[8] = (eventData.data[5] >> 4) & 0xfu;
      data.pin[9] = (eventData.data[5]) & 0xfu;
      data.pin[10] = (eventData.data[6] >> 4) & 0xfu;
      data.pin[11] = (eventData.data[6]) & 0xfu;
      data.pin[12] = (eventData.data[7] >> 4) & 0xfu;
      // (eventData.data[7]) & 0xfu reserved

      return data;
    }




    Protocol::EventDataKeyPadEncrypted Protocol::EventDataKeyPadEncrypted::initialize(EventDataEncrypted const & eventData)
    {
      EventDataKeyPadEncrypted data;

      if (eventData.operationMode != data.operationModeType)
        throw std::bad_cast();

      data.sessionId = eventData.sessionId;
      data.screenSelected = eventData.data[0];
      data.keyNumber = eventData.data[1];

      return data;
    }


    Protocol::EventDataSignatureEncrypted Protocol::EventDataSignatureEncrypted::initialize(EventDataEncrypted const & eventData)
    {
      EventDataSignatureEncrypted data;

      if (eventData.operationMode != data.operationModeType)
        throw std::bad_cast();

      data.sessionId = eventData.sessionId;
      data.keyValue = eventData.data[0];

      return data;
    }



    Protocol::RomStartImageData Protocol::RomStartImageData::initializePinPad(EncodingMode encodingMode, bool imageType, uint8_t imageNumber, uint8_t padType, uint8_t keyLayout)
    {
      RomStartImageData data{ encodingMode, OperationModeType_PinPad, imageType, imageNumber };
      data.data[0] = padType;
      data.data[1] = keyLayout;

      return data;
    }



    Protocol::RomStartImageData Protocol::RomStartImageData::initializeSlideShow(EncodingMode encodingMode, bool imageType, uint8_t imageNumber)
    {
      RomStartImageData data{ encodingMode, OperationModeType_SlideShow, imageType, imageNumber };
      return data;
    }



    Protocol::RomStartImageData Protocol::RomStartImageData::initializeKeyPad(EncodingMode encodingMode, bool imageType, uint8_t imageNumber, uint8_t padType, std::array<bool, 9> keyEnabled)
    {
      RomStartImageData data{ encodingMode, OperationModeType_KeyPad, imageType, imageNumber };

      data.data[0] = padType;
      data.data[1] = keyEnabled[8] ? 0x1u : 0u;
      data.data[2] = static_cast<uint8_t>(
        (keyEnabled[7] ? 0x80u : 0) |
        (keyEnabled[6] ? 0x40u : 0) |
        (keyEnabled[5] ? 0x20u : 0) |
        (keyEnabled[4] ? 0x10u : 0) |
        (keyEnabled[3] ? 0x08u : 0) |
        (keyEnabled[2] ? 0x04u : 0) |
        (keyEnabled[1] ? 0x02u : 0) |
        (keyEnabled[0] ? 0x01u : 0));

      return data;
    }


    Protocol::RomStartImageData Protocol::RomStartImageData::initializeSignature(EncodingMode encodingMode, bool imageType, uint8_t imageNumber, std::array<bool, 3> keyEnabled)
    {
      RomStartImageData data{ encodingMode, OperationModeType_Signature, imageType, imageNumber };

      data.data[0] = static_cast<uint8_t>(
        (keyEnabled[2] ? 0x04u : 0) |
        (keyEnabled[1] ? 0x02u : 0) |
        (keyEnabled[0] ? 0x01u : 0));

      return data;
    }


    Protocol::RomStartImageData Protocol::RomStartImageData::initializeMessageBox(EncodingMode encodingMode, bool imageType, uint8_t imageNumber)
    {
      RomStartImageData data{ encodingMode, OperationModeType_MessageBox, imageType, imageNumber };

      return data;
    }






  } // namespace STU

} // namespace WacomGSS
