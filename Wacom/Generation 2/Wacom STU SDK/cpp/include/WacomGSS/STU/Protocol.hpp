/// @file      WacomGSS/STU/Protocol.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Definition of the WacomGSS::STU::Protocol class

#ifndef WacomGSS_STU_Protocol_hpp
#define WacomGSS_STU_Protocol_hpp

#include <WacomGSS/STU/Interface.hpp>


namespace WacomGSS
{
  namespace STU
  {

    /// @brief A very limited iterator that performs a bounds check on access.
    template<typename InputIterator>
    class CheckedInputIteratorReference
    {
      InputIterator &       m_iter;
      InputIterator const & m_end;
    public:
      typedef typename InputIterator::value_type value_type;
      typedef typename InputIterator::reference  reference;

      CheckedInputIteratorReference(InputIterator & begin, InputIterator const & end)
      :
        m_iter(begin),
        m_end(end)
      {
      }

      reference operator * () const
      {
        if (m_iter != m_end)
          return *m_iter;

        throw std::range_error("The InputIterator did not have enough elements");
      }

      CheckedInputIteratorReference & operator ++ ()
      {
        ++m_iter;
        return *this;
      }
    };



    /// @brief Abstracts the documented %STU tablet protocol.
    ///
    /// This class provides an abstraction to send and receive
    /// data packets to and from the tablet using the Interface class.
    class Protocol
    {
      Interface * m_interface; // will never be null

      template<size_t N>
      void get(std::array<uint8_t,N> & data)
      {
        m_interface->get(data);
      }


      template<size_t N>
      void set(std::array<uint8_t,N> const & data)
      {
        m_interface->set(data);
      }

      template<typename ForwardIterator, class InputReport>
      static ForwardIterator decodeReport(ForwardIterator begin, ForwardIterator end, InputReport & data, std::forward_iterator_tag)
      {
        auto d = std::distance(begin, end);
        if (d >= 0 && static_cast<size_t>(d) < InputReport::reportSize)
          throw std::range_error("decodeReport(): the ForwardIterator did not have enough elements to decode the requested InputReport");

        return decodeReport_unsafe(begin, data);
      }


      template<typename InputIterator, class InputReport>
      static InputIterator decodeReport(InputIterator begin, InputIterator end, InputReport & data, std::input_iterator_tag)
      {
        CheckedInputIteratorReference<InputIterator> iter(begin, end);
        decodeReport_unsafe(iter, data);
        return begin;
      }

    public:
      /// @brief Constructs the object.
      ///
      /// @param intf A reference to the Interface to use.
      Protocol(Interface & intf) noexcept
      :
        m_interface(&intf)
      {
      }


      /// @brief Rebinds the object to a different Interface.
      ///
      /// @param intf A reference to the Interface to use.
      Protocol & rebind(Interface & intf) noexcept
      {
        m_interface = &intf;
        return *this;
      }


      /// @brief Provides access to the stored Interface reference.
      Interface * operator -> () const noexcept
      {
        return m_interface;
      }


      /// @brief Report Identifier values.
      enum ReportId : uint8_t
      {
        ReportId_PenData                           = 0x01, ///< <b>Mode:</b> in         <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_Status                            = 0x03, ///< <b>Mode:</b>    get     <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_Reset                             = 0x04, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
      //ReportId_05                                = 0x05, //   <b>Mode:</b> -internal- <b>Compatibility:</b> 300fw2
        ReportId_HidInformation                    = 0x06, ///< <b>Mode:</b>    get     <b>Compatibility:</b>  -   -   -   430V -  540
        ReportId_Information                       = 0x08, ///< <b>Mode:</b>    get     <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_Capability                        = 0x09, ///< <b>Mode:</b>    get     <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_Uid                               = 0x0a, ///< <b>Mode:</b>    get/set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_Uid2                              = 0x0b, ///< <b>Mode:</b>    get     <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_DefaultMode                       = 0x0c, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -   430V    540
        ReportId_ReportRate                        = 0x0d, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -   430V    540
        ReportId_RenderingMode                     = 0x0e, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_Eserial                           = 0x0f, ///< <b>Mode:</b>    get     <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_PenDataEncrypted                  = 0x10, ///< <b>Mode:</b> in         <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_HostPublicKey                     = 0x13, ///< <b>Mode:</b>    get/set <b>Compatibility:</b> 300 500 520A  -   -   -
        ReportId_DevicePublicKey                   = 0x14, ///< <b>Mode:</b> in/get     <b>Compatibility:</b> 300 500 520A  -   -   -
        ReportId_StartCapture                      = 0x15, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_EndCapture                        = 0x16, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_DHprime                           = 0x1a, ///< <b>Mode:</b>    get/set <b>Compatibility:</b> 300 500 520A  -   -   -
        ReportId_DHbase                            = 0x1b, ///< <b>Mode:</b>    get/set <b>Compatibility:</b> 300 500 520A  -   -   -
        ReportId_ClearScreen                       = 0x20, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_InkingMode                        = 0x21, ///< <b>Mode:</b>    get/set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_InkThreshold                      = 0x22, ///< <b>Mode:</b>    get/set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_ClearScreenArea                   = 0x23, ///< <b>Mode:</b>        set <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_StartImageDataArea                = 0x24, ///< <b>Mode:</b>        set <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_StartImageData                    = 0x25, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_ImageDataBlock                    = 0x26, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_EndImageData                      = 0x27, ///< <b>Mode:</b>        set <b>Compatibility:</b> 300 500 520A 430 530 540
        ReportId_HandwritingThicknessColor         = 0x28, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -  520A 430 530 540  *430=thickness only
        ReportId_BackgroundColor                   = 0x29, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -  520A  -  530 540
        ReportId_HandwritingDisplayArea            = 0x2a, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -  520A 430 530 540
        ReportId_BacklightBrightness               = 0x2b, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -  520A  -  530 540
        ReportId_ScreenContrast                    = 0x2c, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -  520A  -  530 540
        ReportId_HandwritingThicknessColor24       = 0x2d, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -   430 530 540  *430=thickness only
        ReportId_BackgroundColor24                 = 0x2e, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -    -  530 540
        ReportId_BootScreen                        = 0x2f, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_PenDataOption                     = 0x30, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   *  520A  -   -   -   *500=fw2.03 only
        ReportId_PenDataEncryptedOption            = 0x31, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   *  520A  -   -   -   *500=fw2.03 only
        ReportId_PenDataOptionMode                 = 0x32, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   *  520A 430 530 540  *500=fw2.03 only
        ReportId_PenDataTimeCountSequenceEncrypted = 0x33, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_PenDataTimeCountSequence          = 0x34, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_EncryptionCommand                 = 0x40, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_EncryptionStatus                  = 0x50, ///< <b>Mode:</b> in/get     <b>Compatibility:</b>  -   -   -   430 530 540
      //ReportId_60                                = 0x60, //   <b>Mode:</b> -internal- <b>Compatibility:</b>  -   -   -   430 530 540
        ReportId_GetReport                         = 0x80, ///< <b>Mode:</b>        set <b>Compatibility:</b> SERIAL ONLY
        ReportId_SetResult                         = 0x81, ///< <b>Mode:</b> in         <b>Compatibility:</b> SERIAL ONLY
        ReportId_PinPadData                        = 0x90, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_PinPadDataEncrypted               = 0x91, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_PinOperationMode                  = 0x92, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_OperationMode                     = 0x93, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_RomStartImageData                 = 0x94, ///< <b>Mode:</b>        set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_RomImageOccupancy                 = 0x95, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_RomImageHash                      = 0x96, ///< <b>Mode:</b>    get/set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_RomImageDelete                    = 0x97, ///< <b>Mode:</b>        set <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_CurrentImageArea                  = 0x98, ///< <b>Mode:</b>    get     <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_EventData                         = 0x99, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_EventDataEncrypted                = 0x9a, ///< <b>Mode:</b> in         <b>Compatibility:</b>  -   -   -    -   -  540
        ReportId_RomImageDisplay                   = 0x9b, ///< <b>Mode:</b>        set <b>Compatibility:</b>  -   -   -    -   -  540
     //,ReportId_a0                                = 0xa0, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a2                                = 0xa2, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a3                                = 0xa3, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a5                                = 0xa5, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a6                                = 0xa6, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a7                                = 0xa7, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a8                                = 0xa8, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_a9                                = 0xa9, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_aa                                = 0xaa, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_ab                                = 0xab, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_ac                                = 0xac, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_ad                                = 0xad, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_ae                                = 0xae, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_af                                = 0xaf, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_b0                                = 0xb0, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_b2                                = 0xb2, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_b3                                = 0xb3, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
      //ReportId_b4                                = 0xb4, //   <b>Mode:</b> -internal- <b>Compatibility:</b>
        ReportId_ReportSizeCollection              = 0xff  ///< <b>Mode:</b>    get     <b>Compatibility:</b>  -   -   -   430V -   -
      };

