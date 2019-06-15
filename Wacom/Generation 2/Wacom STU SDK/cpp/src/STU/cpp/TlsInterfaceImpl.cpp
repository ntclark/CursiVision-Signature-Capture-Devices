#include <WacomGSS/STU/TlsInterfaceImpl.hpp>
#include <WacomGSS/STU/TlsProtocol.hpp>



namespace WacomGSS
{
  namespace STU
  {


    TlsInterfaceImpl::TlsInterfaceImpl()
    :
      m_outPipeId(0x01),
      m_inPipeId(0x82),
      m_timeOut(1000),
      m_sslCipherList("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"),
      m_sslConnect(0)
    {
    }
    

    void TlsInterfaceImpl::processReadBuffer(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
    {
      queueReport(std::vector<uint8_t>(begin, end));
    }




    void TlsInterfaceImpl::startReaderThread()
    {
      stopReaderThread();

      m_readerThread = std::move(thread(std::bind(std::mem_fn(&TlsInterfaceImpl::readerThread), this)));

      int retries = 500;
      do
      {
        this_thread::sleep_for(chrono::milliseconds(10));
        if (m_readerThreadRunning.load())
          return;
      }
      while (--retries);
      
      throw std::runtime_error("The background reader thread failed to start in a timely manner");
    }


    


    static OpenSSL::EVP_PKEY createRSAkey()
    {
      using namespace OpenSSL;
      
      BIGNUM exp;
      BN_set_word(exp, OpenSSL::RSA_F4);

      RSA rsa;      
      if (RSA_generate_key_ex(rsa, 2048, exp, nullptr) != 1)
      {
        throw error("RSA_generate_key_ex");
      }

      EVP_PKEY pkey;
      if (EVP_PKEY_assign_RSA(pkey, std::move(rsa)) != 1)
      {
        throw error("EVP_PKEY_assign_RSA");
      }

      return pkey;
    }



    static OpenSSL::X509 createCertificate(long serialNumber, char const *commonName, OpenSSL::EVP_PKEY & pkey, OpenSSL::X509 * issuer, OpenSSL::EVP_PKEY *signKey, bool isCA, long offset_sec)
    {
      using namespace OpenSSL;

      X509 x509;

      if (X509_set_version(x509, 2) != 1) // 2 == x509v3
        throw error("X509_set_version"); 

      if (ASN1_INTEGER_set(X509_get_serialNumber(x509), serialNumber) != 1)
        throw error("ASN1_INTEGER_set");

      if (X509_time_adj_ex(X509_get_notBefore(x509), 0, 0, nullptr) == nullptr)
        throw error("X509_time_adj_ex");

      if (X509_time_adj_ex(X509_get_notAfter(x509), 0, offset_sec, nullptr) == nullptr)
        throw error("X509_time_adj_ex");

      if (X509_set_pubkey(x509, pkey) != 1)
        throw error("X509_set_pubkey");

      auto name = X509_get_subject_name(x509);

      if (X509_NAME_add_entry_by_txt(name, "C", MBSTRING_UTF8, reinterpret_cast<unsigned char const *>("JP"), -1, -1, 0) != 1) // Country
        throw error("X509_NAME_add_entry_by_txt");
      if (X509_NAME_add_entry_by_txt(name, "O", MBSTRING_UTF8, reinterpret_cast<unsigned char const *>("Wacom Co.,Ltd."), -1, -1, 0) != 1) // Country
        throw error("X509_NAME_add_entry_by_txt");
      if (X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_UTF8, reinterpret_cast<unsigned char const *>(commonName), -1, -1, 0) != 1) // Country
        throw error("X509_NAME_add_entry_by_txt");
 
      if (issuer)
      {
        name = X509_get_subject_name(*issuer);
      }

      if (X509_set_issuer_name(x509, name) != 1)
        throw error("X509_set_issuer_name");
      
      if (isCA)
      {
        /*
        //Add X509v3 extention
        X509_EXTENSION	*ext;
        //STACK_OF(X509_EXTENSION)	*extlist;
        char			*exname = "basicConstraints";
        char			*exvalue = "CA:TRUE";

        ext = X509V3_EXT_conf(NULL, NULL, exname, exvalue);

        X509_add_ext(x509, ext, 0);
        */
        throw std::runtime_error("not impl");
      }
      
      if (X509_sign(x509, signKey ? *signKey : pkey, EVP_sha256()) == 0)
        throw error("X509_sign");

      return x509;
    }



