/// @file      WacomGSS/STU/Tablet.hpp
/// @copyright Copyright (c) 2012 Wacom Company Limited
/// @author    mholden
/// @date      2012-09-07
/// @brief     A higher level interface to provide simplified access.
#ifndef WacomGSS_STU_Tablet_hpp
#define WacomGSS_STU_Tablet_hpp

#include <WacomGSS/STU/ProtocolHelper.hpp>


namespace WacomGSS
{
  namespace STU
  {

    /// @brief Defines the interface for the handling of encryption.
    class EncryptionHandler
    {
    public:
      virtual ~EncryptionHandler();
      
      virtual void                reset() = 0;
      virtual void                clearKeys() = 0;
      virtual bool                requireDH() = 0;
      virtual void                setDH(Protocol::DHprime const & dhPrime, Protocol::DHbase const & dhBase) = 0; 
      virtual Protocol::PublicKey generateHostPublicKey() = 0;
      virtual void                computeSharedKey(Protocol::PublicKey const & devicePublicKey) = 0;
      virtual void                decrypt(std::uint8_t data[16]) = 0;
    };

    /// @brief Defines the interface for the handling of encryption.
    class EncryptionHandler2
    {
    public:
      virtual ~EncryptionHandler2();

      virtual void                      reset() = 0;
      virtual void                      clearKeys() = 0;      
      virtual void                      getParameters(Protocol::SymmetricKeyType &, Protocol::AsymmetricPaddingType &, Protocol::AsymmetricKeyType &) = 0;
      virtual std::vector<std::uint8_t> getPublicExponent() = 0; 
      virtual std::vector<std::uint8_t> generatePublicKey() = 0;       
      virtual void                      computeSessionKey(std::vector<uint8_t> & data) = 0;
      virtual void                      decrypt(std::uint8_t data[16]) = 0;
    };


    /// @brief Higher level interface to provide simplified access to %STU tablets.
    class Tablet : noncopyable
    {
      enum EncryptionType
      {
        EncryptionType_Unknown,
        EncryptionType_None,
        EncryptionType_v1,
        EncryptionType_v2
      };
        
      std::unique_ptr<Interface>          m_intf;
      std::shared_ptr<EncryptionHandler>  m_encryptionHandler;
      std::shared_ptr<EncryptionHandler2> m_encryptionHandler2;
      EncryptionType                      m_encryptionType;
      bool                                m_supported;
      std::array<std::uint16_t, 256>      m_reportCountLengths;
      Protocol::Status                    m_status;
      uint32_t                            m_sessionId;
      int                                 m_retries;
      chrono::milliseconds                m_sleepBetweenRetries;
      size_t                              m_asymmetricBlockSize;
      recursive_mutex mutable             m_apiMutex;
      
      void constructorDefaults() noexcept;

      void checkSupported(Protocol::ReportId) const;
      void checkErrorCode(Protocol::ErrorCode ec = Protocol::ErrorCode_None);
      void checkEncryptionSupported();

      static void wait(chrono::milliseconds time);

      Protocol::Status waitForStatusToSend(Protocol::ReportId, ProtocolHelper::OpDirection);

      void onFirstAttach();
      void onAttach();
      void clear();
      void initializeEncryption();
      void reinitializeEncryption();

      void internal_setEndCapture();

      Protocol protocol();

      class StateHandler; friend class StateHandler;

    public:
      /// @brief  Default constructor.
      Tablet() noexcept;

      
      /// @brief  Constructor.
      /// @param  encryptionHandler Shared pointer to object implmenting EncryptionHandler interface.
      Tablet(std::shared_ptr<EncryptionHandler> encryptionHandler);
      /// @brief  Constructor.
      /// @param  encryptionHandler2 Shared pointer to object implmenting EncryptionHandler2 interface.
      Tablet(std::shared_ptr<EncryptionHandler2> encryptionHandler2);
      /// @brief  Constructor.
      /// @param  encryptionHandler Shared pointer to object implmenting EncryptionHandler interface.
      /// @param  encryptionHandler2 Shared pointer to object implmenting EncryptionHandler2 interface.
      Tablet(std::shared_ptr<EncryptionHandler> encryptionHandler, std::shared_ptr<EncryptionHandler2> encryptionHandler2);
      