      /// @brief Standard definition of a rectangle for the device
      struct Rectangle
      {
        uint16_t upperLeftXpixel;  ///< pixel coordinate of left of rectangle
        uint16_t upperLeftYpixel;  ///< pixel coordinate of top of rectangle
        uint16_t lowerRightXpixel; ///< pixel coordinate of right of rectangle
        uint16_t lowerRightYpixel; ///< pixel coordinate of bottom of rectangle

        Rectangle() noexcept // =default
        {
        }

        Rectangle(uint16_t upperLeftX, uint16_t upperLeftY, uint16_t lowerRightX, uint16_t lowerRightY) noexcept
        :
          upperLeftXpixel(upperLeftX),
          upperLeftYpixel(upperLeftY),
          lowerRightXpixel(lowerRightX),
          lowerRightYpixel(lowerRightY)
        {
        }
      };


      /// @see Status::statusCode
      enum StatusCode : uint8_t
      {
        StatusCode_Ready          = 0x00, ///< Normal state; tablet is transferring pen coordinates. Ready to receive commands.
        StatusCode_Image          = 0x01, ///< %Tablet switches to this after ReportId_StartImageData.
        StatusCode_Capture        = 0x02, ///< %Tablet switches to this after ReportId_StartCapture; tablet is transferring encrypted pen coordinates. Ready to receive commands.
        StatusCode_Calculation    = 0x03, ///< %Tablet is calculating encryption keys.
        StatusCode_Image_Boot     = 0x04, ///< %Tablet is displaying the boot image. The tablet will automatically change to Ready when finished.
        StatusCode_RomBusy        = 0x05, ///< %Tablet is accessing the ROM.
        StatusCode_SystemReset    = 0xff  ///< %Tablet is resetting. So, any function isn't available.
      };

      /// @brief Values returned in Protocol::Status::lastResultCode or a ReportId::ReportId_SetResult report.
      enum ErrorCode : uint8_t
      {
        ErrorCode_None                        = 0x00,   ///< Operation completed successfully.
        ErrorCode_WrongReportId               = 0x01,   ///< Wrong ReportId in received command.
        ErrorCode_WrongState                  = 0x02,   ///< Command was received when the tablet was in the wrong state, it cannot be processed.

        ErrorCode_CRC                         = 0x03,   ///< CRC error was detected in packed received by the tablet (serial interface only).
        ErrorCode_BadParameter                = 0x04,   ///< Parameter is not valid.
        ErrorCode_GraphicsWrongEncodingType   = 0x10,   ///< Wrong encoding type in report data.
        ErrorCode_GraphicsImageTooLong        = 0x11,   ///< Trying to draw outside video memory.
        ErrorCode_GraphicsZlibError           = 0x12,   ///< An error was returned by the ZLIB inflate function.

        ErrorCode_GraphicsWrongParameters     = 0x15,  ///< Image area or handwriting display area is invalid.

        ErrorCode_PadNotExist                 = 0x20, ///< The image for this pad setting does not exist, only used for PINpad and KEYpad mode.
        ErrorCode_RomSizeOverflow             = 0x33, ///< The image size exceed the limitation
        ErrorCode_RomInvalidParameter         = 0x34, ///< Parameter is not valid when using ROM image, such as wrong image number
        ErrorCode_RomErrorCRC                 = 0x35, ///< CRC error occurs when writing image to ROM

      //ErrorCode_GraphicsSprleImageOverflow  =  0x1E, // undocumented
      //ErrorCode_GraphicsSprleBlockOverflow  =  0x1F, // undocumented

      //ErrorCode_Crypt_errors                =  0x20, // undocumented
      //ErrorCode_Crypt_dh_zero_input         =  0x21, // undocumented
      //ErrorCode_Crypt_dh_test_failed        =  0x22, // undocumented
      //ErrorCode_Crypt_dh_wrong_module       =  0x23, // undocumented
      //ErrorCode_Crypt_dh_wrong_base         =  0x24, // undocumented
      //ErrorCode_Crypt_dh_wrong_openkey      =  0x25, // undocumented
      //ErrorCode_Crypt_dh_key_invalid        =  0x26, // undocumented
      //ErrorCode_Crypt_engine_invalid        =  0x27, // undocumented

      //ErrorCode_Internal                    =  0x80 ~ 0xff
      };

      /// @see getStatus()
      struct Status
      {
        StatusCode statusCode;      ///< @see Protocol::StatusCode
        ErrorCode  lastResultCode;  ///< @see Protocol::ErrorCode
        uint16_t   statusWord;      ///< reserved
      };

      /// @brief Retrieves the status of the tablet.
      ///
      /// @return Status.
      ////
      /// @see    ReportId_Status
      Status getStatus();


      /// @see setReset()
      enum ResetFlag : uint8_t
      {
        ResetFlag_Software = 0x00, ///< Perform a soft reset.
        ResetFlag_Hardware = 0x01  ///< Perform a hard reset.
      };

      /// @brief Sends a reset command.
      ///
      /// @param flag A value from Protocol::ResetFlag.
      ///
      /// @remarks Resets the tablet:
      ///   - Status is switched to StatusCode_Ready.
      ///   - Background is cleared to white.
      ///   - Inking Mode is set ON.
      ///   - Encryption mode is set OFF.
      ///   - Pen data option is set OFF.
      ///  The following is set to default:
      ///   - Ink threshold
      ///   - Image area
      ///   - Clear screen area
      ///   - Handwriting display area
      ///   - Handwriting thickness
      ///   - Handwriting color
      ///   - Background color
      ///
      /// @warning Following a Hardware reset you must re-open the USB connection.
      ///
      /// @see ReportId_Reset
      void setReset(ResetFlag flag);


      /// @see getHidInformation()
      struct HidInformation
      {
        uint16_t  idVendor;  ///< Vendor ID (assigned by the USB-IF)
        uint16_t  idProduct; ///< Product ID (assigned by the manfacturer)
        uint16_t  bcdDevice; ///< Device release number in binary-coded decimal
      };

      /// @brief  Retrieves the USB/HID identification information from the tablet.
      /// @return HidInformation.
      /// @see    ReportId_HidInformation
      /// @note   Only available in serial mode.
      HidInformation getHidInformation();


      /// @see getInformation()
      /// STU-540: secureIc will be 0, secureIcVersion[0] is firmware revision
      struct Information
      {
        union
        {
          char modelName[9];                  ///< model name
          char modelNameNullTerminated[9+1];
        };
        uint8_t firmwareMajorVersion;         ///< firmware version.
        uint8_t firmwareMinorVersion;         ///< firmware version.
        uint8_t secureIc;                     ///< non-zero if the security chip is present.
        uint8_t secureIcVersion[4];           ///< firmware version of the security chip.
      };

      /// @brief  Retrieves information from the tablet.
      /// @return Information.
      /// @see    ReportId_Information
      Information getInformation();


      /// @see getCapability()
      struct Capability
      {
        uint16_t tabletMaxX;        ///< The maximum value returned in PenData::x.
        uint16_t tabletMaxY;        ///< The maximum value returned in PenData::y.
        uint16_t tabletMaxPressure; ///< The maximum value returned in PenData::pressure.
        uint16_t screenWidth;       ///< The LCD width, in pixels.
        uint16_t screenHeight;      ///< The LCD height, in pixels.
        uint8_t  maxReportRate;     ///< <b>not supported for 300/500</b> Number of reports per second.
        uint16_t resolution;        ///< <b>not supported for 300/500</b> The number of tablet units per inch.
        union
        {
          uint8_t encodingFlag;    ///< <b>not supported for 300/500</b> Use with ProtocolHelper::simulateEncodingFlag() to ensure compatibility with older tablets.
          WacomGSS_deprecated uint8_t  zlibColorSupport; ///< <b>STU-520A only:</b> bit 0 is set if ZLIB is supported for color compression.
        };
      };

