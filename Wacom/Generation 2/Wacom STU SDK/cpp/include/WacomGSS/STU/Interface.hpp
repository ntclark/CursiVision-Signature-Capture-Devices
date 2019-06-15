/// @file      WacomGSS/STU/Interface.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Definition of the WacomGSS::STU::Interface class


#ifndef WacomGSS_STU_Interface_hpp
#define WacomGSS_STU_Interface_hpp

#include <WacomGSS/STU/InterfaceQueue.hpp>
#include <WacomGSS/STU/Error.hpp>
#include <WacomGSS/OpenSSL.hpp>
#include <array>
#include <functional>

namespace WacomGSS
{
  namespace STU
  {

    /// @brief The base interface to communicate with %STU tablets.
    ///
    /// The class Inteface provides a virtual base class to abstract the
    /// low-level mechanism of transferring data to and from the tablet.
    /// Derived classes, such as UsbInterface and SerialInterface provide
    /// a concrete implementation and will have their own version of a connect()
    /// function in order to establish communication. 
    ///
    /// It is beyond the scope of this document to enumerate available tablets.
    ///
    /// In case of errors, exceptions are thrown. An exception is typically fatal
    /// for a connection and all that can be done is to delete the object.
    /// All methods may throw an exception unless explicitly marked noexcept; 
    /// only notable exceptions are specifically mentioned in the documentation.
    class Interface : public noncopyable
    {
      std::shared_ptr<InterfaceImpl> m_interfaceData;      

    protected:
      std::function<void (Report const & report, size_t numberofQueues)> m_queueReport_debug;

      /// @brief Adds a report to the base and all additional queues.
      void queueReport(Report && report);

      /// @brief Sets an exception value to the base and all additional queues.
      bool queueException(std::exception_ptr && ex) noexcept;

      /// @brief Clears all pending reports and exceptions from the the base and all additional queues.
      void queueClear();
      
    public:
      /// @brief Default constructor.
      Interface();


      /// @brief Virtual destructor.
      virtual ~Interface() = 0;

      // connect(...) Derived-class, implementation-dependent to connect the Interface to a device.
      //virtual std::error_code connect(...) = 0; // interface dependent


      /// @brief Disconnects the interface from the device, freeing resources.
      virtual void disconnect() = 0;


      /// @brief Reports if the Interface is connected to a device.
      ///
      /// @return true if a connection has been established.
      ///
      /// @remark This does not report if the object is in an error condition.
      virtual bool isConnected() const = 0;



      /// @brief Creates an interface queue bound to this interface.
      ///
      /// @return a new InterfaceQueue object.  
      ///
      /// @remark All queues receive the same data. It is up to you to process only the reports that you accept and ignore others. 
      /// @remark Calls to get() and set() may generate reports (particularly SerialInterface); be prepared to ignore reports that you are not concerned with.
      InterfaceQueue interfaceQueue();

      /// @brief Notifies all InterfacesQueues bound to this interface to check their predicate.
      ///
      /// @remark This is typically used during the close down sequence when an InterfaceQueue on a different thread is waiting on input and
      ///         you are shutting down this object.
      void queueNotifyAll();

      /// @brief Sets the predicate of all InterfacesQueues bound to this interface.
      /// @remark This is typically used during the close down sequence when an InterfaceQueue on a different thread is waiting on input and
      ///         you are shutting down this object.
      void queueSetPredicateAll(bool predicate);
      
      /// @brief Sends a request for a given Report to the tablet.
      ///
      /// @param data pointer to buffer to receive the report.
      /// @param length the size of the buffer pointed to by data.
      ///
      /// @remark The first byte of the data array must be the ReportId for the report you want to retrieve;
      ///         length must be the required size for the given report.
      /// @remark This function blocks until the tablet reports it has received the message, or a timeout occurs.
      ///
      /// @exception std::exception
      virtual void get(std::uint8_t * data, std::size_t length) = 0;

      /// @copydoc Interface::get(std::uint8_t *,std::size_t)
      template<size_t N> void get(std::array<uint8_t,N> & data)
      {
        get(data.data(), data.size());
      }



      /// @brief Sends a Report message to the tablet.
      ///
      /// @param data pointer to buffer containing the report to send.
      /// @param length the size of the buffer pointed to by data.
      ///
      /// @remark The first byte of the data array must be the report id; 
      ///         length must be the required size for the given report.
      /// @remark This function blocks until the tablet reports it has received the message, or a timeout occurs.
      ///
      /// @exception Interface::set_error
      /// @exception std::exception
      virtual void set(std::uint8_t const * data, std::size_t length) = 0;