      /// @brief  Constructor.
      /// @param  intf  The Interface to attach to.
      /// @remark Takes ownership of the Interface object.
      Tablet(std::unique_ptr<Interface> && intf);

      /// @brief  Constructor.
      /// @param  intf              The Interface to attach to.
      /// @param  encryptionHandler Shared pointer to object implmenting EncryptionHandler interface.
      /// @remark Takes ownership of the Interface object.
      Tablet(std::unique_ptr<Interface> && intf, std::shared_ptr<EncryptionHandler> encryptionHandler);
      /// @brief  Constructor.
      /// @param  intf                The Interface to attach to.
      /// @param  encryptionHandler2  Shared pointer to object implmenting EncryptionHandler2 interface.
      /// @remark Takes ownership of the Interface object.
      Tablet(std::unique_ptr<Interface> && intf, std::shared_ptr<EncryptionHandler2> encryptionHandler2);

      /// @brief  Constructor.
      /// @param  intf                The Interface to attach to.
      /// @param  encryptionHandler   Shared pointer to object implmenting EncryptionHandler interface.
      /// @param  encryptionHandler2  Shared pointer to object implmenting EncryptionHandler2 interface.
      /// @remark Takes ownership of the Interface object.
      Tablet(std::unique_ptr<Interface> && intf, std::shared_ptr<EncryptionHandler> encryptionHandler, std::shared_ptr<EncryptionHandler2> encryptionHandler2);
      
      /// @brief Destructor.
      virtual ~Tablet() noexcept;

      /// @brief  Assigns an Interface to this object.
      /// @param  intf The Interface to attach to.
      /// @return This object.
      /// @remark Takes ownership of the Interface object.
      /// @see    attach()
      Tablet & operator = (std::unique_ptr<Interface> && intf);

      /// @brief  Attaches this object to an Interface.
      /// @param  intf The Interface to attach to.
      /// @remark Takes ownership of the Interface object.
      void attach(std::unique_ptr<Interface> && intf);
      
      /// @brief  Detaches this object from the Interface.
      std::unique_ptr<Interface> detach();

      /// @brief  Checks if an Interface has been assigned.
      /// @return true if no Interface has been assigned.
      bool empty() const noexcept;

      /// @brief  Checks if a tablet is connected.
      /// @return true if a tablet is connected.
      /// @remark This is safe to call if the interface object is null.
      bool isConnected() const;

      /// @brief  Disconnects from the tablet.
      /// @see    Interface::disconnect()
      void disconnect();

      /// @brief  Creates an interface queue bound to this interface.
      /// @see    Interface::interfaceQueue()
      InterfaceQueue interfaceQueue();

      /// @brief  Notifies all InterfacesQueues bound to this interface to check their predicate.
      /// @see    Interface::queueNotifyAll()
      void queueNotifyAll();

      /// @brief Sets the predicate of all InterfacesQueues bound to this interface.
      /// @see    Interface::queueSetPredicateAll()
      void queueSetPredicateAll(bool predicate);

      /// @brief  Reports if the device supports bulk transfer.
      /// @see    Interface::supportsWrite()
      bool supportsWrite() const;

      /// @brief  Retrieves the expected lengths of valid reports.
      /// @see    Interface::getReportCountLengths()
      bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const;
     
      /// @brief  Retrieves the idProduct of the tablet.
      ///
      /// @return returns the USB/HID identifier.
      ///
      /// @remark The interface must be connected to retrieve this information.      
      std::uint16_t getProductId() const;


      /// @brief    Reports if the Tablet supports the given report.
      /// @param    reportId  Id of report type to check.
      /// @return   true if the tablet supports the given report, false if it does not or if the 
      ///           Interface is unable to provide the required information 
      /// @warning  This is only a guide: not all Interface types can provide the information necessary to 
      ///           predetermine if the given report is supported
      /// @see      Interface::getReportCountLengths()
      bool isSupported(Protocol::ReportId reportId) const;
     
      /// @brief  Retrieves the status of the tablet.
      /// @see    Protocol::getStatus()
      Protocol::Status getStatus();

      /// @brief  Performs a soft reset on the tablet.
      /// @see    Protocol::setReset()
      void reset();
      