      /// @see Capability::encodingFlag
      enum EncodingFlag : uint8_t
      {
        EncodingFlag_Zlib  = 0x01, ///< set if ZLIB is supported for color compression (not bulk).
        EncodingFlag_1bit  = 0x02,
        EncodingFlag_16bit = 0x04,
        EncodingFlag_24bit = 0x08
      };


      /// @brief  Retrieves the capabilities of the tablet.
      ///
      /// @return Capability.
      /// @see    ReportId_Capability
      Capability getCapability();

      /// @name Uid
      /// @{

      /// @brief  Retrieves the unique ID of the tablet.
      /// @return The tablet identifier.
      /// @see    ReportId_Uid
      uint32_t getUid();

      /// @brief    Sets the unique ID of the tablet.
      /// @param    uid unique identifier.
      /// @warning  The UID data is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @see      ReportId_Uid
      void  setUid(uint32_t uid);
      /// @}

      /// @name Uid2
      /// @{

      /// @see getUid2()
      struct Uid2
      {
        union
        {
          char uid2[10];
          char uid2NullTerminated[10+1];
        };
      };

      /// @brief Retrieves the hardware unique ID of the tablet.
      /// @return The hardware unique ID.
      /// @see ReportId_Uid2
      /// @note Not supported on 300/500/520.
      Uid2 getUid2();
      /// @}


      /// @see getEserial()
      struct Eserial
      {
        union
        {
          char eSerial[13];
          char eSerialNullTerminated[13 + 1];
        };
      };

      /// @brief Retrieves the hardware unique ID of the tablet.
      /// @return The hardware unique ID.
      /// @see ReportId_Uid2
      /// @note Not supported on 300/500/520.
      Eserial getEserial();
      /// @}


      /// @name DefaultMode
      /// @{

      /// @brief description of the communications protocol.
      enum DefaultMode : uint8_t
      {
        DefaultMode_HID    = 0x1, ///< communications protocol will be STU/HID/USB
        DefaultMode_Serial = 0x2  ///< communications protocol will be STU/Serial/USB
      };

      /// @brief Sets the communication mode to use.
      /// This value will be stored in device non-volatile memory, and will not be changed by Power ON/OFF.
      /// The only way to change the value is to set this report again, and the setting will not take effect until next boot up.
      /// Hardware reset task is issued automatically after set the default interface mode.
      ///
      /// @note Only supported on 430V.
      void setDefaultMode(DefaultMode defaultMode);

      /// @brief Retrieves the communication mode that the tablet has been set to.
      /// @note Only supported on 430V.
      uint8_t getDefaultMode();
      /// @}


      /// @name ReportRate
      /// @{

      /// @brief Sets the packet report rate for serial devices.
      /// This feature is only supported in VCP mode, for USB HID mode, the report rate is fixed as 200 report/second.
      /// The report rate will be back to default after reboot, app need to set it each time after it is connected.
      /// The change of reportrate will soft reset the device, which means settings such as inking threshold will be back to default.
      /// Supported values are: 150, 200
      void setReportRate(uint8_t reportRate);

      /// @brief Retrieves the packet report rate for serial devices.
      /// @note Only supported on 430V.
      uint8_t getReportRate();
      /// @}


      /// @name RenderingMode
      /// @{

      /// @brief Selects ink algorithm to use
      enum RenderingMode : uint8_t
      {
        RenderingMode_Legacy = 0x00, ///< legacy rendering (default)
        RenderingMode_WILL   = 0x01  ///< high quality WILL rendering
      };

      /// @brief Sets the requested ink algorithm for the tablet.
      /// @note: only supported on STU-540
      void setRenderingMode(RenderingMode renderingMode);

      /// @brief Retrieves the current ink algorithm for the tablet.
      /// @note: only supported on STU-540
      uint8_t getRenderingMode();

      /// @}


      /// @brief 128-bit number.
      typedef std::array<uint8_t,16> PublicKey;

      /// @name HostPublicKey
      /// @{

      /// @brief  Retrieves the host public key.
      /// @return 128-bit key.
      /// @note   Not supported on 430/530.
      /// @see    ReportId_HostPublicKey
      PublicKey getHostPublicKey();

      /// @brief  Sets the host public key.
      /// @param  hostPublicKey 128-bit key.
      /// @note   Not supported on 430/530.
      /// @see    ReportId_HostPublicKey
      void      setHostPublicKey(PublicKey const & hostPublicKey);
      /// @}


      /// @brief  Retrieves the tablet's public key.
      /// @return 128-bit key.
      /// @note   Not supported on 430/530.
      /// @see    ReportId_DevicePublicKey
      PublicKey getDevicePublicKey();

      /// @brief    Start encrypted data capture session.
      /// @param    sessionId 32-bit session id.
      /// @remarks  Before using this command, encryption must be initialized.
      ///           If key exchange is not finished, the tablet will return error.
      ///
      /// @see      ReportId_StartCapture
      void setStartCapture(uint32_t sessionId);

      /// @brief  Ends encrypted data capture session.
      /// @see    ReportId_EndCapture
      void setEndCapture();

      /// @brief Diffie-Hellmann prime number.
      typedef std::array<uint8_t,16> DHprime;

      /// @name DHprime
      /// @{

      /// @brief  Retrieves the Diffie-Hellmann prime number.
      /// @return 128-bit number.
      /// @see    ReportId_DHprime
      DHprime getDHprime();

      /// @brief    Sets the Diffie-Hellmann prime number to use.
      /// @param    prime   128-bit number.
      /// @warning  This data is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @note   Not supported on 430/530.
      /// @see      ReportId_DHprime
      void    setDHprime(DHprime const & prime);
      /// @}


      /// @brief Holds a Diffie-Hellmann base number.
      typedef std::array<uint8_t,2> DHbase;

      /// @name DHbase
      /// @{

      /// @brief  Retrieves the Diffie-Hellmann base number.
      /// @return 16-bit number.
      /// @see    ReportId_DHbase
      DHbase getDHbase();

      /// @brief    Sets the Diffie-Hellmann base number to use.
      /// @param    base 16-bit number.
      /// @warning  This data is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @note   Not supported on 430/530.
      /// @see      ReportId_DHbase
      void   setDHbase(DHbase const & base);
      /// @}

      /// @brief Clears the LCD screen.
      ///
      /// @remark The whole screen is cleared using the selected background color.
      ///         Drawing and data transmission with the pen are interrupted during this operation.
      ///         The screen is cleared asychronously.
      /// @remark <b>STU-520 only:</b>The tablet can take 600ms to complete. During this time the tablet status is StatusCode_Image.
      /// @see    ReportId_ClearScreen
      void setClearScreen();

      /// @brief Clears an area the LCD screen.
      ///
      /// @remark The selected area is cleared using the selected background color.
      ///         Drawing and data transmission with the pen are interrupted during this operation.
      ///         The screen is cleared asychronously.
      /// @note   Not supported on 300/500/520.
      /// @see    ReportId_ClearScreenArea
      void setClearScreenArea(Rectangle const & area);


      /// @name InkingMode
      /// @{

      /// @brief inking mode values.
      enum InkingMode : uint8_t
      {
        InkingMode_Off = 0x00,
        InkingMode_On  = 0x01
      };

      /// @brief  Checks if inking is enabled.
      /// @return non-zero if inking is enabled.
      /// @see    ReportId_InkingMode
      uint8_t getInkingMode();

      /// @brief  Enables and disables inking.
      /// @param  inkingMode InkingMode.
      /// @see    ReportId_InkingMode
      void setInkingMode(InkingMode inkingMode);
      /// @}

      /// @brief  Pen pressure thresholds for drawing ink on LCD screen.
      /// @see    getInkThreshold(), setInkThreshold()
      /// Valid values are 0x000 to 0x3ff.
      /// Default on is 0x15
      /// Default off is 0x10
      struct InkThreshold
      {
        uint16_t onPressureMark;  ///< Pressure threshold for starting inking
        uint16_t offPressureMark; ///< Pressure threshold for stopping inking
      };

      /// @name InkThreshold
      /// @{

      /// @brief  Gets pressure thresholds used for drawing ink on LCD screen.
      /// @return InkThreshold.
      /// @see    ReportId_InkThreshold
      InkThreshold getInkThreshold();

      /// @brief  Sets pressure thresholds used for drawing ink on LCD screen.
      /// @param  inkThreshold Threshold values to set.
      /// @see    ReportId_InkThreshold
      void setInkThreshold(InkThreshold const & inkThreshold);
      /// @}


