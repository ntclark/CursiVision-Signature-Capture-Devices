#include <WacomGSS/STU/Tablet.hpp>
#include <WacomGSS/compatibility/thread.hpp>

namespace WacomGSS
{
  namespace STU
  {

    //=========================================================================

    EncryptionHandler::~EncryptionHandler()
    {
    }
      
    void EncryptionHandler::reset()
    {
    }
    

    EncryptionHandler2::~EncryptionHandler2()
    {
    }

    //=========================================================================

    class Tablet::StateHandler : noncopyable
    {
      Tablet &      m_tablet;
      bool          m_encryption;
      std::uint32_t m_sessionId;
    public:
      StateHandler(Tablet & tablet, Protocol::ReportId reportId, ProtocolHelper::OpDirection opDirection);
      void restore();
    };



    Tablet::StateHandler::StateHandler(Tablet & tablet, Protocol::ReportId reportId, ProtocolHelper::OpDirection opDirection)
    :
      m_tablet(tablet),
      m_encryption(false)
    {      
      if (!ProtocolHelper::statusCanSend(m_tablet.m_status.statusCode, reportId, opDirection))
      {
        if (m_tablet.m_status.statusCode == Protocol::StatusCode_Capture)
        {
          m_encryption = true;
          m_sessionId = m_tablet.m_sessionId;
          m_tablet.endCapture();
        }
        else if (m_tablet.m_status.statusCode == Protocol::StatusCode_Image)
        {
          m_tablet.protocol().setEndImageData(Protocol::EndImageDataFlag_Abandon);
        }
        m_tablet.waitForStatusToSend(reportId, opDirection);
      }
    }



    void Tablet::StateHandler::restore()
    {
      if (m_encryption)
      {
        //m_tablet.reinitializeEncryption();
        m_tablet.startCapture(m_sessionId);
      }
    }



    //=========================================================================
    
    
    
    Protocol::Status Tablet::waitForStatusToSend(Protocol::ReportId reportId, ProtocolHelper::OpDirection opDirection)
    {
      auto p = protocol();
      m_status = ProtocolHelper::waitForStatusToSend(p, reportId, opDirection, m_retries, m_sleepBetweenRetries);
      return m_status;
    }


    void Tablet::checkErrorCode(Protocol::ErrorCode ec)
    {
      m_status = protocol().getStatus();
      if (m_status.lastResultCode != ec)
      {
        throw std::runtime_error("API failed");
      }
    }



    void Tablet::wait(chrono::milliseconds time)
    {
      #if WacomGSS_compatibility_thread == 1
        ::boost::this_thread::sleep(::boost::posix_time::millisec(time.count())); // boost
      #else
        this_thread::sleep_for(time); // C++11
      #endif
    }


    void Tablet::clear()
    {
      m_sessionId = 0;
      m_supported = false;
      m_reportCountLengths.fill(0);
      
      if (m_encryptionHandler)
        m_encryptionHandler->reset();
      if (m_encryptionHandler2)
        m_encryptionHandler2->reset();
      m_encryptionType = EncryptionType_Unknown;
      m_asymmetricBlockSize = 0;
    }


    Protocol Tablet::protocol()
    {
      if (!m_intf)
        throw std::logic_error("interface object has not been set");
      return Protocol(*m_intf);
    }


    void Tablet::onAttach()
    {
      m_encryptionType = EncryptionType_Unknown;
      if (isConnected())
      {
        m_supported = m_intf->getReportCountLengths(m_reportCountLengths);
        m_status    = protocol().getStatus();
      }
    }

    void Tablet::onFirstAttach()
    {
      onAttach();
      if (m_status.statusCode != Protocol::StatusCode_Ready)
      {
        if (m_status.statusCode == Protocol::StatusCode_Capture)
        {
          internal_setEndCapture();          
        }
      }
    }

    //=========================================================================
    
    void Tablet::constructorDefaults() noexcept
    {
      m_encryptionType = EncryptionType_Unknown;
      m_supported = false;
      m_sessionId = 0;
      m_retries = 25;
      m_sleepBetweenRetries = chrono::milliseconds(50);
      m_asymmetricBlockSize = 0;
    }


    Tablet::Tablet() noexcept
    {
      constructorDefaults();
    }


    Tablet::Tablet(std::shared_ptr<EncryptionHandler> encryptionHandler)
    :
      m_encryptionHandler(encryptionHandler)
    {
      constructorDefaults();
    }

