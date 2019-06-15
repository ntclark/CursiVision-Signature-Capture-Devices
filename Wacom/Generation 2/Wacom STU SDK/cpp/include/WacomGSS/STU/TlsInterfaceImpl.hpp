/// @file      WacomGSS/STU/TlsInterfaceImpl.hpp
/// @copyright Copyright (c) 2016 Wacom Company Limited
/// @author    mholden
/// @date      2016-11-17
/// @brief     Definition of the WacomGSS::STU::TlsInterfaceImpl class

#ifndef WacomGSS_STU_TlsInterfaceImpl_hpp
#define WacomGSS_STU_TlsInterfaceImpl_hpp

#include <WacomGSS/STU/Interface.hpp>
#include <WacomGSS/compatibility/mutex.hpp>
#include <WacomGSS/compatibility/thread.hpp>
#include <WacomGSS/compatibility/atomic.hpp>


namespace WacomGSS
{

  namespace STU
  {
    

    /// @briefs Provides an OpenSSL implementation of the TLS interface, allowing for operating system specific code.
    class TlsInterfaceImpl : public InterfaceTLS
    {
    protected:
      static const long k_bioCallback_read_exception = -80808080;
      static const long k_bioCallback_write_exception = -81818181;

      OpenSSL::Loader         m_sslLoader;

      std::uint8_t            m_outPipeId;
      std::uint8_t            m_inPipeId;
      chrono::milliseconds    m_timeOut;
      std::string             m_sslCipherList;

      OpenSSL::SSL            m_ssl;
      int                     m_sslConnect; // return from ssl_connect
      recursive_mutex mutable m_apiMutex;

      std::exception_ptr      m_bioCallback_write_exception;

      thread                  m_readerThread;
      std::atomic_bool        m_readerThreadRunning;
      std::exception_ptr      m_bioCallback_read_exception;

      std::function<void(uint8_t const *, size_t)> m_send_debug;


      virtual void readerThread() noexcept = 0;
      virtual void stopReaderThread() = 0;
      virtual long bioCallback_read(uint8_t * buffer, long length) = 0;
      virtual long bioCallback_write(uint8_t const * buffer, long length) = 0;

      void processReadBuffer(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end);
      void startReaderThread();

      static long bioCallback_s(OpenSSL::BIO_ *b, int oper, const char *argp, int argi, long argl, long retvalue);
      long bioCallback(OpenSSL::BIO_ *, int oper, const char *argp, int argi, long /*argl*/, long retvalue);
      void establish(OpenSSL::EVP_PKEY & key, OpenSSL::X509 & certificate);
      void connect2();

      TlsInterfaceImpl();

    public:
      //void disconnect() override; // OS specific

      bool isConnected() const override;

      void get(uint8_t * data, size_t length) override;

      void set(uint8_t const * data, size_t length) override;

      bool supportsWrite() const override;
  
      void write(uint8_t const * data, size_t length) override;

      bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const override;

      std::uint16_t getProductId() const override;


      OpenSSL::X509 getPeerCertificate() override;
      //bool isConnectedOOB() const override; // OS specific
      //void setOOB(uint8_t const * data, size_t length) override; // OS specific
      //void getOOB(uint8_t * data, size_t length) override; // OS specific
      
      Report send(uint8_t const * data, size_t length) override;
            
      InterfaceQueue send(uint8_t const * data, size_t length, InterfaceQueue_tag) override;

      /// @brief Allows for a debug hook to be added to see sends and receives.
      /// @see class TlsInterface_Debug as an example of how to implement.
      template<class T>
      void setDebug(T & t)
      {
        lock_guard<decltype(m_apiMutex)> lock(m_apiMutex);

        m_queueReport_debug = t;
        m_send_debug        = t;
      }

    };



  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_TlsInterfaceImpl_hpp