      /// @see setStartImageData()
      enum EncodingMode : uint8_t
      {
        EncodingMode_1bit       = 0x00, ///< uncompressed monochrome
        EncodingMode_1bit_Zlib  = 0x01, ///< Zlib-compressed monochrome
        EncodingMode_16bit      = 0x02, ///< uncompressed color
        EncodingMode_24bit      = 0x04, ///< uncompressed color (530 only)
        EncodingMode_1bit_Bulk  = 0x10, ///< data will be sent using Interface::write() instead of Interface::set().
        EncodingMode_16bit_Bulk = 0x12, ///< data will be sent using Interface::write() instead of Interface::set().
        EncodingMode_24bit_Bulk = 0x14 ///< data will be sent using Interface::write() instead of Interface::set() (530 only).
      };
      static const uint8_t EncodingMode_Raw       = 0x00; ///< uncompressed monochrome
      static const uint8_t EncodingMode_Zlib      = 0x01; ///< Zlib-compressed monochrome
      static const uint8_t EncodingMode_Bulk      = 0x10; ///< backwards-compatible
      static const uint8_t EncodingMode_16bit_565 = 0x02; ///< backwards-compatible

      /// @brief  Sends a Start Image Data Transmit command to the tablet.
      /// @param  encodingMode sets how the image data will be transferred.
      /// @see    ReportId_StartImageData @ref EncodingMode
      void setStartImageData(EncodingMode encodingMode);

      /// @deprecated Superseded by setStartImageData(EncodingMode)
      WacomGSS_deprecated void setStartImageData(uint8_t encodingMode) { setStartImageData(static_cast<EncodingMode>(encodingMode)); }


      /// @brief  Sends a Start Image Data Transmit command to the tablet.
      /// @param  encodingMode sets how the image data will be transferred.
      /// @param  area the area of the LCD screen to set.
      /// @note   Not supported on 300/500/520.
      /// @see    ReportId_StartImageDataArea @ref EncodingMode
      void setStartImageDataArea(EncodingMode encodingMode, Rectangle const & area);

      /// @brief  Holds a block of image data for transmission to the tablet.
      /// @see    setImageDataBlock()
      struct ImageDataBlock
      {
        static const uint16_t maxLengthHID    = 256-3;
        static const uint16_t maxLengthSerial = 2560-3;
        static const uint16_t maxLength540    = 2560-3;

        uint16_t                   length; ///< number of bytes in data.
        std::array<uint8_t,2560-3> data;   ///< image data.
      };

      /// @brief  Sends an image data block to the tablet.
      /// @param  imageDataBlock the next block of image data.
      /// @see    ReportId_ImageDataBlock
      void setImageDataBlock(ImageDataBlock const & imageDataBlock);

      /// @see    setEndImageData()
      enum EndImageDataFlag : uint8_t
      {
        EndImageDataFlag_Commit = 0x00, ///< commit image to LCD screen
        EndImageDataFlag_Abandon = 0x01 ///< abandon image update. Return to READY state.
      };

      /// @brief  Sends an End Image Data Transmit command to the tablet.
      /// @param  endImageDataFlag flag to commits or abandon image update.
      /// @see    ReportId_EndImageData
      void setEndImageData(EndImageDataFlag endImageDataFlag);

      WacomGSS_deprecated void setEndImageData() { setEndImageData(EndImageDataFlag_Commit); }

      /// @brief Holds thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// STU-540 thickness:
      /// 0=very thin (1px)
      /// 1=thin (3px) (default)
      /// 2=medium (5px)
      /// 3=bold (7px)
      struct HandwritingThicknessColor
      {
        uint16_t penColor;      ///< Ink color in 16-bit (5-6-5 RGB) format.
        uint8_t  penThickness;  ///< Ink thickness.
      };

      /// @name HandwritingThicknessColor
      /// @{

      /// @brief  Gets thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @return HandwritingThicknessColor.
      /// @see    ReportId_HandwritingThicknessColor
      HandwritingThicknessColor getHandwritingThicknessColor();

      /// @brief  Sets thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @param  handwritingThicknessColor The new thickness and color of the ink.
      /// @see    ReportId_HandwritingThicknessColor
      void setHandwritingThicknessColor(HandwritingThicknessColor const & handwritingThicknessColor);
      /// @}


      /// @brief Holds thinkness and color of pen used to draw ink on the tablet's LCD screen.
      struct HandwritingThicknessColor24
      {
        uint32_t penColor;      ///< Ink color in 24-bit 0x00rrggbb format.
        uint8_t  penThickness;  ///< Ink thickness.
      };

      /// @name HandwritingThicknessColor
      /// @{

      /// @brief  Gets thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @return HandwritingThicknessColor.
      /// @note   Not supported on 300/500/520.
      /// @see    ReportId_HandwritingThicknessColor
      HandwritingThicknessColor24 getHandwritingThicknessColor24();

      /// @brief  Sets thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @param  handwritingThicknessColor The new thickness and color of the ink.
      /// @note   Not supported on 300/500/520.
      /// @see    ReportId_HandwritingThicknessColor
      void setHandwritingThicknessColor24(HandwritingThicknessColor24 const & handwritingThicknessColor);
      /// @}


      /// @name BackgroundColor
      /// @{

      /// @brief  Gets the tablet background color.
      /// @return Color in 16-bit (5-6-5 RGB) format.
      /// @see    ReportId_BackgroundColor
      uint16_t getBackgroundColor();

      /// @brief    Sets the tablet background color.
      /// @param    backgroundColor Color in 16-bit (5-6-5 RGB) format.
      /// @remark   This method does not clear the screen, you must call clearScreen() separately.
      /// @warning  The background color is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @see      ReportId_BackgroundColor
      void     setBackgroundColor(uint16_t backgroundColor);
      /// @}


      /// @name BackgroundColor24
      /// @{

      /// @brief  Gets the tablet background color.
      /// @return Color in 24-bit 0x00rrggbb format.
      /// @note   Not supported on 300/500/520.
      /// @see    ReportId_BackgroundColor
      uint32_t getBackgroundColor24();

      /// @brief    Sets the tablet background color.
      /// @param    backgroundColor Color in 24-bit 0x00rrggbb format.
      /// @remark   This method does not clear the screen, you must call clearScreen() separately.
      /// @warning  The background color is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @note     Not supported on 300/500/520.
      /// @see      ReportId_BackgroundColor
      void     setBackgroundColor24(uint32_t backgroundColor);
      /// @}


      /// @name BootScreen
      /// @{

      /// @brief governs operation of boot screen image display
      enum BootScreenFlag : uint8_t
      {
        BootScreenFlag_Disable = 0x00,
        BootScreenFlag_Enable  = 0x01
      };

      /// @brief sets whether the boot image is shown at start up.
      void    setBootScreen(BootScreenFlag bootScreenFlag);

      /// @brief retrieves whether the boot image is shown at start up.
      uint8_t getBootScreen();

      /// @}


      /// @brief Defines rectangular area on the tablet's LCD sceen within which ink is captured.
      /// @deprecated Superseded by use of generic Rectangle struct
      struct HandwritingDisplayArea : Rectangle
      {
        HandwritingDisplayArea() noexcept {}
        HandwritingDisplayArea(Rectangle const & r) noexcept : Rectangle(r) {}
        HandwritingDisplayArea & operator = (Rectangle const & r) noexcept { static_cast<Rectangle &>(*this) = r; return *this; }
      };

      /// @name HandwritingDisplayArea
      /// @{

      /// @brief  Gets the area on the tablet's LCD sceen within which ink capture is enabled.
      /// @return HandwritingDisplayArea holding coordinates of ink capture rectangle.
      /// @see    ReportId_HandwritingDisplayArea
      Rectangle getHandwritingDisplayArea();

      /// @brief  Sets the area on the tablet's LCD sceen within which ink capture is enabled.
      /// @param  handwritingDisplayArea HandwritingDisplayArea holding coordinates of ink capture rectangle to set.
      /// @see    ReportId_HandwritingDisplayArea
      void                   setHandwritingDisplayArea(Rectangle const & area);

      /// @deprecated Superseded by setHandwritingDisplayArea(Rectangle const &)
      WacomGSS_deprecated
      void setHandwritingDisplayArea(HandwritingDisplayArea const & handwritingDisplayArea)
      {
        setHandwritingDisplayArea(static_cast<Rectangle const &>(handwritingDisplayArea));
      }

      /// @}

