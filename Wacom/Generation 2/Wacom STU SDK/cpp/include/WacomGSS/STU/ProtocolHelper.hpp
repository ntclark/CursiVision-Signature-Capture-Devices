/// @file      WacomGSS/STU/ProtocolHelper.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides higher level assistance to using the STU tablet protocol.

#ifndef WacomGSS_STU_ProtocolHelper_hpp
#define WacomGSS_STU_ProtocolHelper_hpp

#include <WacomGSS/STU/Protocol.hpp>

namespace WacomGSS
{
  namespace STU
  {    

    namespace ProtocolHelper
    {

      /// @brief Specifies how to scale an image.
      enum Scale
      {
        Scale_Stretch, ///< Scale to fill tablet display, stretching horizontally or vertically as required
        Scale_Fit,     ///< Scale for best fit while maintaining original aspect ratio. May introduce borders
        Scale_Clip     ///< Maintaining original size but clip and centre. May introduce borders
      };

      /// @brief Specifies the border to use if one is introduced due to scaling.
      enum Border
      {
        Border_Black,
        Border_White
      };

      /// @brief Specifies which portion of an image to use if it is too large and being clipped.
      enum Clip : std::uint8_t
      {
        Clip_Left    = 0x00,  ///< Use left side, clipping from the right
        Clip_Center  = 0x01,  ///< Use centre portion, clipping from left and right
        Clip_Right   = 0x02,  ///< Use right side, clipping from the left

        Clip_Top     = 0x00,  ///< Use top, clipping from the bottom
        Clip_Middle  = 0x10,  ///< Use middle portion, clipping from top and bottom
        Clip_Bottom  = 0x20   ///< Use bottom, clipping from the top
      };

      /// @name Inking support
      /// @{

      const std::uint8_t InkState_isOff   = 0x01;
      const std::uint8_t InkState_isOn    = 0x02;
      const std::uint8_t InkState_isInk   = 0x04;
      const std::uint8_t InkState_isFirst = 0x08;
      const std::uint8_t InkState_isLast  = 0x10;

      /// @brief Describes the state of inking.
      enum InkState : std::uint8_t
      {
        InkState_Up     = InkState_isOff                                              , ///< No ink is being drawn
        InkState_Down   =                InkState_isOn                                , ///< No ink is being drawn
        InkState_Inking =                InkState_isOn|InkState_isInk                 , ///< Ink is being drawn
        InkState_First  =                InkState_isOn|InkState_isInk|InkState_isFirst, ///< This is the first point of ink to draw.
        InkState_Last   = InkState_isOff|                             InkState_isLast   ///< This marks that inking has finished. You should not draw this point.
      };
            
      /// @brief Calculates what the next what the InkState should be given the current state and the provided pressure.
      ///
      /// @param inkState             The ink state of the previous pressure point.
      /// @param rdy                  The new rdy state to be calculated.
      /// @param pressure             The new pressure point to be calculated.
      /// @param inkThreshold         The on and off pressure values for state change.
      ///
      /// @return ink state for the provided pressure and current state.
      InkState getNextInkState(InkState inkState, uint8_t rdy, uint16_t pressure, Protocol::InkThreshold const & inkThreshold) noexcept;

      /// @brief Provides convenient wrapper for getNextInkState()
      class InkingState
      {
        Protocol::InkThreshold m_inkThreshold;
        InkState               m_currentState;

      public:
        /// @brief Default constructor sets inkThreshold to the STU-300/500/520/430/530 factory default value for inkThreshold.
        InkingState() noexcept;

        /// @brief  Sets the ink threshold parameter to use. This is only effective the on the next call to nextState().
        ///
        /// @param inkThreshold   The ink threshold to use. @see Protocol::getInkThreshold or Tablet::getInkThreshold.
        void setInkThreshold(Protocol::InkThreshold const & inkThreshold) noexcept;

        /// @brief  Retrieves the stored ink threshold value.
        ///
        /// @return The ink threshold that is currently in use by this class.
        Protocol::InkThreshold getInkThreshold() const noexcept;

        /// @brief Explicitly sets the current state to the provided value.
        ///
        /// @param inkState    The ink state to set.
        void setState(InkState inkState) noexcept;

        /// @brief Sets the inkState to InkState_Up.
        void clear() noexcept;

        /// @brief Retrieves the current ink state.
        ///
        /// @return The ink current state.
        InkState currentState() const noexcept;

        /// @brief Calculates what the new ink state should be passed on the current inkstate and the provided pressure.
        ///
        /// @return The new ink state.
        InkState nextState(uint8_t rdy, uint16_t pressure) noexcept;
      };
      /// @}