      /// @brief  Retrieves HID information from the tablet.
      /// @see    Protocol::getHidInformation()
      Protocol::HidInformation getHidInformation();

      /// @brief  Retrieves information from the tablet.
      /// @see    Protocol::getInformation()
      Protocol::Information getInformation();
      
      /// @brief  Retrieves the capabilities of the tablet.
      /// @see    Protocol::getCapability()
      Protocol::Capability getCapability();

      /// @brief  Retrieves the unique ID of the tablet.
      /// @see    Protocol::getUid()
      uint32_t getUid();
      
      /// @brief  Sets the unique ID of the tablet.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    Protocol::setUid()
      void     setUid(uint32_t uid);

      /// @brief  Retrieves the unique ID of the tablet.
      /// @see    Protocol::getUid2()
      Protocol::Uid2 getUid2();

      /// @brief  Retrieves the unique ID of the tablet.
      /// @see    Protocol::getUid2()
      Protocol::Eserial getEserial();
            
      /// @brief  Sets the communication mode to use.
      /// @note   requires a hard reboot to take affect.
      /// @note   Only supported on 430V.
      void     setDefaultMode(Protocol::DefaultMode defaultMode);

      /// @brief  Retrieves the communication mode that the tablet has been set to.
      /// @see    Protocol::getDefaultMode()
      /// @note   Only supported on 430V.
      uint8_t getDefaultMode();


      /// @brief Sets the packet report rate for serial devices.
      /// This feature is only supported in VCP mode, for USB HID mode, the report rate is fixed as 200 report/second.
      /// The report rate will be back to default after reboot, app need to set it each time after it is connected.
      /// The change of reportrate will soft reset the device, which means settings such as inking threshold will be back to default.      
      /// Supported values are: 150, 200
      /// @see Protocol::setReportRate()
      void setReportRate(uint8_t reportRate);


      /// @brief Retrieves the packet report rate for serial devices.
      /// @note Only supported on 430V.
      /// @see Protocol::getReportRate()
      uint8_t getReportRate();

      
      /// @brief Sets the requested ink algorithm for the tablet.
      /// @note: only supported on STU-540
      /// @see Protocol::setRenderingMode()
      void setRenderingMode(Protocol::RenderingMode renderingMode);


      /// @brief Retrieves the current ink algorithm for the tablet.
      /// @note: only supported on STU-540
      /// @see Protocol::getRenderingMode()
      uint8_t getRenderingMode();


      /// @brief sets whether the boot image is shown at start up.
      /// @note: only supported on STU-540
      /// @see Protocol::setBootScreen()
      void setBootScreen(Protocol::BootScreenFlag bootScreenFlag);


      /// @brief Retrieves whether the boot image is shown at start up.
      /// @note: only supported on STU-540
      /// @see Protocol::getBootScreen()
      uint8_t getBootScreen();


      /// @brief  Retrieves the host public key.
      /// @see    Protocol::getHostPublicKey()
      Protocol::PublicKey getHostPublicKey();
            
      //setHostPublicKey(PublicKey const & hostPublicKey);

      /// @brief  Retrieves the tablet's public key.
      /// @see    Protocol::getDevicePublicKey()
      Protocol::PublicKey getDevicePublicKey();

      /// @brief  Starts an encrypted data capture session.
      /// @remark Performs key exchange and enters encrypted mode.
      /// @see    Protocol::setStartCapture()
      void startCapture(uint32_t sessionId); // setup encrypt

      /// @brief  Ends an encrypted data capture session.
      /// @see    Protocol::setEndCapture()
      void endCapture();
      
      /// @brief  Retrieves the Diffie-Hellmann prime number.
      /// @see    Protocol::getDHprime()
      Protocol::DHprime getDHprime();

      /// @brief  Sets the Diffie-Hellmann prime number to use.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    Protocol::setDHprime()
      void  setDHprime(Protocol::DHprime const & prime); // change encryption

      /// @brief  Retrieves the Diffie-Hellmann base number.
      /// @see    Protocol::getDHbase()
      Protocol::DHbase getDHbase();

      /// @brief  Sets the Diffie-Hellmann prime number to use.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    Protocol::setDHbase()
      void  setDHbase(Protocol::DHbase const & base); // change encryption