    long TlsInterfaceImpl::bioCallback_s(OpenSSL::BIO_ *b, int oper, const char *argp, int argi, long argl, long retvalue) 
    {
	  using namespace OpenSSL;
      char * cb_arg = BIO_get_callback_arg(b);
      return reinterpret_cast<TlsInterfaceImpl *>(cb_arg)->bioCallback(b, oper, argp, argi, argl, retvalue);      
    }



    

    


    long TlsInterfaceImpl::bioCallback(OpenSSL::BIO_ *, int oper, const char *argp, int argi, long /*argl*/, long retvalue)
    {
      using namespace OpenSSL;

      long retVal;

      if ((BIO_CB_RETURN & oper) != 0)
        retVal = retvalue;
      else
        retVal = 1;

      switch (oper) 
      {
        //case BIO_CB_FREE:
        //  break;
        //case BIO_CB_READ:
        //  break;
        //case BIO_CB_WRITE:
        //  break;
        //case BIO_CB_PUTS:
        //  break;
        //case BIO_CB_GETS:
        //  break;
        //case BIO_CB_CTRL:
        //  break;
        //case BIO_CB_RETURN | BIO_CB_GETS:
        //  break;
        //case BIO_CB_RETURN | BIO_CB_PUTS:
        //  break;
        //case BIO_CB_RETURN | BIO_CB_CTRL:
        //  break;

        case BIO_CB_RETURN | BIO_CB_READ:
          retVal = bioCallback_read((uint8_t *)argp, argi);
          break;

        case BIO_CB_RETURN | BIO_CB_WRITE:
          retVal = bioCallback_write((uint8_t const *)argp, argi);
          break;        
      }
      return retVal;
    }



    void TlsInterfaceImpl::establish(OpenSSL::EVP_PKEY & key, OpenSSL::X509 & certificate)
    {
      using namespace OpenSSL;
      
      if (SSL_library_init() != 1)
        throw std::runtime_error("SSL_library_init() failed");

      SSL_CTX sslCtx{ TLSv1_2_client_method() };

      if (SSL_CTX_use_certificate(sslCtx, certificate) != 1) 
        throw error("SSL_CTX_use_certificate");

      if (SSL_CTX_use_PrivateKey(sslCtx, key) != 1)
        throw error("SSL_CTX_use_PrivateKey");

      if (SSL_CTX_set_cipher_list(sslCtx, m_sslCipherList.c_str()) != 1)
        throw error("SSL_CTX_set_cipher_list");

      //SSL_CTX_set_verify(sslCtx, SSL_VERIFY_FAIL_IF_NO_PEER_CERT|SSL_VERIFY_PEER, &sslVerifyCallback);
      m_ssl.reset(sslCtx);

      OpenSSL::BIO bioRead(BIO_s_null());
      OpenSSL::BIO bioWrite(BIO_s_null());
      
      BIO_set_callback_arg(bioRead , reinterpret_cast<char *>(this));
      BIO_set_callback_arg(bioWrite, reinterpret_cast<char *>(this));
      BIO_set_callback(bioRead , bioCallback_s);
      BIO_set_callback(bioWrite, bioCallback_s);

      SSL_set_bio(m_ssl, bioRead, bioWrite);

      m_sslConnect = SSL_connect(m_ssl);
      if (m_sslConnect <= 0)
        throw error("SSL_connect");      
    }



    void TlsInterfaceImpl::connect2()
    {
      TlsProtocolOOB protocolOOB(*this);      
      protocolOOB.setReset(TlsProtocolOOB::ResetFlag_Connection);

      ProtocolHelper::waitForStatus(protocolOOB, TlsProtocolOOB::OobStatus_Operational);

      OpenSSL::EVP_PKEY key(std::move(createRSAkey()));

      OpenSSL::X509 certificate{ createCertificate(1, "Wacom STU SDK", key, nullptr, nullptr, false, 60 * 60 * 24 * 30) };

      establish(key, certificate);

      ProtocolHelper::waitForStatus(protocolOOB, TlsProtocolOOB::OobStatus_Established);
    }



    OpenSSL::X509 TlsInterfaceImpl::getPeerCertificate()
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      if (m_sslConnect > 0)
      {
        return OpenSSL::SSL_get_peer_certificate(m_ssl);
      }
      else
      {
        throw not_connected_error();
      }
    }



    bool TlsInterfaceImpl::isConnected() const
    {
      return (m_sslConnect > 0);
    }