    Tablet::Tablet(std::shared_ptr<EncryptionHandler2> encryptionHandler2)
    :
      m_encryptionHandler2(encryptionHandler2)
    {
      constructorDefaults();
    }

    Tablet::Tablet(std::shared_ptr<EncryptionHandler> encryptionHandler, std::shared_ptr<EncryptionHandler2> encryptionHandler2)
    :
      m_encryptionHandler(encryptionHandler),
      m_encryptionHandler2(encryptionHandler2)
    {
      constructorDefaults();
    }

    Tablet::Tablet(std::unique_ptr<Interface> && intf)
    :
      m_intf(std::move(intf))      
    {
      constructorDefaults();
      onFirstAttach();
    }

    Tablet::Tablet(std::unique_ptr<Interface> && intf, std::shared_ptr<EncryptionHandler> encryptionHandler)
    :
      m_intf(std::move(intf)),
      m_encryptionHandler(encryptionHandler)      
    {
      constructorDefaults();
      onFirstAttach();
    }

    Tablet::Tablet(std::unique_ptr<Interface> && intf, std::shared_ptr<EncryptionHandler2> encryptionHandler2)
    :
      m_intf(std::move(intf)),
      m_encryptionHandler2(encryptionHandler2)
    {
      constructorDefaults();
      onFirstAttach();
    }

    Tablet::Tablet(std::unique_ptr<Interface> && intf, std::shared_ptr<EncryptionHandler> encryptionHandler, std::shared_ptr<EncryptionHandler2> encryptionHandler2)
    :
      m_intf(std::move(intf)),
      m_encryptionHandler(encryptionHandler),
      m_encryptionHandler2(encryptionHandler2)      
    {
      constructorDefaults();
      onFirstAttach();
    }


    Tablet::~Tablet() noexcept    
    {
      try
      {
        lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
        clear();
      }
      catch (...)
      {
      }
    }

    Tablet & Tablet::operator = (std::unique_ptr<Interface> && intf)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      attach(std::move(intf));
      return *this;
    }
    
    void Tablet::attach(std::unique_ptr<Interface> && intf)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      clear();
      m_intf = std::move(intf);