      /// @brief Describes the requested operation direction.
      enum OpDirection : std::uint16_t
      {
        OpDirection_Get = 0x0100,
        OpDirection_Set = 0x0200
      };

      /// @name Tablet status support
      /// @{

      /// @brief Checks if the given statusCode is compatible with a given operation. 
      ///
      /// @param statusCode   StatusCode to check.
      /// @param reportId     ReportId to check.
      /// @param opDirection  Direction of operation.
      ///
      /// @return true if statusCode and reportId are compatible in the given direction.
      bool statusCanSend(std::uint8_t statusCode, std::uint8_t reportId, OpDirection opDirection);

      /// @brief  Suspends the thread until the device is ready to accept the specified reportId, or until the function times out.
      ///
      /// @param protocol             Protcol object representing the device to wait for.
      /// @param reportId             Id of report type you want to send.
      /// @param opDirection          Direction of operation.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      ///
      /// @throws Interface::timeout_error
      Protocol::Status waitForStatusToSend(Protocol & protocol, std::uint8_t reportId, OpDirection opDirection, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @brief Suspends the thread until the device is in the specified state, or until the function times out.
      ///
      /// @param protocol             Protcol object representing the device to wait for.
      /// @param statusCode           Status to wait for.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      ///
      /// @throws Interface::timeout_error
      Protocol::Status waitForStatus(Protocol & protocol, std::uint8_t statusCode, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));
      
      /// @}


      /// @name Encryption support
      /// @{

      /// @brief Checks if prime is valid for use in encryption.
      ///
      /// @param prime Diffie-Hellmann prime number to check.
      ///
      /// @return true if prime is valid for use in encryption.
      bool supportsEncryption(Protocol::DHprime const & prime) noexcept;

      /// @brief Checks if the device supports encryption.
      ///
      /// @param protocol Protcol object representing the device.
      ///
      /// @return true if the device supports encryption.
      bool supportsEncryption(Protocol & protocol);
 
      /// @brief Provides a convenient single to call to send the host key and wait for the device key to be received.
      ///
      /// @param protocol             Protcol object representing the device.
      /// @param hostPublicKey        Host public key to be sent to device.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      ///
      /// @return The public key of the device.
      ///
      /// @remark Polls the device using repeated calls to Protocol::getStatus() and retrieves the key synchronously when reported as ready.
      Protocol::PublicKey setHostPublicKeyAndPollForDevicePublicKey(Protocol & protocol, Protocol::PublicKey const & hostPublicKey, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @brief Provides a convenient single to call to send the host key and wait for the device key to be received.
      /// @deprecated This may fail as the device may not send the PublicKey. Use setHostPublicKeyAndPollForDevicePublicKey() instead.
      WacomGSS_deprecated 
      Protocol::PublicKey setHostPublicKeyAndWaitForDevicePublicKey(Protocol & protocol, Protocol::PublicKey const & hostPublicKey, chrono::milliseconds timeout = chrono::milliseconds(1000));
     
      /// @brief
      ///
      /// @throws Interface::timeout_error
      Protocol::PublicKey waitForDevicePublicKey(InterfaceQueue & interfaceQueue, int millisecondsTimeout = 1250);

      /// @brief Provides a convenient single to request the tablet to generate a key and wait for the device to complete.
      ///
      /// @param protocol             Protcol object representing the device.
      /// @param timeout              Maximum period of time to wait for response.
      ///
      /// @return The encryption status that was returned.
      Protocol::EncryptionStatus generateSymmetricKeyAndWaitForEncryptionStatus(Protocol & protocol, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50), chrono::milliseconds timeout = chrono::milliseconds(1000));

      /// @}

      /// @name Image support
      /// @{

      
      /// @brief  Combines the output of Protocol::getCapability() with the tablet's product id so that encodingFlag can be inferred 
      ///         for tablets that do not support the flag in firmware.
      /// @param  idProduct     #ProductId of the tablet (from UsbDevice::idProduct)
      /// @param  encodingFlag  encoding flag (from Protocol::getCapability())
      /// @return Simulated encodingFlag
      /// @see Protocol::getCapability()
      uint8_t simulateEncodingFlag(uint16_t idProduct, uint8_t encodingFlag = 0x00);

      /// @brief  Provides a simple call to determine whether the tablet supports colour.
      ///
      /// @param encodingFlag   As retrieved from Protocol::getCapability(). We recommend you filter through ProtocolHelper::simulateEncodingFlag() prior to calling this API.
      /// @return true if the tablet supports colour.
      bool encodingFlagSupportsColor(uint8_t encodingFlag) noexcept;
      