      /// @brief  Gets the tablet back-light brightness setting.
      /// @return Current brightness setting.
      /// @see    ReportId_BacklightBrightness
      uint16_t getBacklightBrightness();

      /// @brief    Sets the tablet back-light brightness.
      /// @param    backlightBrightness Brightness setting from 0 (dimmest) to 3 (brightest).
      /// @warning  The backlight brightness is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @see      ReportId_BacklightBrightness
      void     setBacklightBrightness(uint16_t backlightBrightness);


      /// @brief  Gets the tablet contrast setting.
      /// @return Current contrast setting.
      /// @see    ReportId_ScreenContrast
      uint16_t getScreenContrast();

      /// @brief    Sets the tablet screen contrast.
      /// @param    screenContrast contrast setting.
      /// @warning  The screen contrast is stored in non-volatile memory of the tablet. Do NOT write frequently.
      /// @see      ReportId_ScreenContrast
      void     setScreenContrast(uint16_t screenContrast);


      /// @name PenDataOptionMode
      /// @{

      /// @see setPenDataOptionMode()
      enum PenDataOptionMode : uint8_t
      {
        PenDataOptionMode_None              = 0x00, ///< Report PenData/PenDataEncrypted.
        PenDataOptionMode_TimeCount         = 0x01, ///< Report PenDataOption/PenDataEncryptedOption with timeCount field set. (520 only)
        PenDataOptionMode_SequenceNumber    = 0x02, ///< Report PenDataOption/PenDataEncryptedOption with sequenceNumber field set.
        PenDataOptionMode_TimeCountSequence = 0x03  ///< Report PenDataTimeCountSequence/PenDataTimeCountSequenceEncrypted with sequenceNumber field set. (430/530/540 only)
      };

      /// @brief  Retrieves the set option mode.
      /// @return OptionMode.
      /// @see    ReportId_PenDataOptionMode
      uint8_t getPenDataOptionMode();

      /// @brief  Enables or disables an option mode.
      /// @param  optionMode The new setting.
      /// @note   Not all tablets and firmwares support all option modes.
      /// @see    ReportId_PenDataOptionMode
      void    setPenDataOptionMode(PenDataOptionMode penDataOptionMode);
      /// @}


      /// @name OperationMode
      /// @{

      /// @see setOperationMode()
      enum OperationModeType : uint8_t
      {
        OperationModeType_Normal     = 0x00, ///< Tablet will return PenData
        OperationModeType_PinPad     = 0x01, ///< Tablet will return PinPadData
        OperationModeType_SlideShow  = 0x02, ///< Tablet will display bitmaps and not return pen data
        OperationModeType_KeyPad     = 0x03, ///< Tablet will return KeyPadData
        OperationModeType_Signature  = 0x04, ///< Tablet will display signature capture screen and buttons
        OperationModeType_MessageBox = 0x05  ///< not used in setOperationMode, @see setRomStartImage
      };

      /// @see setOperationMode()
      struct OperationMode_PinPad
      {
        uint8_t screenSelect;     ///< 1..3
        uint8_t pinBypass;        ///< When bypass is enable, digit 0 is approved if minDigit is not 0. 0x00=disable (default); 0x01=enable
        uint8_t minDigit;         ///< 0..12 (minimum digit must be less than maximum digit)
        uint8_t maxDigit;         ///< 0..12
        uint8_t numberHiddenMode; ///< 0=disable, 1=enable (number is changed to *)
        uint8_t idAfterEnter;     ///< 0=blank screen; 1-6 message box screen
        uint8_t idAfterCancel;    ///< 0=blank screen; 1-6 message box screen
      };

      /// @see setOperationMode()
      struct OperationMode_SlideShow
      {
        uint8_t                 workingMode;    ///< 0=round robin (need to set number and slideNumber accordingly); 1-10=slide number (only one slide shown)
        uint8_t                 numberOfSlides; ///< 2..10 number of slides to show
        std::array<uint8_t, 10> slideNumber;    ///< index of slide to show
        uint32_t                interval;       ///< 2000..120000 interval in milliseconds.
      };

      /// @see setOperationMode()
      struct OperationMode_KeyPad
      {
        uint8_t screenSelect;  ///< 1..3
        uint8_t idAfterSelect; ///< 0=blank screen; 1-6 message box screen
      };

      /// @see setOperationMode()
      struct OperationMode_Signature
      {
        uint8_t signatureScreen;  ///< 1 to 3: screen pattern select
        std::array<uint8_t, 3> keyDefinition; ///< 0=Cancel; 1=Enter; 2=Clear. The 3 keys must be defined differently
        uint8_t idAfterEnter;     ///< 0=blank screen; 1-6 message box screen
        uint8_t idAfterCancel;    ///< 0=blank screen; 1-6 message box screen
      };

      /// @see setOperationMode()
      struct OperationMode
      {
        uint8_t                 operationMode;
        std::array<uint8_t, 11> data;

        static OperationMode initializeNormal();
        static OperationMode initializePinPad(OperationMode_PinPad const & pinPad);
        static OperationMode initializeSlideShow(OperationMode_SlideShow const & slideShow);
        static OperationMode initializeKeyPad(OperationMode_KeyPad const & keyPad);
        static OperationMode initializeSignature(OperationMode_Signature const & signature);
      };

      /// @brief sets the operating mode that tablet runs in.
      void setOperationMode(OperationMode const & operationMode);

      /// @brief retrieves the current operating mode that tablet is running in.
      OperationMode getOperationMode();

      /// @}


      /// @name RomStartImageData
      /// @{

      /// Pin Pad key layouts
      /// <pre>
      /// Key layout 1:      Key layout 2:      Key layout 3:
      ///     7 8 9              1 4 7              1 2 3
      ///     4 5 6              2 5 8              4 5 6
      ///     1 2 3              3 6 9              7 8 9
      ///       0                  0                  0    <br/>
      /// Key layout 4:      Key layout 5:
      ///     7 4 1              3 6 9
      ///     8 5 2              2 5 8
      ///     9 6 3              1 4 7
      ///       0                  0
      /// </pre>
      struct RomStartImageData
      {
        EncodingMode      encodingMode; ///< limited to 24-bit raw only
        OperationModeType operationMode; ///< excluding Normal
        bool              imageType; ///< false=not pushed & default message; true=pushed & error message
        uint8_t           imageNumber; ///< pin=1..3; slide=1..10; key=1..3; signature=1..3; message=1..6
        std::array<uint8_t, 3> data;

        static const size_t KeyPadKeyEnabled = 9;
        static const size_t SigKeyEnabled    = 3;

        /// @param padType 1..3. 1=digits, 2=digits,asterisk,hash; 3=digits,dot,double-zero.
        /// @param keyLayout see comments
        static RomStartImageData initializePinPad(EncodingMode encodingMode, bool imageType, uint8_t imageNumber, uint8_t padType, uint8_t keyLayout);
        static RomStartImageData initializeSlideShow(EncodingMode encodingMode, bool imageType, uint8_t imageNumber);
        static RomStartImageData initializeKeyPad(EncodingMode encodingMode, bool imageType, uint8_t imageNumber, uint8_t padType, std::array<bool, KeyPadKeyEnabled> keyEnabled);
        static RomStartImageData initializeSignature(EncodingMode encodingMode, bool imageType, uint8_t imageNumber, std::array<bool, SigKeyEnabled> keyEnabled);
        static RomStartImageData initializeMessageBox(EncodingMode encodingMode, bool imageType, uint8_t imageNumber);
      };

      /// @brief begins sending of an image for storage in flash memory.
      /// @see setStartImageData(), setImageDataBlock(), setImageEndData()
      void setRomStartImageData(RomStartImageData const & romStartImage);

      ///@}

      ///@name RomImageHash
      ///@{

      /// @brief see getRomImageHash
      struct RomImageHash
      {
        uint8_t operationMode;
        bool    imageType;
        uint8_t imageNumber;
        uint8_t result;
        std::array<uint8_t, 16> hash;
      };

      /// @brief requests the specified image hash be made available.
      /// Use this call first to request an hash, then getRomImageHash() can be called to retrieve the value.
      void setRomImageHash(OperationModeType operationModeType, bool imageType, uint8_t imageNumber);

      /// @brief retrieves the image hash that has been requested.
      RomImageHash getRomImageHash();

      ///@}