      /// @copydoc Interface::set(std::uint8_t const *,std::size_t)
      template<size_t N>
      void set(std::array<uint8_t,N> const & data)
      {
        set(data.data(), data.size());
      }

      /// @brief Reports if the device supports bulk transfer.
      ///
      /// @return true if the device supports bulk transfer.
      ///
      /// @remark The return value is invariant while the object is connected.
      virtual bool supportsWrite() const = 0;


      /// @brief Sends a bulk transfer to the device.
      ///
      /// @param data pointer to buffer to send.
      /// @param length the size of the buffer pointed to by data.
      ///
      /// @remark This function blocks until the tablet reports it has received the message, or a timeout occurs.
      ///
      /// @exception std::exception
      virtual void write(std::uint8_t const * data, std::size_t length) = 0;


      /// @brief  Retrieves the expected lengths of valid reports.
      ///
      /// @param  reportCountLengths reference to an array that contains the length of each ReportId.
      ///
      /// @return true if the array is has been filled with valid data.
      ///
      /// @remark If the interface cannot provide this information, the function returns false and the 
      ///         contents of reportCountsLengths is left undefined. Otherwise, each position in the 
      ///         array is the length of the corresponding ReportId, or 0 if it is not supported.
      ///         <p>
      ///         The interface must be connected to retrieve this information.
      virtual bool getReportCountLengths(std::array<std::uint16_t, 256> & reportCountLengths) const;


      /// @brief  Retrieves the idProduct of the tablet.
      ///
      /// @return returns the USB/HID identifier.
      ///
      /// @remark The interface must be connected to retrieve this information.      
      virtual std::uint16_t getProductId() const = 0;
    

      /// @brief Thrown if an IO api is called when the object is not connected to a tablet.
      WacomGSS_STU_runtime_error_decl( not_connected_error       ) ;


      /// @brief  Typically thrown when the device which was connected is unexpectly removed.
      WacomGSS_STU_runtime_error_decl( device_removed_error      ) ;


      /// @brief  Thrown when a call to write() is made on an object which does not support write.
      ///
      /// You can avoid this exception by calling supportsWrite() prior to using write().
      WacomGSS_STU_runtime_error_decl( write_not_supported_error ) ;


      /// @brief  Thrown when a low-level I/O error occurs.
      ///
      /// This is a fatal error and it is not expected that the object can
      /// continue communication with the device. The only sensible call
      /// that can be made is to disconnect().
      WacomGSS_STU_runtime_error_decl( io_error                  ) ; // serial only?


      /// @brief  Thrown when an I/O error has occured that cannot be retried within the allowed
      ///         time limit.
      ///
      /// This can most often happen on serial devices where CRC errors occur.
      WacomGSS_STU_runtime_error_decl( timeout_error             ) ;// serial only?


      /// @brief  Thrown when the tablet responds to the data sent by set() with an error code.
      ///
      /// This can occur on serial communications.
      class set_error : public std::runtime_error // serial only
      {
        std::uint8_t m_value;

        static char const * s_message;

      public:
        set_error(std::uint8_t value)
        :
          std::runtime_error(s_message),
          m_value(value)
        {
        }

        std::uint8_t value() const noexcept
        {
          return m_value;
        }

        static char const * set_message(char const * m);
      };


      /// @brief  Thrown when the tablet responds to the data sent by send() with an error code.
      ///
      /// This is only for TLS connections
      class send_error : public std::runtime_error
      {
        std::uint16_t m_value;    ///< the error value
        std::uint16_t m_packetId; ///< TLS protocol
        std::uint8_t  m_reportId; ///< legacy protocol (if applicable)

        static char const * s_message;

      public:
        /// @brief constructs setting all values
        send_error(std::uint16_t value, std::uint16_t packetId, std::uint8_t reportId)
          :
          std::runtime_error(s_message),
          m_value(value),
          m_packetId(packetId),
          m_reportId(reportId)
        {
        }

        /// @brief constructs setting just the error value. This used when sending an unrecognised command
        send_error(std::uint16_t value)
        :
          send_error(value, 0, 0)
        {
        }

        /// @brief accessor for error value.
        std::uint16_t value() const noexcept
        {
          return m_value;
        }