      /// @brief  Clears the LCD screen.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    Protocol::setClearScreen()
      void setClearScreen(); // encryption, change stage to image
      
      /// @brief  Clears an area of the LCD screen.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    Protocol::setClearScreenArea()
      void setClearScreenArea(Protocol::Rectangle const & area); // encryption, change stage to image

      /// @brief  Checks if inking is enabled.
      /// @see    Protocol::getInkingMode()
      uint8_t getInkingMode();

      /// @brief  Enables and disables inking.
      /// @see    Protocol::setInkingMode()
      void setInkingMode(Protocol::InkingMode inkingMode);

      /// @brief  Gets pressure thresholds used for drawing ink on the tablet's LCD screen.
      /// @see    Protocol::getInkThreshold()
      Protocol::InkThreshold getInkThreshold();

      /// @brief  Sets pressure thresholds used for drawing ink on the tablet's LCD screen.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    Protocol::setInkThreshold()
      void                   setInkThreshold(Protocol::InkThreshold const & inkThreshold);

      //void startImageData(uint8_t encodingMode); // encryption
      //void imageDataBlock(ImageDataBlock const & imageDataBlock);
      
      /// @brief  Sends an image to the Tablet.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    ProtocolHelper::writeImage()
      void writeImage(Protocol::EncodingMode encodingMode, uint8_t const * data, size_t length); // encryption, update screen time

      /// @brief  Sends an image to an area of the Tablet screen.
      /// @note   If the tablet is in encrypted mode, this is temporarily suspended for the duration of this call and then restarted.
      /// @see    ProtocolHelper::writeImage()
      void writeImageArea(Protocol::EncodingMode encodingMode, Protocol::Rectangle const & area, uint8_t const * data, size_t length); // encryption, update screen time

      /// @deprecated Superseded by writeImage(Protocol::EncodingMode, uint8_t const *, size_t)
      WacomGSS_deprecated
      void writeImage(uint8_t encodingMode, uint8_t const * data, size_t length); // encryption, update screen time
      

      /// @brief  Sends an End Image Data command to the tablet.
      /// @note   This method is provided for situations where the tablet is unexpectedly in 
      ///         StatusCode::StatusCode_Image in order to return it to StatusCode::StatusCode_Ready. 
      ///         However the screen may be in an undefined state and it is recommended that you 
      ///         call clearScreen() instead.
      /// @see    Protocol::setEndImageData()      
      void endImageData(Protocol::EndImageDataFlag endImageDataFlag);
      
      /// @deprecated Superseded by endImageData(Protocol::EndImageDataFlag)
      WacomGSS_deprecated void endImageData() { endImageData(Protocol::EndImageDataFlag_Commit); }
            
      /// @brief  Gets thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @see    Protocol::getHandwritingThicknessColor()
      Protocol::HandwritingThicknessColor getHandwritingThicknessColor();

      /// @brief  Sets the thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @see    Protocol::setHandwritingThicknessColor()
      void    setHandwritingThicknessColor(Protocol::HandwritingThicknessColor const & handwritingThicknessColor);
      
      /// @brief  Gets thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @see    Protocol::getHandwritingThicknessColor24()
      Protocol::HandwritingThicknessColor24 getHandwritingThicknessColor24();

      /// @brief  Sets the thinkness and color of pen used to draw ink on the tablet's LCD screen.
      /// @see    Protocol::setHandwritingThicknessColor24()
      void    setHandwritingThicknessColor24(Protocol::HandwritingThicknessColor24 const & handwritingThicknessColor24);

      /// @brief  Gets the tablet background color.
      /// @see    Protocol::getBackgroundColor()
      uint16_t getBackgroundColor();

      /// @brief  Sets the tablet background color.
      /// @see    Protocol::setBackgroundColor()
      void     setBackgroundColor(uint16_t backgroundColor);

      /// @brief  Gets the tablet background color.
      /// @see    Protocol::getBackgroundColor24()
      uint32_t getBackgroundColor24();

      /// @brief  Sets the tablet background color.
      /// @see    Protocol::setBackgroundColor24()
      void     setBackgroundColor24(uint32_t backgroundColor);

      /// @brief  Gets the area on the tablet's LCD sceen within which ink capture is enabled.
      /// @see    Protocol::getHandwritingDisplayArea()
      Protocol::Rectangle getHandwritingDisplayArea();