      /// @see setRomImageDelete()
      enum RomImageDeleteMode : uint8_t
      {
        RomImageDeleteMode_All = 0x00,
        RomImageDeleteMode_PinPad_All = 0x01,
        RomImageDeleteMode_SlideShow_All = 0x02,
        RomImageDeleteMode_KeyPad_All = 0x03,
        RomImageDeleteMode_Signature_All = 0x04,
        RomImageDeleteMode_MessageBox_All = 0x05,
        RomImageDeleteMode_PinPad_Number = 0x06,
        RomImageDeleteMode_SlideShow_Number = 0x07,
        RomImageDeleteMode_KeyPad_Number = 0x08,
        RomImageDeleteMode_Signature_Number = 0x09,
        RomImageDeleteMode_MessageBox_Number = 0x0a
      };

      ///@brief Deletes one, some or all images from flash memory.
      void setRomImageDelete(RomImageDeleteMode romImageDeleteMode, bool imageType, uint8_t imageNumber);

      /// @note This command is only support PIN and Key pad mode.
      /// When you would like to display the message image from PC, please use this command and "Start Image Data Area".
      /// If the area of the message image is wrong, you will get the error code 0x15. This means "GRAPHICS_WRONG_PARAMETERS".
      /// Please refer to "Status[GET]". You can get the current message image area. (Current message Image Area(0x98)).
      Rectangle getCurrentImageArea();

      /// @brief Displays the selected image.
      void setRomImageDisplay(OperationModeType operationModeType, bool imageType, uint8_t imageNumber);

      /// @see struct EncryptionStatus
      enum StatusCodeRSA : uint8_t
      {
        StatusCodeRSA_Ready       = 0x00,
        StatusCodeRSA_Calculating = 0xFA,
        StatusCodeRSA_Even        = 0xFB,
        StatusCodeRSA_Long        = 0xFC,
        StatusCodeRSA_Short       = 0xFD,
        StatusCodeRSA_Invalid     = 0xFE,
        StatusCodeRSA_NotReady    = 0xFF
      };

      /// @see struct EncryptionStatus
      enum ErrorCodeRSA : uint8_t
      {
        ErrorCodeRSA_None                   = 0x00,
        ErrorCodeRSA_BadParameter           = 0x01,
        ErrorCodeRSA_ParameterTooLong       = 0x02,
        ErrorCodeRSA_PublicKeyNotReady      = 0x03,
        ErrorCodeRSA_PublicExponentNotReady = 0x04,
        ErrorCodeRSA_SpecifiedKeyInUse      = 0x05,
        ErrorCodeRSA_SpecifiedKeyNotInUse   = 0x06,
        ErrorCodeRSA_BadCommandCode         = 0x07,
        ErrorCodeRSA_CommandPending         = 0x08,
        ErrorCodeRSA_SpecifiedKeyExists     = 0x09,
        ErrorCodeRSA_SpecifiedKeyNotExist   = 0x0A,
        ErrorCodeRSA_NotInitialized         = 0x0B
      };

      /// @see struct EncryptionStatus
      enum SymmetricKeyType : uint8_t
      {
        SymmetricKeyType_AES128 = 0,
        SymmetricKeyType_AES192 = 1,
        SymmetricKeyType_AES256 = 2
      };

      /// @see struct EncryptionStatus
      enum AsymmetricKeyType : uint8_t
      {
        AsymmetricKeyType_RSA1024 = 0,
        AsymmetricKeyType_RSA1536 = 1,
        AsymmetricKeyType_RSA2048 = 2
      };

      /// @see struct EncryptionStatus
      enum AsymmetricPaddingType : uint8_t
      {
        AsymmetricPaddingType_None  = 0,
        AsymmetricPaddingType_PKCS1 = 1,
        AsymmetricPaddingType_OAEP  = 2
      };

      /// @brief Provides the current state of the v2 encryption engine
      struct EncryptionStatus
      {
        uint8_t symmetricKeyType;
        uint8_t asymmetricPaddingType;
        uint8_t asymmetricKeyType;
        uint8_t statusCodeRSAe; ///< status of public exponent
        uint8_t statusCodeRSAn; ///< status of host public key
        uint8_t statusCodeRSAc; ///< status of symmetric key
        uint8_t lastResultCode; ///< ErrorCodeRSA
        bool    rng;
        bool    sha1;
        bool    aes;

        static const ReportId reportId   = ReportId_EncryptionStatus;
        static const size_t   reportSize = 16;
      };

      /// @brief returns the encryption status for v2 style encryption
      /// @note not supported on 300/500/520
      EncryptionStatus getEncryptionStatus();


      enum EncryptionCommandNumber : uint8_t
      {
        EncryptionCommandNumber_SetEncryptionType    = 0x01,
        EncryptionCommandNumber_SetParameterBlock    = 0x02,
        EncryptionCommandNumber_GetStatusBlock       = 0x03,
        EncryptionCommandNumber_GetParameterBlock    = 0x04,
        EncryptionCommandNumber_GenerateSymmetricKey = 0x05,
      //EncryptionCommandNumber_Reserved_06          = 0x06,
      //EncryptionCommandNumber_Reserved_07          = 0x07,
      //EncryptionCommandNumber_Reserved_08          = 0x08,
      //EncryptionCommandNumber_Reserved_09          = 0x09
      };

      enum EncryptionCommandParameterBlockIndex : uint8_t
      {
        EncryptionCommandParameterBlockIndex_RSAe = 0,
        EncryptionCommandParameterBlockIndex_RSAn = 1,
        EncryptionCommandParameterBlockIndex_RSAc = 2,
        EncryptionCommandParameterBlockIndex_RSAm = 3
      };

      /// @see getEncryptionCommand()
      struct EncryptionCommand
      {
        EncryptionCommandNumber encryptionCommandNumber;
        uint8_t                 parameter;
        uint8_t                 lengthOrIndex;
        std::array<uint8_t,64>  data;

        /// @brief Helper function to initialize an EncryptionCommand parameter block with the SetEncryptionType command
        /// @param symmetricKeyType       symmetric key type
        /// @param asymmetricPaddingType  asymmetric padding type
        /// @param asymmetricKeyType      asymmetric key type
        /// @return Initialized EncryptionCommand object
        static EncryptionCommand initializeSetEncryptionType(SymmetricKeyType symmetricKeyType, AsymmetricPaddingType asymmetricPaddingType, AsymmetricKeyType asymmetricKeyType);

        /// @brief Helper function to initialize an EncryptionCommand parameter block with the SetParameterBlock command
        /// @param index  parameter block index
        /// @param length length of data block
        /// @param data   data block
        /// @return Initialized EncryptionCommand object
        static EncryptionCommand initializeSetParameterBlock(EncryptionCommandParameterBlockIndex index, uint8_t length, uint8_t const * data);

        /// @brief Helper function to initialize an EncryptionCommand parameter block with the GenerateSymmetricKey command
        /// @return Initialized EncryptionCommand object
        static EncryptionCommand initializeGenerateSymmetricKey();

        /// @brief Helper function to initialize an EncryptionCommand parameter block with the GetParameterBlock command
        /// @param index  parameter block index
        /// @param offset
        /// @return Initialized EncryptionCommand object
        static EncryptionCommand initializeGetParameterBlock(EncryptionCommandParameterBlockIndex index, uint8_t offset);
      };

      /// @brief  Retrieves the specified data from the v2 encryption engine
      /// @param  encryptionCommandNumber Encryption command to retrieve data for
      /// @return EncryptionCommand struct containing the requested data
      /// @note   not supported on 300/500/520
      EncryptionCommand getEncryptionCommand(EncryptionCommandNumber encryptionCommandNumber);


      /// @brief Sends the specified data to the v2 encryption engine
      /// @param  encryptionCommandNumber Encryption command to set
      /// @note not supported on 300/500/520
      void setEncryptionCommand(EncryptionCommand const & encryptionCommand);


      /// @see getReportSizeCollection()
      typedef std::array<uint16_t, 256> ReportSizeCollection;

      /// @brief Retrieves the size of each supported report.
      /// @see ReportSizeCollection
      /// @note Only supported on the 430V when in serial mode.
      ReportSizeCollection getReportSizeCollection();


      /// @brief  Data returned from the %STU tablet in response to pen movement when PenDataOptionMode_None is set.
      /// @remark This data will be sent from the %STU tablet automatically if pen is put in effective area.
      ///         (Pen pressure is not required for the %STU tablet to detect pen movement).
      struct PenData
      {
        uint8_t  rdy;      ///< non-zero if the stylus is in proximity.
        uint8_t  sw;       ///< non-zero if the stylus is considered to be touching the surface.
        uint16_t pressure; ///< pressure value in tablet units.
        uint16_t x;        ///< x-coordinate in tablet units.
        uint16_t y;        ///< y-coordinate in tablet units.

