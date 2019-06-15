/// @file      WacomGSS/STU/SerialProtocol.hpp
/// @copyright Copyright (c) 2011 Wacom Company Limited
/// @author    mholden
/// @date      2011-10-18
/// @brief     Provides abstraction of serial-specific protocol

#ifndef WacomGSS_STU_SerialProtocol_hpp
#define WacomGSS_STU_SerialProtocol_hpp

#include <WacomGSS/STU/Error.hpp>
#include <WacomGSS/noncopyable.hpp>
#include <WacomGSS/Crc.hpp>
#include <vector>
#include <functional>

namespace WacomGSS
{
  namespace STU
  {    


    namespace SerialProtocol
    {

      static const uint8_t ReportId_GetReport  = 0x80; // SERIAL ONLY
      static const uint8_t ReportId_SetResult  = 0x81; // SERIAL ONLY
      static const uint8_t Error_None          = 0x00;
       
      /// Thrown when a CRC check fails
      WacomGSS_STU_runtime_error_decl(crc_error)

      /// Returns the length of the Serial header
      constexpr int getHeaderLength() noexcept;

      /// The start of a serial packet as the top bit set. This returns true if it is set.
      bool isStartReport(uint8_t byte) noexcept;

      /// Returns true if the 'has CRC' bit is set.
      bool decodeHasCrc(uint8_t header1) noexcept;

      /// Decodes the data length bits in the header.
      int decodeEncodedDataLength(uint8_t header1, uint8_t header2);

      /// Returns the maximum size the encoded data can be.
      constexpr int maxEncodedDataLength();

      /// Calculates the encoded size from the raw data size.
      int calcEncodedDataLength(bool useCrc, int length);

      typedef std::vector<uint8_t> Container;

      void encodeHeader(Container & encodedData, bool useCrc, int encodedDataLength);

      std::vector<uint8_t> encodeReport(uint8_t const * begin, uint8_t const * end, bool useCrc);


      //0  0[7-1]          0 0000000                [0]>>1
      //1  0[0]   1[7-2]   0 0111111   [0]&01 <<6 | [1]>>2
      //2  1[1-0] 2[7-3]   0 1122222   [1]&03 <<5 | [2]>>3
      //3  2[2-0] 3[7-4]   0 2223333   [2]&07 <<4 | [3]>>4
      //4  3[3-0] 4[7-5]   0 3333444   [3]&0f <<3 | [4]>>5
      //5  4[4-0] 5[7-6]   0 4444455   [4]&1f <<2 | [5]>>6
      //6  5[5-0] 6[7]     0 5555556   [5]&3f <<1 | [6]>>7
      //7  6[6-0]          0 6666666   [6]&7f <<0 
      //
      //8  7[7-1]          0 7777777                [7]>>1
      //9  7[7-1]          0 7888888   [7]&01 <<6 | [8]>>2

      class EncodeData : noncopyable
      {
        Container & m_container;
        int         m_i;
        uint8_t     m_previousByte;
  
        void push_back(uint8_t byte);
  
      public:
        EncodeData(Container & container, uint8_t const * begin, uint8_t const * end, bool useCrc);
      };
  
  
      template<class ConstIterator, class Container>
      class DecodeData : noncopyable
      {
        Container & m_container;
        int         m_i;
        uint8_t     m_previousByte;
  
        void push_back(uint8_t byte)
        {
          switch (m_i)
          {
            case 0:                                                                                                         ++m_i; break;
            case 1: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 1) | ((byte >> 6) & 0x01)) & 0xff ) ); ++m_i; break;
            case 2: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 2) | ((byte >> 5) & 0x03)) & 0xff ) ); ++m_i; break;
            case 3: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 3) | ((byte >> 4) & 0x07)) & 0xff ) ); ++m_i; break;
            case 4: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 4) | ((byte >> 3) & 0x0f)) & 0xff ) ); ++m_i; break;
            case 5: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 5) | ((byte >> 2) & 0x1f)) & 0xff ) ); ++m_i; break;
            case 6: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 6) | ((byte >> 1) & 0x3f)) & 0xff ) ); ++m_i; break;
            case 7: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 7) | ((byte     ) & 0x7f)) & 0xff ) ); m_i = 0;
          }
          m_previousByte = byte;
        }
          
      public:
        DecodeData(ConstIterator begin, ConstIterator end, Container & container)
        :
          m_container(container),
          m_i(0),
          m_previousByte(0)
        {
          //std::for_each(begin, end, std::bind(std::mem_fn(&DecodeData::push_back), this));
          std::for_each(begin, end, [this](uint8_t data) { push_back(data); });
        }  
      };

      template<class ConstIterator, class Container>
      inline void decodeData(ConstIterator begin, ConstIterator end, Container & container)
      {
        DecodeData<ConstIterator,Container>(begin, end, container);
      }



      template<class Container>
      bool checkCrcAndRemove(Container & container)
      {
        bool crcValid = false;

        auto size = container.size();
        if (size >= 2)
        {
          uint16_t storedCrc = static_cast<uint16_t>(container[size-2] | (static_cast<uint16_t>(container[size-1]) << 8));
          container.pop_back();
          container.pop_back();

          uint16_t calculatedCrc = crc16_ansi(container.begin(), container.end());

          crcValid = (calculatedCrc == storedCrc);
        }
        return crcValid;
      }




    } // namespace SerialProtocol


  } // namespace STU

} // namespace WacomGSS

#endif // WacomGSS_STU_SerialProtocol_h