      /// @brief  Sets the area on the tablet's LCD sceen within which ink capture is enabled.
      /// @see    Protocol::setHandwritingDisplayArea()
      void  setHandwritingDisplayArea(Protocol::Rectangle const & handwritingDisplayArea);

      /// @deprecated Superseded by setHandwritingDisplayArea(Protocol::Rectangle const &)
      WacomGSS_deprecated void setHandwritingDisplayArea(Protocol::HandwritingDisplayArea const & handwritingDisplayArea)
      {
        setHandwritingDisplayArea(static_cast<Protocol::Rectangle const &>(handwritingDisplayArea));
      }
       
      /// @brief  Gets the tablet back-light brightness setting.
      /// @see    Protocol::getBacklightBrightness()
      uint16_t getBacklightBrightness();

      /// @brief  Sets the tablet back-light brightness.
      /// @see    Protocol::setBacklightBrightness()
      void     setBacklightBrightness(uint16_t backlightBrightness);
 
      /// @brief  Gets the tablet LCD contrast setting.
      /// @see    Protocol::getScreenContrast()
      uint16_t getScreenContrast();

      /// @brief  Sets the tablet LCD contrast setting.
      /// @see    Protocol::setScreenContrast()
      void     setScreenContrast(uint16_t screenContrast);
      
      /// @brief  Checks if option data is attached to handwriting XY coordinates information.
      /// @see    Protocol::getPenDataOptionMode()
      uint8_t getPenDataOptionMode();

      /// @brief  Enables or disables attaching option data to handwriting XY coordinates information.
      /// @see    Protocol::setPenDataOptionMode()
      void setPenDataOptionMode(Protocol::PenDataOptionMode penDataOptionMode);
      
      /// @brief  Retrieves the current extended encryption status
      /// @see    Protocol::getEncryptionStatus()
      Protocol::EncryptionStatus getEncryptionStatus();

      /// @brief  Retrieves the specified encryption parameter
      /// @see    Protocol::getEncryptionCommand()      
      Protocol::EncryptionCommand getEncryptionCommand(Protocol::EncryptionCommandNumber encryptionCommandNumber);

      /// @brief  Decrypts a block of data using the encryption handler supplied to the constructor
      /// @param  data  16-byte block of data to decrypt
      void decrypt(std::uint8_t data[16]) const;

      /// @brief sets the operating mode that tablet runs in.
      void setOperationMode(Protocol::OperationMode const & operationMode);

      /// @brief retrieves the current operating mode that tablet is running in.
      Protocol::OperationMode getOperationMode();

      /// @brief begins sending of an image for storage in flash memory.
      /// @see setRomStartImageData(), setStartImageData(), setImageDataBlock(), setImageEndData()
      void writeRomImage(Protocol::RomStartImageData const & romStartImage, uint8_t const * data, size_t length);

      /// @brief requests the specified image hash be made available.
      /// Use this call first to request an hash, then getRomImageHash() can be called to retrieve the value.
      void setRomImageHash(Protocol::OperationModeType operationModeType, bool imageType, uint8_t imageNumber);

      /// @brief retrieves the image hash that has been requested.
      Protocol::RomImageHash getRomImageHash();

      ///@brief Deletes one, some or all images from flash memory.
      void setRomImageDelete(Protocol::RomImageDeleteMode romImageDeleteMode, bool imageType, uint8_t imageNumber);

      /// @note This command is only support PIN and Key pad mode.
      /// When you would like to display the message image from PC, please use this command and "Start Image Data Area".
      /// If the area of the message image is wrong, you will get the error code 0x15. This means "GRAPHICS_WRONG_PARAMETERS".
      /// Please refer to "Status[GET]". You can get the current message image area. (Current message Image Area(0x98)).
      Protocol::Rectangle getCurrentImageArea();

      /// @brief Displays the selected image.
      void setRomImageDisplay(Protocol::OperationModeType operationModeType, bool imageType, uint8_t imageNumber);

      /// @brief  Thrown if you attempt to use an unsupported report type.
      ///
      /// You can avoid this error by calling isSupported() before attempting to use the report type
      WacomGSS_STU_runtime_error_decl(not_supported_error)
    };



  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Tablet_hpp