        static const ReportId reportId   = ReportId_PenData;
        static const size_t   reportSize = 6;
      };


      /// @brief Data returned from the %STU tablet in response to pen movement when PenDataOptionMode_TimeCount
      ///        or PenDataOptionMode_SequenceNumber is set.
      /// @remark This data will be sent from the %STU tablet automatically if pen is put in effective area.
      /// (Pen pressure is not required for the %STU tablet to detect pen movement).
      /// @note The value will wrap at 0xffff.
      struct PenDataOption : PenData
      {
        uint16_t option; ///< data dependant upon PenDataOptionMode setting

        static const ReportId reportId   = ReportId_PenDataOption;
        static const size_t   reportSize = 8;
      };

      /// @brief Data returned from the %STU tablet in response to pen movement when PenDataOptionMode_TimeCountSequence is set.
      struct PenDataTimeCountSequence : PenData
      {
        uint16_t timeCount;     ///< time count (incremented every 1ms)
        uint16_t sequence;      ///< pen data report sequence number

        static const ReportId reportId   = ReportId_PenDataTimeCountSequence;
        static const size_t   reportSize = 10;
      };

      /// @brief Data returned from the %STU tablet in encrypted mode in response to pen movement when PenDataOptionMode_None is set.
      struct PenDataEncrypted
      {
        uint32_t sessionId;  ///< as specified in setStartCapture()
        PenData  penData[2];

        static const ReportId reportId   = ReportId_PenDataEncrypted;
        static const size_t   reportSize = 16;
        static const size_t   encryptedSize = 16;
      };


      /// @brief Data returned from the %STU tablet in encrypted mode in response to pen movement when PenDataOptionMode_TimeCount
      ///        or PenDataOptionMode_SequenceNumber is set.
      struct PenDataEncryptedOption : PenDataEncrypted
      {
        uint16_t option[2];         ///< data dependant upon PenDataOptionMode setting

        static const ReportId reportId   = ReportId_PenDataEncryptedOption;
        static const size_t   reportSize = 20;
      };


      /// @brief Data returned from the %STU tablet in encrypted mode in response to pen movement when PenDataOptionMode_TimeCountSequence is set.
      struct PenDataTimeCountSequenceEncrypted : PenDataTimeCountSequence
      {
        uint32_t sessionId;

        static const ReportId reportId   = ReportId_PenDataTimeCountSequenceEncrypted;
        static const size_t   reportSize = 16;
        static const size_t   encryptedSize = 16;
      };

      /// @brief Encryption pulic key retrieved from the %STU tablet.
      struct DevicePublicKey : PublicKey
      {
        static const ReportId reportId   = ReportId_DevicePublicKey;
        static const size_t   reportSize = 16 /* = size()*/;
      };



      struct EventData
      {
        uint8_t  operationMode; ///< selector for type of data.
        std::array<uint8_t, 8> data;

        static const ReportId reportId = ReportId_EventData;
        static const size_t   reportSize = 9;
      };

      struct EventDataPinPad
      {
        uint8_t keyInput;
        std::array<uint8_t, 13> pin;


        static const ReportId reportId = ReportId_EventData;
        static const size_t   reportSize = 9;
        static const OperationModeType operationModeType = OperationModeType_PinPad;

        static EventDataPinPad initialize(EventData const & eventData);
      };

      struct EventDataKeyPad
      {
        uint8_t screenSelected;
        uint8_t keyNumber;

        static const ReportId reportId = ReportId_EventData;
        static const size_t   reportSize = 9;
        static const OperationModeType operationModeType = OperationModeType_KeyPad;

        static EventDataKeyPad initialize(EventData const & eventData);
      };

      struct EventDataSignature
      {
        uint8_t keyValue;

        static const ReportId reportId = ReportId_EventData;
        static const size_t   reportSize = 9;
        static const OperationModeType operationModeType = OperationModeType_Signature;

        static EventDataSignature initialize(EventData const & eventData);
      };


      struct EventDataEncrypted : EventData
      {
        uint32_t sessionId;  ///< as specified in setStartCapture()

        static const ReportId reportId = ReportId_EventDataEncrypted;
        static const size_t   reportSize = 16;
        static const size_t   encryptedSize = 16;
      };

      struct EventDataPinPadEncrypted : EventDataPinPad
      {
        uint32_t sessionId;  ///< as specified in setStartCapture()

        static const ReportId reportId = ReportId_EventDataEncrypted;
        static const size_t   reportSize = 16;
        static const size_t   encryptedSize = 16;

        static EventDataPinPadEncrypted initialize(EventDataEncrypted const & eventData);
      };

      struct EventDataKeyPadEncrypted : EventDataKeyPad
      {
        uint32_t sessionId;  ///< as specified in setStartCapture()

        static const ReportId reportId = ReportId_EventDataEncrypted;
        static const size_t   reportSize = 16;
        static const size_t   encryptedSize = 16;

        static EventDataKeyPadEncrypted initialize(EventDataEncrypted const & eventData);
      };

      struct EventDataSignatureEncrypted : EventDataSignature
      {
        uint32_t sessionId;  ///< as specified in setStartCapture()

        static const ReportId reportId = ReportId_EventDataEncrypted;
        static const size_t   reportSize = 16;
        static const size_t   encryptedSize = 16;

        static EventDataSignatureEncrypted initialize(EventDataEncrypted const & eventData);
      };



      /// @brief Unsafe (no checking) decoding of PenData.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, PenData & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0,cmd1,cmd2,cmd3,cmd4,cmd5;

        cmd0 = *i; ++i;
        data.rdy      = static_cast<uint8_t >((cmd0 >> 7) & 0x01);
        data.sw       = static_cast<uint8_t >((cmd0 >> 4) & 0x07);
        cmd1 = *i; ++i;
        data.pressure = static_cast<uint16_t>(((cmd0 & 0x0f) << 8) | cmd1);
        cmd2 = *i; ++i;
        cmd3 = *i; ++i;
        data.x        = static_cast<uint16_t>(((cmd2       ) << 8) | cmd3);
        cmd4 = *i; ++i;
        cmd5 = *i; ++i;
        data.y        = static_cast<uint16_t>(((cmd4       ) << 8) | cmd5);

        return i;
      }


      /// @brief  Unsafe (no checking) decoding of PenDataOption.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator iter, PenDataOption & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd6,cmd7;

        auto i = std::move(decodeReport_unsafe(std::move(iter), static_cast<PenData &>(data)));
        cmd6 = *i; ++i;
        cmd7 = *i; ++i;
        data.option = static_cast<uint16_t>((cmd6 << 8) | cmd7);

        return i;
      }


      /// @brief  Unsafe (no checking) decoding of PenDataTimeCountSequence.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator iter, PenDataTimeCountSequence & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd6,cmd7,cmd8,cmd9;

        auto i = std::move(decodeReport_unsafe(std::move(iter), static_cast<PenData &>(data)));
        cmd6 = *i; ++i;
        cmd7 = *i; ++i;
        data.timeCount = static_cast<uint16_t>((cmd6 << 8) | cmd7);
        cmd8 = *i; ++i;
        cmd9 = *i; ++i;
        data.sequence = static_cast<uint16_t>((cmd8 << 8) | cmd9);