    InterfaceQueue TlsInterfaceImpl::send(uint8_t const * data, size_t length, InterfaceQueue_tag)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      if (m_sslConnect > 0)
      { 
        if (m_send_debug)
        {
          m_send_debug(data, length);
        }
        
        auto queue(std::move(interfaceQueue()));
        int len = OpenSSL::SSL_write(m_ssl, data, (int)length);        
        if ((size_t)len != length)
        {
          if ((len == k_bioCallback_write_exception || len == -1) && m_bioCallback_write_exception)
          {
            std::rethrow_exception(m_bioCallback_write_exception);
          }
          else
          {
            throw Interface::io_error();
          }
        }

        return std::move(queue);
      }
      else
      {
        throw not_connected_error();
      }
    }



    Report TlsInterfaceImpl::send(uint8_t const * data, size_t length)
    {
      ProtocolHelper::SendHint sendHint;
      sendHint.packetId = (length >= 6) ? static_cast<TlsProtocol::PacketId>((data[4] << 8) | data[5]) : static_cast<TlsProtocol::PacketId>(0);
      sendHint.reportId = (length >= 7 && (sendHint.packetId == TlsProtocol::PacketId_Legacy_GetFeature || sendHint.packetId == TlsProtocol::PacketId_Legacy_SetFeature)) ? static_cast<Protocol::ReportId>(data[6]) : static_cast<Protocol::ReportId>(0);
      
      auto queue(send(data, length, InterfaceQueue_tag()));
      return ProtocolHelper::waitForReturn(queue, m_timeOut, sendHint);
    }



    static void checkReturnValueStatus(Report const & report)
    {
      uint16_t returnValueStatus = TlsProtocol::decodeReport_returnValueStatus(report);
      if (returnValueStatus != TlsProtocol::ReturnValueStatus_Success)
      {
        throw TlsInterfaceImpl::send_error(returnValueStatus);
      }
    }



    void TlsInterfaceImpl::get(uint8_t * /*out*/ data, size_t length)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      if (m_sslConnect > 0)
      {
        if (data && length)
        {
#pragma warning(suppress:4686)
          auto cmd(ProtocolHelper::makeLegacyGetReport(data[0]));

          auto report(send(cmd.data(), cmd.size()));
          
          checkReturnValueStatus(report);

          if (report.size() == length+8)
          {
#ifdef _MSC_VER
            std::copy(report.begin()+8, report.end(), stdext::make_checked_array_iterator(data, length));
#else
            std::copy(report.begin()+8, report.end(), data);
#endif
          }
          else
          {
            // size error
            throw std::runtime_error("report size doesn't match expected get size");
          }
        }
      }
      else 
      {
        throw not_connected_error();
      }
    }



    void TlsInterfaceImpl::set(uint8_t const * data, size_t length)
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      if (m_sslConnect > 0)
      {
        if (data && length)
        {          
          auto cmd(ProtocolHelper::makeLegacySetReport(data, length));

          auto report(send(cmd.data(), cmd.size()));

          checkReturnValueStatus(report);
        }
      }
      else 
      {
        throw not_connected_error();
      }
    }

   

    bool TlsInterfaceImpl::supportsWrite() const 
    {      
      return true;
    }
  
    
    
    void TlsInterfaceImpl::write(uint8_t const * data, size_t length) 
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);
      if (m_sslConnect > 0 && data && length)
      {        
        auto cmd(ProtocolHelper::makeLegacyWrite(data, length));

        auto report(send(cmd.data(), cmd.size()));
        
        checkReturnValueStatus(report);
      }
      else
      {
        throw not_connected_error();
      }
    }



    bool TlsInterfaceImpl::getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const
    {
      std::fill(reportCountLengths.begin(), reportCountLengths.end(), (uint16_t)0);

      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

      if (m_sslConnect > 0)
      {
        Protocol protocol(const_cast<TlsInterfaceImpl &>(*this));
        auto reportSizeCollection(protocol.getReportSizeCollection());

        std::copy(reportSizeCollection.begin(), reportSizeCollection.end(), reportCountLengths.begin());
        return true;
      }
      else
      {
        throw not_connected_error();
      }

      return true;
    }



    std::uint16_t TlsInterfaceImpl::getProductId() const
    {
      lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

      if (isConnectedOOB()) 
      {
        TlsProtocolOOB protocolOOB(const_cast<TlsInterfaceImpl &>(*this));
        auto descriptor(protocolOOB.getDescriptor());

        return descriptor.idProduct;
      }
      else 
      {
        throw not_connected_error();
      }
    }


    

  } // namespace STU
  
} // namespace WacomGSS