      /// @brief Provides a single call to write an image to the device, using the appropriate method according to the encodingMode.
      ///
      /// @param protocol             Protcol object representing the device.
      /// @param encodingMode         A combination of values from the Protocol::EncodingMode enum.
      /// @param maxImageBlockSize    The maximum block size supported by the communications protocol.
      /// @param data                 Pointer to buffer containing image data.
      /// @param length               Length of image data.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      void writeImage(Protocol & protocol, Protocol::EncodingMode encodingMode, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @brief Provides a single call to write a Rom image to the device, using the appropriate method according to the encodingMode.
      ///
      /// @param protocol             Protcol object representing the device.
      /// @param romStartImageData    The encoding mode and image selection data.
      /// @param maxImageBlockSize    The maximum block size supported by the communications protocol.
      /// @param data                 Pointer to buffer containing image data.
      /// @param length               Length of image data.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      void writeRomImage(Protocol & protocol, Protocol::RomStartImageData const & romStartImageData, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @deprecated Superseded by writeImage(Protocol & , Protocol::EncodingMode , uint16_t , uint8_t const * , size_t , int , chrono::milliseconds )
      WacomGSS_deprecated
      void writeImage(Protocol & protocol, Protocol::EncodingMode encodingMode, uint8_t const * data, size_t length, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @deprecated Superseded by writeImage(Protocol & , Protocol::EncodingMode , uint16_t , uint8_t const * , size_t , int , chrono::milliseconds )
      WacomGSS_deprecated 
      void writeImage(Protocol & protocol, uint8_t encodingMode, uint8_t const * data, size_t length, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @brief Provides a single call to write an image to an area of the device, using the appropriate method according to the encodingMode.
      ///
      /// @param protocol             Protcol object representing the device.
      /// @param encodingMode         A combination of values from the Protocol::EncodingMode enum.
      /// @param area                 The rectangle the image will be written to.
      /// @param maxImageBlockSize    The maximum block size supported by the communications protocol.
      /// @param data                 Pointer to buffer containing image data.
      /// @param length               Length of image data.
      /// @param retries              Maximum number of times to retry.
      /// @param sleepBetweenRetries  Period in milliseconds to pause for between each retry.
      void writeImageArea(Protocol & protocol, Protocol::EncodingMode encodingMode, Protocol::Rectangle const & area, uint16_t maxImageBlockSize, uint8_t const * data, size_t length, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @deprecated Superseded by writeImageArea(Protocol & , Protocol::EncodingMode , Protocol::Rectangle const & , uint16_t , uint8_t const * , size_t , int , chrono::milliseconds )
      WacomGSS_deprecated
      void writeImageArea(Protocol & protocol, Protocol::EncodingMode encodingMode, Protocol::Rectangle const & area, uint8_t const * data, size_t length, int retries = 25, chrono::milliseconds sleepBetweenRetries = chrono::milliseconds(50));

      /// @}


      /// @brief Internal class to assist the template class ValidatingInterface.
      class ValidatingInterfaceImpl
      {
        bool                          m_initialized;
        bool                          m_validationSupported;
        std::array<std::uint16_t,256> m_reportCountLengths;

      protected:
        bool                            initialized() const noexcept;
        std::array<std::uint16_t,256> & reportCountLengths() noexcept;
        void                            initialize(bool validationSupported) noexcept;

      public:
        ValidatingInterfaceImpl();

        /// @brief  Checks the given buffer against the reportCountLengths.
        /// @return true if data begins with a ReportId taht has a non-zero length in reportCountLengths which matches length, otherwise the return of the appropriate virtual function.
        bool validate(std::uint8_t const * data, std::size_t length, OpDirection opDirection);

        /// @brief Throws ValidatingInterfaceImpl::feature_not_supported if not overridden.
        virtual bool onFeatureNotSupported(std::uint8_t const * data, std::size_t length, OpDirection opDirection);

        /// @brief Throws feature_length_mismatch if not overridden.
        virtual bool onFeatureLengthMismatch(std::uint8_t const * data, std::size_t length, std::uint16_t expectedLength, OpDirection opDirection);

        WacomGSS_STU_runtime_error_decl(feature_not_supported)
        WacomGSS_STU_runtime_error_decl(feature_length_mismatch)
      };


      /// @brief Provides a wrapper to pre and post check API calls.
      template<class BaseInterface>
      class ValidatingInterface : public BaseInterface, public ValidatingInterfaceImpl
      {
      public:        
        bool validate(std::uint8_t const * data, std::size_t length, OpDirection opDirection)
        {
          if (!ValidatingInterfaceImpl::initialized())
          {
            ValidatingInterfaceImpl::initialize( BaseInterface::getReportCountLengths(ValidatingInterfaceImpl::reportCountLengths()) );
          }
          return ValidatingInterfaceImpl::validate(data, length, opDirection);
        }

        /// @brief Provides additional validation for the get API as well as providing pre- and post-call hooks.        
        void get(std::uint8_t * data, std::size_t length) override
        {
          if (validate(data, length, OpDirection_Get) && preCall(data, length, OpDirection_Get))
          {
            try
            {
              BaseInterface::get(data, length);
            }
            catch (...)
            {
              postCallException(data, length, OpDirection_Get);
              throw;
            }
            postCall(data, length, OpDirection_Get);
          }
        }

        /// @brief Provides additional validation for the set API as well as providing pre- and post-call hooks.        
        void set(std::uint8_t const * data, std::size_t length) override
        {
          if (validate(data, length, OpDirection_Set) && preCall(data, length, OpDirection_Set))
          {
            try
            {
              BaseInterface::set(data, length);
            }
            catch (...)
            {
              postCallException(data, length, OpDirection_Set);
              throw;
            }
            postCall(data, length, OpDirection_Set);
          }
        }

        /// @brief    Called prior to get() or set().
        /// @returns  true to continue the operation or false to ignore the call.
        virtual bool preCall(std::uint8_t const * , std::size_t , OpDirection )
        {
          return true;
        }


        virtual void postCall(std::uint8_t const * , std::size_t , OpDirection )
        {
        }

        virtual void postCallException(std::uint8_t const * , std::size_t , OpDirection )
        {
        }
      };


      namespace ostream_operators
      {
        std::ostream & operator << (std::ostream &, OpDirection);                     ///< insertion opertator for OpDirection
        std::ostream & operator << (std::ostream &, Protocol::StatusCode);            ///< insertion opertator for Protocol::StatusCode
        std::ostream & operator << (std::ostream &, Protocol::ErrorCode);             ///< insertion opertator for Protocol::ErrorCode
        std::ostream & operator << (std::ostream &, Protocol::ResetFlag);             ///< insertion opertator for Protocol::ResetFlag
        std::ostream & operator << (std::ostream &, Protocol::ReportId);              ///< insertion opertator for Protocol::ReportId
        std::ostream & operator << (std::ostream &, Protocol::InkingMode);            ///< insertion opertator for Protocol::InkingMode
        std::ostream & operator << (std::ostream &, Protocol::EncodingMode);          ///< insertion opertator for Protocol::EncodingMode
        std::ostream & operator << (std::ostream &, Protocol::DefaultMode);           ///< insertion opertator for Protocol::DefaultMode
        std::ostream & operator << (std::ostream &, Protocol::RenderingMode);         ///< insertion opertator for Protocol::RenderingMode
        std::ostream & operator << (std::ostream &, Protocol::PenDataOptionMode);     ///< insertion opertator for Protocol::PenDataOptionMode
        std::ostream & operator << (std::ostream &, Protocol::EndImageDataFlag);      ///< insertion opertator for Protocol::EndImageDataFlag
        std::ostream & operator << (std::ostream &, Protocol::OperationModeType);     ///< insertion opertator for Protocol::OperationModeType        
        std::ostream & operator << (std::ostream &, Protocol::RomImageDeleteMode);    ///< insertion opertator for Protocol::RomImageDeleteMode
        std::ostream & operator << (std::ostream &, Protocol::StatusCodeRSA);         ///< insertion opertator for Protocol::StatusCodeRSA
        std::ostream & operator << (std::ostream &, Protocol::ErrorCodeRSA);          ///< insertion opertator for Protocol::ErrorCodeRSA
        std::ostream & operator << (std::ostream &, Protocol::SymmetricKeyType);      ///< insertion opertator for Protocol::SymmetricKeyType
        std::ostream & operator << (std::ostream &, Protocol::AsymmetricKeyType);     ///< insertion opertator for Protocol::AsymmetricKeyType
        std::ostream & operator << (std::ostream &, Protocol::AsymmetricPaddingType); ///< insertion opertator for Protocol::AsymmetricPaddingType        
      }

    } // namespace ProtocolHelper

  } // namespace STU

} // namespace WacomGSS

#if defined(WacomGSS_WIN32)
#include <WacomGSS/STU/Win32/ProtocolHelper.hpp>
#endif

#endif // WacomGSS_STU_ProtocolHelper_hpp