      onAttach();      
    }


    std::unique_ptr<Interface> Tablet::detach()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      clear();
      return std::move(m_intf);
    }



    //---------------------

    bool Tablet::empty() const noexcept
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return !m_intf;
    }


    bool Tablet::isConnected() const
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return m_intf && m_intf->isConnected();
    }


    void Tablet::disconnect()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      m_intf->disconnect();
    }


    InterfaceQueue Tablet::interfaceQueue()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return std::move(m_intf->interfaceQueue());
    }


    void Tablet::queueNotifyAll()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      m_intf->queueNotifyAll();
    }


    void Tablet::queueSetPredicateAll(bool predicate)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      m_intf->queueSetPredicateAll(predicate);
    }


    bool Tablet::supportsWrite() const
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return m_intf->supportsWrite();
    }


    bool Tablet::getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return m_intf->getReportCountLengths(reportCountLengths);
    }


    std::uint16_t Tablet::getProductId() const
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return m_intf->getProductId();
    }

    //---------------------


    bool Tablet::isSupported(Protocol::ReportId reportId) const
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      return !m_supported || m_reportCountLengths[reportId] != 0;
    }


    void Tablet::checkSupported(Protocol::ReportId reportId) const
    {
      if (!isSupported(reportId))
      {
        throw not_supported_error();
      }
    }

    
    Protocol::Status Tablet::getStatus()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      m_status = protocol().getStatus();
      return m_status;
    }



    void Tablet::reset()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      clear();
      protocol().setReset(Protocol::ResetFlag_Software);
      wait(chrono::milliseconds(1100)); // Originally 750. Needed to be increased for STU-540.
      checkErrorCode();
    }

    Protocol::HidInformation Tablet::getHidInformation()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430V
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HidInformation);
      
      auto ret = protocol().getHidInformation();
      checkErrorCode();
      return ret;
    }

    Protocol::Information Tablet::getInformation()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all
      
      auto ret = protocol().getInformation();
      checkErrorCode();
      return ret;
    }


    Protocol::Capability Tablet::getCapability()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all
      
      auto ret = protocol().getCapability();
      checkErrorCode();
      return ret;
    }

    

    uint32_t Tablet::getUid()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      auto ret = protocol().getUid();
      checkErrorCode();
      return ret;
    }


    void Tablet::setUid(uint32_t uid)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready
      
      StateHandler state(*this, Protocol::ReportId_Uid, ProtocolHelper::OpDirection_Set);
 
      protocol().setUid(uid);
      checkErrorCode();

      state.restore();
    }



    Protocol::Uid2 Tablet::getUid2()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430
      // OpStatus     : all

      checkSupported(Protocol::ReportId_Uid2);

      auto ret = protocol().getUid2();
      checkErrorCode();
      return ret;
    }


    Protocol::Eserial Tablet::getEserial()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : all

      checkSupported(Protocol::ReportId_Eserial);

      auto ret = protocol().getEserial();
      checkErrorCode();
      return ret;
    }



    uint8_t Tablet::getDefaultMode()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430V
      // OpStatus     : all

      checkSupported(Protocol::ReportId_DefaultMode);

      auto ret = protocol().getDefaultMode();
      checkErrorCode();
      return ret;
    }


    void Tablet::setDefaultMode(Protocol::DefaultMode defaultMode)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430V
      // OpStatus     : ready

      checkSupported(Protocol::ReportId_DefaultMode);

      StateHandler state(*this, Protocol::ReportId_DefaultMode, ProtocolHelper::OpDirection_Set);

      protocol().setDefaultMode(defaultMode);
      checkErrorCode();

      state.restore();
    }



    uint8_t Tablet::getReportRate()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430V
      // OpStatus     : all

      checkSupported(Protocol::ReportId_ReportRate);

      auto ret = protocol().getReportRate();
      checkErrorCode();
      return ret;
    }


    void Tablet::setReportRate(uint8_t reportRate)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430V
      // OpStatus     : ready

      checkSupported(Protocol::ReportId_ReportRate);

      StateHandler state(*this, Protocol::ReportId_ReportRate, ProtocolHelper::OpDirection_Set);

      protocol().setReportRate(reportRate);
      checkErrorCode();

      state.restore();
    }


    uint8_t Tablet::getRenderingMode()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : all

      checkSupported(Protocol::ReportId_RenderingMode);

      StateHandler state(*this, Protocol::ReportId_RenderingMode, ProtocolHelper::OpDirection_Get);

      auto ret = protocol().getRenderingMode();
      checkErrorCode();
      return ret;
    }


    void Tablet::setRenderingMode(Protocol::RenderingMode renderingMode)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready

      checkSupported(Protocol::ReportId_RenderingMode);

      StateHandler state(*this, Protocol::ReportId_RenderingMode, ProtocolHelper::OpDirection_Set);

      protocol().setRenderingMode(renderingMode);
      checkErrorCode();

      state.restore();
    }


    uint8_t Tablet::getBootScreen()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : all

      checkSupported(Protocol::ReportId_BootScreen);

      StateHandler state(*this, Protocol::ReportId_BootScreen, ProtocolHelper::OpDirection_Get);

      auto ret = protocol().getBootScreen();
      checkErrorCode();
      return ret;
    }


    void Tablet::setBootScreen(Protocol::BootScreenFlag bootScreenFlag)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready

      checkSupported(Protocol::ReportId_BootScreen);

      StateHandler state(*this, Protocol::ReportId_BootScreen, ProtocolHelper::OpDirection_Set);

      protocol().setBootScreen(bootScreenFlag);
      checkErrorCode();

      state.restore();
    }


    Protocol::PublicKey Tablet::getHostPublicKey()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      auto ret = protocol().getHostPublicKey();
      checkErrorCode();
      return ret;
    }



    Protocol::PublicKey Tablet::getDevicePublicKey()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      auto ret = protocol().getDevicePublicKey();
      checkErrorCode();
      return ret;
    }

    void Tablet::reinitializeEncryption()
    {
      switch (m_encryptionType)
      {
        case EncryptionType_v1:
          break;

        case EncryptionType_v2:
          {
            auto p = protocol();
            
            // step 6: Request the device to calculate key
            // step 7 (& 8): Wait for the completion.

            auto encryptionStatus = ProtocolHelper::generateSymmetricKeyAndWaitForEncryptionStatus(p, m_retries, m_sleepBetweenRetries, m_retries*m_sleepBetweenRetries);
            if (encryptionStatus.statusCodeRSAc != Protocol::StatusCodeRSA_Ready)
              throw runtime_error("statusCodeRSAc != Protocol::StatusCodeRSA_Ready");

            // step 9: Retrieve the AES key from the device.

            std::vector<uint8_t> bin(m_asymmetricBlockSize); // encryption block size
            {
              uint8_t index = 0;
              for (auto iter = bin.begin(); iter != bin.end(); ++index)
              {
                p.setEncryptionCommand(Protocol::EncryptionCommand::initializeGetParameterBlock(Protocol::EncryptionCommandParameterBlockIndex_RSAc, index));

                auto encryptionCommand = p.getEncryptionCommand(Protocol::EncryptionCommandNumber_GetParameterBlock);

                iter = std::copy_n(encryptionCommand.data.begin(), encryptionCommand.lengthOrIndex, iter);
              }
            }

            // step 10: Decrypt The AES key.

            m_encryptionHandler2->computeSessionKey(bin);
          }
          break;

        case EncryptionType_Unknown:
        case EncryptionType_None:        
        default:
          throw std::logic_error("invalid encryptionType value");
      }
    }

    void Tablet::initializeEncryption()
    { 
      auto p = protocol();

      if ( m_encryptionType == EncryptionType_Unknown )
      {
        Protocol::DHprime dhPrime = { 0 };
        try
        {
          dhPrime = p.getDHprime();
          checkErrorCode();
        }
        catch (...)
        { }
        
        
        if ( ProtocolHelper::supportsEncryption(dhPrime) ) 
        {
          m_encryptionType = EncryptionType_v1;
        }
        else
        {
          try
          {
            p.getEncryptionStatus();
            checkErrorCode();
            m_encryptionType = EncryptionType_v2;
          }
          catch (...)
          {
            m_encryptionType = EncryptionType_None;
            throw;
          }
        }
      }

      if (m_encryptionType == EncryptionType_v1)
      {
        if (!m_encryptionHandler)
          throw runtime_error("no encryption handler supplied");

        if (m_encryptionHandler->requireDH())
        {
          Protocol::DHprime dhPrime = { 0 };
          try
          {
            dhPrime = p.getDHprime();
            checkErrorCode();
          }
          catch (...)
          {
            //MessageBox(m_hWnd, L"Unexpected: firmware missing API", nullptr, MB_ICONEXCLAMATION | MB_OK);
          }

          auto dhBase = p.getDHbase();
          checkErrorCode();

          m_encryptionHandler->setDH(dhPrime, dhBase);
        }      
        auto hostPublicKey = m_encryptionHandler->generateHostPublicKey();

        if (!ProtocolHelper::statusCanSend(m_status.statusCode, Protocol::ReportId_HostPublicKey, ProtocolHelper::OpDirection_Set))
        {
          waitForStatusToSend(Protocol::ReportId_HostPublicKey, ProtocolHelper::OpDirection_Set);
        }

        auto devicePublicKey = ProtocolHelper::setHostPublicKeyAndPollForDevicePublicKey(p, hostPublicKey, m_retries, m_sleepBetweenRetries);
        checkErrorCode();

        m_encryptionHandler->computeSharedKey(devicePublicKey);
      }
      else
      {
        if (!m_encryptionHandler2)
          throw runtime_error("no encryption handler supplied");

        Protocol::SymmetricKeyType      symmetricKeyType      = Protocol::SymmetricKeyType_AES256;
        Protocol::AsymmetricPaddingType asymmetricPaddingType = Protocol::AsymmetricPaddingType_OAEP;
        Protocol::AsymmetricKeyType     asymmetricKeyType     = Protocol::AsymmetricKeyType_RSA2048;
        m_encryptionHandler2->getParameters(symmetricKeyType, asymmetricPaddingType, asymmetricKeyType);
        
        m_asymmetricBlockSize = 2048/8;
        switch (asymmetricKeyType)
        {
          case Protocol::AsymmetricKeyType_RSA1024: m_asymmetricBlockSize = 1024/8; break;
          case Protocol::AsymmetricKeyType_RSA1536: m_asymmetricBlockSize = 1536/8; break;
          case Protocol::AsymmetricKeyType_RSA2048: m_asymmetricBlockSize = 2048/8; break;
          default:
            throw std::logic_error("unknown AsymmetricKeyType value");
        }

        if (!ProtocolHelper::statusCanSend(m_status.statusCode, Protocol::ReportId_EncryptionCommand, ProtocolHelper::OpDirection_Set))
        {
          waitForStatusToSend(Protocol::ReportId_EncryptionCommand, ProtocolHelper::OpDirection_Set);
        }

        // step 1: Tell the device to use encryption type.

        p.setEncryptionCommand(Protocol::EncryptionCommand::initializeSetEncryptionType(symmetricKeyType, asymmetricPaddingType, asymmetricKeyType));
        checkErrorCode();

        // step 2: Generate a couple of RSA keys.        

        // exponent
        std::vector<std::uint8_t> e = m_encryptionHandler2->getPublicExponent();
        if (e.empty())
          throw std::logic_error("invalid exponent");
        
        // public key
        std::vector<std::uint8_t> publicKey = m_encryptionHandler2->generatePublicKey();
        auto size = publicKey.size();
        
        // step 3: Tell the public exponent (e) to the device.

        p.setEncryptionCommand(Protocol::EncryptionCommand::initializeSetParameterBlock(Protocol::EncryptionCommandParameterBlockIndex_RSAe, static_cast<uint8_t>(e.size() & 0xff), e.data()));
        checkErrorCode();

        // step 4: Tell the device to public key (n) to the device.

        const auto blockSize = Protocol::EncryptionCommand().data.size();

        for (size_t offset = 0; offset < size; offset += blockSize)
        {
          auto remaining = size - (offset*blockSize);
          uint8_t chunk = remaining > blockSize ? static_cast<uint8_t>(blockSize) : static_cast<uint8_t>(remaining);
          p.setEncryptionCommand(Protocol::EncryptionCommand::initializeSetParameterBlock(Protocol::EncryptionCommandParameterBlockIndex_RSAn, chunk, publicKey.data()+offset));
          checkErrorCode();
        }
  
        // step 5: Get Status Block...check e and n are OK.

        auto encryptionStatus = p.getEncryptionStatus();
        if (encryptionStatus.statusCodeRSAe != Protocol::StatusCodeRSA_Ready || encryptionStatus.statusCodeRSAn != Protocol::StatusCodeRSA_Ready)
          throw runtime_error("failed up upload public key");
      }

      reinitializeEncryption();
    }


    void Tablet::startCapture(std::uint32_t sessionId)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready

      if ( ! m_encryptionHandler && ! m_encryptionHandler2 )
        throw std::logic_error("cannot startCapture without an EncryptionHandler");

      if ( m_encryptionType == EncryptionType_None )
        throw not_supported_error();

      if ( m_status.statusCode == Protocol::StatusCode_Capture )
      {
        m_sessionId = 0;
        internal_setEndCapture();        
      }
      
      initializeEncryption();
      
      if ( ! ProtocolHelper::statusCanSend(m_status.statusCode, Protocol::ReportId_StartCapture, ProtocolHelper::OpDirection_Set) )
      {
        waitForStatusToSend(Protocol::ReportId_StartCapture, ProtocolHelper::OpDirection_Set);
      }
      
      protocol().setStartCapture(sessionId);
      checkErrorCode();
      m_sessionId = sessionId;
    }



    void Tablet::internal_setEndCapture()
    {
      // Ensure minimum time from endimage or startCapture otherwise pad will timeout.
      this_thread::sleep_for(chrono::milliseconds(80));

      protocol().setEndCapture();          

      checkErrorCode();
      if (m_status.statusCode != Protocol::StatusCode_Ready)
      {
        auto p = protocol();
        m_status = ProtocolHelper::waitForStatus(p, Protocol::StatusCode_Ready, m_retries, m_sleepBetweenRetries);          
      }
    }



    void Tablet::endCapture()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : capture

      if (m_status.statusCode == Protocol::StatusCode_Capture)
      {
        m_sessionId = 0;
        internal_setEndCapture();        
      }
    }



    void Tablet::setClearScreen()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

      StateHandler state(*this, Protocol::ReportId_ClearScreen, ProtocolHelper::OpDirection_Set);

      protocol().setClearScreen();

      checkErrorCode();

      state.restore();
    }



    void Tablet::setClearScreenArea(Protocol::Rectangle const & area)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);


      StateHandler state(*this, Protocol::ReportId_ClearScreenArea, ProtocolHelper::OpDirection_Set);

      protocol().setClearScreenArea(area);
      checkErrorCode();

      state.restore();
    }

    Protocol::DHprime Tablet::getDHprime()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

      //checkSupported(Protocol::ReportId_DHprime);

      auto ret = protocol().getDHprime();
      checkErrorCode();
      return ret;
    }

    void Tablet::setDHprime(Protocol::DHprime const & dhPrime)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready

      bool restartEncryption = false;
      if (!ProtocolHelper::statusCanSend(m_status.statusCode, Protocol::ReportId_DHprime, ProtocolHelper::OpDirection_Set))
      {
        if (m_status.statusCode == Protocol::StatusCode_Capture)
        {
          internal_setEndCapture();
          
          restartEncryption = true;
        }
        else if (m_status.statusCode == Protocol::StatusCode_Image)
        {
          protocol().setEndImageData(Protocol::EndImageDataFlag_Abandon);
          checkErrorCode();
        }

        waitForStatusToSend(Protocol::ReportId_DHprime, ProtocolHelper::OpDirection_Set);
      }

      protocol().setDHprime(dhPrime);
      checkErrorCode();

      if (restartEncryption)
      {
        startCapture(m_sessionId);
      }      
    }


    Protocol::DHbase Tablet::getDHbase()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      auto ret = protocol().getDHbase();
      checkErrorCode();
      return ret;
    }


    void Tablet::setDHbase(Protocol::DHbase const & dhBase)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready

      checkSupported(Protocol::ReportId_DHbase);

      StateHandler state(*this, Protocol::ReportId_DHbase, ProtocolHelper::OpDirection_Set);

      protocol().setDHbase(dhBase);
      checkErrorCode();

      state.restore();
    }


    uint8_t Tablet::getInkingMode()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      auto ret = protocol().getInkingMode();
      checkErrorCode();
      return ret;
    }


    void Tablet::setInkingMode(Protocol::InkingMode inkingMode)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      protocol().setInkingMode(inkingMode);
      checkErrorCode();
    }



    Protocol::InkThreshold Tablet::getInkThreshold()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : all

      auto ret = protocol().getInkThreshold();
      checkErrorCode();
      return ret;
    }


    void Tablet::setInkThreshold(Protocol::InkThreshold const & inkThreshold)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready

      StateHandler state(*this, Protocol::ReportId_InkThreshold, ProtocolHelper::OpDirection_Set);
      
      protocol().setInkThreshold(inkThreshold);
      checkErrorCode();

      state.restore();
    }
    
    
    void Tablet::writeImage(Protocol::EncodingMode encodingMode, uint8_t const * data, size_t length)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready
            
      StateHandler state(*this, Protocol::ReportId_StartImageData, ProtocolHelper::OpDirection_Set);
    
      uint16_t maxImageBlockSize = (m_reportCountLengths[Protocol::ReportId_ImageDataBlock] > 3) ? (m_reportCountLengths[Protocol::ReportId_ImageDataBlock] - 3u) : Protocol::ImageDataBlock::maxLengthHID;

      auto p = protocol();
      ProtocolHelper::writeImage(p, encodingMode, maxImageBlockSize, data, length, m_retries, m_sleepBetweenRetries);
      checkErrorCode();

      state.restore();
    }
    

    void Tablet::writeImage(uint8_t encodingMode, uint8_t const * data, size_t length)
    {
      // lock_guard will be called next...
      writeImage(static_cast<Protocol::EncodingMode>(encodingMode), data, length);
    }
    

    void Tablet::writeImageArea(Protocol::EncodingMode encodingMode, Protocol::Rectangle const & area, uint8_t const * data, size_t length)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : ready
            
      StateHandler state(*this, Protocol::ReportId_StartImageDataArea, ProtocolHelper::OpDirection_Set);
    
      uint16_t maxImageBlockSize = (m_reportCountLengths[Protocol::ReportId_ImageDataBlock] > 3) ? (m_reportCountLengths[Protocol::ReportId_ImageDataBlock] - 3u) : Protocol::ImageDataBlock::maxLengthHID;

      auto p = protocol();
      ProtocolHelper::writeImageArea(p, encodingMode, area, maxImageBlockSize, data, length, m_retries, m_sleepBetweenRetries);
      checkErrorCode();

      state.restore();
    }
    

    void Tablet::endImageData(Protocol::EndImageDataFlag endImageDataFlag)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : all
      // OpStatus     : image

      if (m_status.statusCode == Protocol::StatusCode_Image)
      {
        protocol().setEndImageData(endImageDataFlag);
        checkErrorCode();
      }
    }


    Protocol::HandwritingThicknessColor Tablet::getHandwritingThicknessColor()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HandwritingThicknessColor);

      auto ret = protocol().getHandwritingThicknessColor();
      checkErrorCode();
      return ret;
    }


    void Tablet::setHandwritingThicknessColor(Protocol::HandwritingThicknessColor const & handwritingThicknessColor)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HandwritingThicknessColor);
      
      protocol().setHandwritingThicknessColor(handwritingThicknessColor);
      checkErrorCode();
    }



    Protocol::HandwritingThicknessColor24 Tablet::getHandwritingThicknessColor24()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430/530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HandwritingThicknessColor24);

      auto ret = protocol().getHandwritingThicknessColor24();
      checkErrorCode();
      return ret;
    }


    void Tablet::setHandwritingThicknessColor24(Protocol::HandwritingThicknessColor24 const & handwritingThicknessColor24)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430/530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HandwritingThicknessColor24);
      
      protocol().setHandwritingThicknessColor24(handwritingThicknessColor24);
      checkErrorCode();
    }


    uint16_t Tablet::getBackgroundColor()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_BackgroundColor);

      auto ret = protocol().getBackgroundColor();
      checkErrorCode();

      return ret;
    }


    void Tablet::setBackgroundColor(uint16_t backgroundColor)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_BackgroundColor);
  
      protocol().setBackgroundColor(backgroundColor);
      checkErrorCode();
    }



    uint32_t Tablet::getBackgroundColor24()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_BackgroundColor24);

      auto ret = protocol().getBackgroundColor24();
      checkErrorCode();

      return ret;
    }


    void Tablet::setBackgroundColor24(uint32_t backgroundColor24)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_BackgroundColor24);
  
      protocol().setBackgroundColor24(backgroundColor24);
      checkErrorCode();
    }

    Protocol::Rectangle Tablet::getHandwritingDisplayArea()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HandwritingDisplayArea);

      auto ret = protocol().getHandwritingDisplayArea();
      checkErrorCode();

      return ret;
    }

    void Tablet::setHandwritingDisplayArea(Protocol::Rectangle const & handwritingDisplayArea)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_HandwritingDisplayArea);
  
      protocol().setHandwritingDisplayArea(handwritingDisplayArea);
      checkErrorCode();
    }


    uint16_t Tablet::getBacklightBrightness()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : all

      checkSupported(Protocol::ReportId_BacklightBrightness);

      auto ret = protocol().getBacklightBrightness();
      checkErrorCode();

      return ret;
    }


    void Tablet::setBacklightBrightness(uint16_t backlightBrightness)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 520A
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_BacklightBrightness);

      waitForStatusToSend(Protocol::ReportId_BacklightBrightness, ProtocolHelper::OpDirection_Set);

      protocol().setBacklightBrightness(backlightBrightness);
      checkErrorCode();
    }


    uint16_t Tablet::getScreenContrast()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430/530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_ScreenContrast);

      auto ret = protocol().getScreenContrast();
      checkErrorCode();

      return ret;
    }


    void Tablet::setScreenContrast(uint16_t screenContrast)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430/530
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_ScreenContrast);

      waitForStatusToSend(Protocol::ReportId_ScreenContrast, ProtocolHelper::OpDirection_Set);

      protocol().setBacklightBrightness(screenContrast);
      checkErrorCode();
    }


    uint8_t Tablet::getPenDataOptionMode()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 500 520A 430 530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_PenDataOptionMode);

      auto ret = protocol().getPenDataOptionMode();
      checkErrorCode();

      return ret;
    }

    void Tablet::setPenDataOptionMode(Protocol::PenDataOptionMode penDataOptionMode)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 500 520A 430 530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_PenDataOptionMode);
  
      protocol().setPenDataOptionMode(penDataOptionMode);
      checkErrorCode();
    }
    


    Protocol::EncryptionStatus Tablet::getEncryptionStatus()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430 530
      // OpStatus     : all

      checkSupported(Protocol::ReportId_EncryptionStatus);

      auto ret = protocol().getEncryptionStatus();
      checkErrorCode();

      return ret;
    }



    Protocol::EncryptionCommand Tablet::getEncryptionCommand(Protocol::EncryptionCommandNumber encryptionCommandNumber)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 430 530
      // OpStatus     : ready | image | capture

      checkSupported(Protocol::ReportId_EncryptionCommand);

      waitForStatusToSend(Protocol::ReportId_EncryptionCommand, ProtocolHelper::OpDirection_Set);

      auto ret = protocol().getEncryptionCommand(encryptionCommandNumber);
      checkErrorCode();

      return ret;
    }


    void Tablet::decrypt(std::uint8_t data[16]) const
    {
      switch (m_encryptionType)
      {
        case EncryptionType_v1:
          if (m_encryptionHandler)
            m_encryptionHandler->decrypt(data);
          break;

        case EncryptionType_v2:
          if (m_encryptionHandler2)
            m_encryptionHandler2->decrypt(data);
          break;
        
        case EncryptionType_None:
        case EncryptionType_Unknown:
          break;

        default:
          std::logic_error("unexpected encryptionType value");
      }
    }


    Protocol::OperationMode Tablet::getOperationMode()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_OperationMode);

      StateHandler state(*this, Protocol::ReportId_OperationMode, ProtocolHelper::OpDirection_Get);

      auto ret = protocol().getOperationMode();
      checkErrorCode();
      return ret;
    }


    void Tablet::setOperationMode(Protocol::OperationMode const & operationMode)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_OperationMode);

      StateHandler state(*this, Protocol::ReportId_OperationMode, ProtocolHelper::OpDirection_Set);

      protocol().setOperationMode(operationMode);
      checkErrorCode();

      state.restore();
    }



    ///@brief Deletes one, some or all images from flash memory.
    void Tablet::setRomImageDelete(Protocol::RomImageDeleteMode romImageDeleteMode, bool imageType, uint8_t imageNumber)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_RomImageDelete);

      StateHandler state(*this, Protocol::ReportId_RomImageDelete, ProtocolHelper::OpDirection_Set);

      protocol().setRomImageDelete(romImageDeleteMode, imageType, imageNumber);
      checkErrorCode();

      state.restore();
    }



    Protocol::Rectangle Tablet::getCurrentImageArea()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_CurrentImageArea);

      StateHandler state(*this, Protocol::ReportId_CurrentImageArea, ProtocolHelper::OpDirection_Get);

      auto ret = protocol().getCurrentImageArea();
      checkErrorCode();
      return ret;
    }



    void Tablet::setRomImageDisplay(Protocol::OperationModeType operationModeType, bool imageType, uint8_t imageNumber)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_RomImageDisplay);

      StateHandler state(*this, Protocol::ReportId_RomImageDisplay, ProtocolHelper::OpDirection_Set);

      protocol().setRomImageDisplay(operationModeType, imageType, imageNumber);
      checkErrorCode();

      state.restore();
    }



    void Tablet::setRomImageHash(Protocol::OperationModeType operationModeType, bool imageType, uint8_t imageNumber)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_RomImageHash);

      StateHandler state(*this, Protocol::ReportId_RomImageHash, ProtocolHelper::OpDirection_Set);

      protocol().setRomImageHash(operationModeType, imageType, imageNumber);
      checkErrorCode();

      state.restore();
    }

    
    Protocol::RomImageHash Tablet::getRomImageHash()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready | capture

      checkSupported(Protocol::ReportId_RomImageHash);

      StateHandler state(*this, Protocol::ReportId_RomImageHash, ProtocolHelper::OpDirection_Get);

      auto ret = protocol().getRomImageHash();
      checkErrorCode();

      return ret;
    }


    void Tablet::writeRomImage(Protocol::RomStartImageData const & romStartImage, uint8_t const * data, size_t length)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      // Compatbility : 540
      // OpStatus     : ready

      checkSupported(Protocol::ReportId_RomStartImageData);

      StateHandler state(*this, Protocol::ReportId_RomStartImageData, ProtocolHelper::OpDirection_Set);

      uint16_t maxImageBlockSize = (m_reportCountLengths[Protocol::ReportId_ImageDataBlock] > 3) ? (m_reportCountLengths[Protocol::ReportId_ImageDataBlock] - 3u) : Protocol::ImageDataBlock::maxLengthHID;

      auto p = protocol();
      ProtocolHelper::writeRomImage(p, romStartImage, maxImageBlockSize, data, length, m_retries, m_sleepBetweenRetries);
      checkErrorCode();

      state.restore();
    }



    WacomGSS_STU_runtime_error_impl( Tablet::not_supported_error       , "not_supported_error"       )


  }
}