        return i;
      }


      /// @brief  Unsafe (no checking) decoding of PenDataEncrypted.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, PenDataEncrypted & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0,cmd1,cmd2,cmd3;
        cmd0 = *i; ++i;
        cmd1 = *i; ++i;
        cmd2 = *i; ++i;
        cmd3 = *i; ++i;
        data.sessionId = (static_cast<uint32_t>(cmd0) << 24) | (cmd1 << 16) | (cmd2 << 8) | (cmd3);

        auto i1 = std::move(decodeReport_unsafe(std::move(i), data.penData[0]));
        auto i2 = std::move(decodeReport_unsafe(std::move(i1), data.penData[1]));

        return i2;
      }


      /// @brief  Unsafe (no checking) decoding of PenDataEncryptedOption.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, PenDataEncryptedOption & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0,cmd1,cmd2,cmd3, cmd16,cmd17,cmd18,cmd19;
        cmd0 = *i; ++i;
        cmd1 = *i; ++i;
        cmd2 = *i; ++i;
        cmd3 = *i; ++i;
        data.sessionId = (static_cast<uint32_t>(cmd0) << 24) | (cmd1 << 16) | (cmd2 << 8) | (cmd3);

        auto i1 = std::move(decodeReport_unsafe(std::move(i ), data.penData[0]));
        auto i2 = std::move(decodeReport_unsafe(std::move(i1), data.penData[1]));

        cmd16 = *i2; ++i2;
        cmd17 = *i2; ++i2;
        data.option[0] = static_cast<uint16_t>((cmd16 << 8) | cmd17);

        cmd18 = *i2; ++i2;
        cmd19 = *i2; ++i2;
        data.option[1] = static_cast<uint16_t>((cmd18 << 8) | cmd19);

        return i2;
      }


      /// @brief Unsafe (no checking) decoding of PenDataTimeCountSequenceEncrypted
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, PenDataTimeCountSequenceEncrypted & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0,cmd1,cmd2,cmd3,cmd4,cmd5,cmd6,cmd7,cmd8,cmd9/*,cmd10,cmd11*/,cmd12,cmd13,cmd14,cmd15;

        cmd0 = *i; ++i;
        data.rdy      = static_cast<uint8_t >((cmd0 >> 7) & 0x01);
        data.sw       = static_cast<uint8_t >((cmd0 >> 4) & 0x07);
        cmd1 = *i; ++i;
        data.pressure = static_cast<uint16_t>(((cmd0 & 0x0f) << 8) | cmd1);
        cmd2 = *i; ++i;
        cmd3 = *i; ++i;
        data.x        = static_cast<uint16_t>(((cmd2       ) << 8) | cmd3);
        cmd4 = *i; ++i;
        cmd5 = *i; ++i;
        data.y        = static_cast<uint16_t>(((cmd4       ) << 8) | cmd5);
        cmd6 = *i; ++i;
        cmd7 = *i; ++i;
        data.timeCount = static_cast<uint16_t>(((cmd6       ) << 8) | cmd7);
        cmd8 = *i; ++i;
        cmd9 = *i; ++i;
        data.sequence = static_cast<uint16_t>(((cmd8       ) << 8) | cmd9);
        /*cmd10 = *i;*/ ++i;
        /*cmd11 = *i;*/ ++i;
        // padding
        cmd12 = *i; ++i;
        cmd13 = *i; ++i;
        cmd14 = *i; ++i;
        cmd15 = *i; ++i;
        data.sessionId = (static_cast<uint32_t>(cmd12) << 24) | (static_cast<uint32_t>(cmd13) << 16) | (static_cast<uint32_t>(cmd14) << 8) | (static_cast<uint32_t>(cmd15));

        return i;
      }



      /// @brief  Unsafe (no checking) decoding of DevicePublicKey.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, DevicePublicKey & data)
      {
        for (auto d = data.begin(); d != data.end(); ++d)
        {
          *d = *i; ++i;
        }
        return i;
      }



      /// @brief Unsafe (no checking) decoding of PenData.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, EncryptionStatus & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type,std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0,cmd1,/*cmd2,*/cmd3,cmd4,cmd5,cmd6,cmd7/*,cmd8,cmd9,cmd10,cmd11,cmd12,cmd13,cmd14,cmd15*/;

        cmd0 = *i; ++i;
        data.symmetricKeyType = static_cast<SymmetricKeyType>(cmd0);
        cmd1 = *i; ++i;
        data.asymmetricPaddingType = static_cast<AsymmetricPaddingType>((cmd1 >> 6) & 0x03);
        data.asymmetricKeyType     = static_cast<AsymmetricKeyType>(cmd1 & 0x3F);
        /*cmd2 = *i;*/ ++i; // reserved
        cmd3 = *i; ++i;
        data.statusCodeRSAe = static_cast<StatusCodeRSA>(cmd3);
        cmd4 = *i; ++i;
        data.statusCodeRSAn = static_cast<StatusCodeRSA>(cmd4);
        cmd5 = *i; ++i;
        data.statusCodeRSAc = static_cast<StatusCodeRSA>(cmd5);
        cmd6 = *i; ++i;
        data.lastResultCode = static_cast<ErrorCodeRSA>(cmd6);
        cmd7 = *i; ++i;
        data.rng  = (cmd7 & 0x4) != 0;
        data.sha1 = (cmd7 & 0x2) != 0;
        data.aes  = (cmd7 & 0x1) != 0;
        /*cmd8  = *i;*/ ++i; //reserved
        /*cmd9  = *i;*/ ++i; //reserved
        /*cmd10 = *i;*/ ++i; //reserved
        /*cmd11 = *i;*/ ++i; //reserved
        /*cmd12 = *i;*/ ++i; //reserved
        /*cmd13 = *i;*/ ++i; //reserved
        /*cmd14 = *i;*/ ++i; //reserved
        /*cmd15 = *i;*/ ++i; //reserved
        return i;
      }


      /// @brief Unsafe (no checking) decoding of EventData.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, EventData & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type, std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0, cmd1, cmd2, cmd3, cmd4, cmd5, cmd6, cmd7, cmd8;

        cmd0 = *i; ++i;
        data.operationMode = cmd0;
        cmd1 = *i; ++i;
        data.data[0] = cmd1;
        cmd2 = *i; ++i;
        data.data[1] = cmd2;
        cmd3 = *i; ++i;
        data.data[2] = cmd3;
        cmd4 = *i; ++i;
        data.data[3] = cmd4;
        cmd5 = *i; ++i;
        data.data[4] = cmd5;
        cmd6 = *i; ++i;
        data.data[5] = cmd6;
        cmd7 = *i; ++i;
        data.data[6] = cmd7;
        cmd8 = *i; ++i;
        data.data[7] = cmd8;

        return i;
      }


      /// @brief Unsafe (no checking) decoding of EventDataEncrypted.
      template<typename Iterator>
      static Iterator decodeReport_unsafe(Iterator i, EventDataEncrypted & data)
      {
        static_assert(std::is_same<typename std::decay<typename std::iterator_traits<Iterator>::value_type>::type, std::uint8_t>::value, "Iterator must decay to type std::uint8_t");

        uint8_t cmd0, cmd1, cmd2, cmd3 /*, cmd13, cmd14, cmd15*/;
        cmd0 = *i; ++i;
        cmd1 = *i; ++i;
        cmd2 = *i; ++i;
        cmd3 = *i; ++i;
        data.sessionId = (static_cast<uint32_t>(cmd0) << 24) | (cmd1 << 16) | (cmd2 << 8) | (cmd3);

        auto i1 = std::move(decodeReport_unsafe(std::move(i), static_cast<EventData &>(data)));

        /*cmd13 = *i1;*/ ++i1; //padding
        /*cmd14 = *i1;*/ ++i1; //padding
        /*cmd15 = *i1;*/ ++i1; //padding

        return i1;
      }


      /// @brief  Generic decodeReport for iterators.
      template<typename InputIterator, class InputReport>
      static InputIterator decodeReport(InputIterator begin, InputIterator end, InputReport & data)
      {
        return decodeReport(begin, end, data, typename std::iterator_traits<InputIterator>::iterator_category());
      }


      /// @brief  Generic decodeReport for const containers.
      template<class Container, class InputReport>
      static typename Container::const_iterator decodeReport(Container const & cmd, InputReport & data)
      {
        return decodeReport(cmd.cbegin(), cmd.cend(), data, typename std::iterator_traits<typename Container::const_iterator>::iterator_category());
      }


      /// @brief  Generic decodeReport for non-const containers.
      template<class Container, class InputReport>
      static typename Container::iterator decodeReport(Container & cmd, InputReport & data)
      {
        return decodeReport(cmd.begin(), cmd.end(), data, typename std::iterator_traits<typename Container::const_iterator>::iterator_category());
      }


      /// @brief  Specialization of decodeReport for const std::array.
      template<class InputReport, std::size_t Size>
      static void decodeReport(std::array<std::uint8_t, Size> const & cmd, InputReport & data)
      {
        static_assert(Size >= InputReport::reportSize, "array size too small, must be at least InputReport::reportSize");
        decodeReport_unsafe(cmd.cbegin(), data);
      }


      /// @brief  Specialization of decodeReport for non-const std::array.
      template<class InputReport, std::size_t Size>
      static void decodeReport(std::array<std::uint8_t, Size> & cmd, InputReport & data)
      {
        decodeReport(const_cast<std::array<std::uint8_t, Size> const &>(cmd), data);
      }



    };



  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Protocol_hpp