        /// @brief accessor for native packet id.
        std::uint16_t packetId() const noexcept
        {
          return m_packetId;
        }

        /// @brief accessor for legacy packet id.
        std::uint8_t reportId() const noexcept
        {
          return m_reportId;
        }

        static char const * set_message(char const * m);
      };

    };



    /// @brief The pure interface for communication with TLS tablets.
    ///
    /// Tablets that support TLS connections require new set of communication calls. 
    /// There are out-of-band (unencrypted) calls to perform hard reset or generic queries.
    /// The primary call is send() which sends data over the encrypted connection.
    /// The inherited calls to get() and set() are emulated using the legacy command set.
    ///
    /// @note In order to use the legacy command set, you must set the ProtocolLevel
    ///       to the correct value.
    class InterfaceTLS : public Interface
    {
    public:
      /// @brief allows the creation of a connection that supports only OOB commands, or
      /// negiotates the full SSL session. Typically SSL is used.
      enum ConnectOption
      {
        ConnectOption_OOB = 0x1,
        ConnectOption_SSL = 0x2
      };

      /// @brief provides the certificate that the tablet has provided for the SSL connection.
      virtual OpenSSL::X509 getPeerCertificate() = 0;

      /// @brief Reports if the Interface is connected to a device.
      ///
      /// @return true if any connection has been established. If connected, OOB commands are always allowed.
      ///
      /// @remark This does not report if the object is in an error condition.
      virtual bool isConnectedOOB() const = 0;


      /// @brief Sends a request for a given OOB report to the tablet.
      ///
      /// @param data pointer to buffer to receive the report.
      /// @param length the size of the buffer pointed to by data.
      ///
      /// @remark The first byte of the data array must be the OOB Report Id for the report you want to retrieve;
      ///         length must be the required size for the given report.
      /// @remark This function blocks until the tablet reports it has received the message, or a timeout occurs.
      ///
      /// @exception std::exception
      virtual void getOOB(uint8_t * data, size_t length) = 0;

      /// @brief Sends an OOB report message to the tablet.
      ///
      /// @param data pointer to buffer containing the report to send.
      /// @param length the size of the buffer pointed to by data.
      ///
      /// @remark The first byte of the data array must be the report id; 
      ///         length must be the required size for the given report.
      /// @remark This function blocks until the tablet reports it has received the message, or a timeout occurs.
      ///
      /// @exception Interface::set_error
      /// @exception std::exception
      virtual void setOOB(uint8_t const * data, size_t length) = 0;

      /// @brief Empty struct in order to select the correct overload.
      struct InterfaceQueue_tag {};

      /// @brief The canonical call to send data to the tablet.
      ///
      /// Use this when the send results in the tablet producing multiple return packets, for example,
      /// to indicate calculation progress. Where a send results in a single return packet, it is much
      /// simpler to use the other send API (without the InterfaceQueue_tag).
      /// 
      /// @param data pointer to buffer containing the report to send.
      /// @param length the size of the buffer pointed to by data.
      /// @param InterfaceQueue_tag used select that an InterfaceQueue is returned, not just a single report.
      ///
      /// @return returns a queue that captures all reports that are returned subsequent to the send.
      ///
      virtual InterfaceQueue send(uint8_t const * data, size_t length, InterfaceQueue_tag) = 0;

      /// @brief The primary call to send data to the tablet.
      ///
      /// Use when a single return value is expected, which is the most typical case. Where multiple
      /// values are returned, use the InterfaceQueue_tag version.
      ///
      /// @param data pointer to buffer containing the report to send.
      /// @param length the size of the buffer pointed to by data.
      ///
      /// @return returns a queue that captures all reports that are returned subsequent to the send.
      virtual Report send(uint8_t const * data, size_t length) = 0;


      /// @copydoc InterfaceTLS::setOOB(std::uint8_t const *,std::size_t)
      template<size_t N> void setOOB(std::array<uint8_t, N> const & data)
      {
        setOOB(data.data(), data.size());
      }

      /// @copydoc InterfaceTLS::getOOB(std::uint8_t *,std::size_t)
      template<size_t N> void getOOB(std::array<uint8_t, N> & data)
      {
        getOOB(data.data(), data.size());
      }

      /// @copydoc InterfaceTLS::send(std::uint8_t const *,std::size_t)
      template<size_t N> Report send(std::array<uint8_t, N> const & data)
      {
        return send(data.data(), data.size());
      }

    };
  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_Interface_hpp
