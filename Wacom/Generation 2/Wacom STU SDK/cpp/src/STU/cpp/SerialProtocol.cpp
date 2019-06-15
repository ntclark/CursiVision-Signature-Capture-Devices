#include <WacomGSS/STU/SerialProtocol.hpp>
#include <WacomGSS/Crc.hpp>

#include <algorithm>

namespace WacomGSS
{
  namespace STU
  {

    namespace SerialProtocol
    {

      WacomGSS_STU_runtime_error_impl(crc_error, "crc_error")

      constexpr int maxEncodedLength() noexcept
      {
        // packet size is encoded into 13 bits, therefore that is the maximum packet size
        return (1 << 13) - 1;
      }


      std::vector<uint8_t> encodeReport(uint8_t const * begin, uint8_t const * end, bool useCrc)
      {

        int encodedDataLength = SerialProtocol::calcEncodedDataLength(useCrc, end-begin);

        std::vector<uint8_t> encodedData;
        encodedData.reserve(static_cast<unsigned int>(getHeaderLength() + encodedDataLength));

        encodeHeader(encodedData, useCrc, encodedDataLength);
        EncodeData(encodedData, begin, end, useCrc);

        return std::move(encodedData);
      }


      bool decodeHasCrc(uint8_t h1) noexcept
      {
        return (h1 & 0x40) != 0;
      }


      /*
      std::vector<uint8_t> decodeReport(std::vector<uint8_t> const & encodedReport)
      {
        std::vector<uint8_t> decodedReport;

        if (encodedReport.size() >= 2)
        {
          bool hasCrc            = decodeHasCrc(encodedReport[0]);
          int  decodedDataLength = decodeEncodedDataLength(encodedReport[0], encodedReport[1]);

          decodeData(encodedReport.begin() += 2, encodedReport.end(), decodedReport);

          if (static_cast<unsigned>(decodedDataLength) != decodedReport.size())
            throw crc_error(); // not accurate

          if (hasCrc)
          {
            if (!checkCrcAndRemove(decodedReport))
              throw crc_error();
          }
        }

        return std::move(decodedReport);
      }
      */



      constexpr int getHeaderLength() noexcept
      {
        return 2;
      }



      bool isStartReport(uint8_t byte) noexcept
      {
        return (byte & 0x80) != 0;
      }



      int decodeEncodedDataLength(uint8_t h1, uint8_t h2)
      {
        return ((h1 & 0x3f) << 7) | (h2 & 0x7f);
      }



      int calcEncodedDataLength(bool useCrcBool, int length)
      {
        if (length < 0)
          throw std::logic_error("getEncodedLength");

        int useCrc = useCrcBool;

        int lengthWithCrcInBits = (length + (useCrc * 2)) << 3;

        int encodedLength = (lengthWithCrcInBits / 7) + ((lengthWithCrcInBits % 7) != 0);

        if (encodedLength > maxEncodedLength())
          throw std::range_error("getEncodedLength");

        return encodedLength;
      }



      void encodeHeader(Container & encodedData, bool useCrcBool, int encodedLength)
      {
        if (encodedLength < 0 || encodedLength > maxEncodedLength())
          throw std::range_error("encodeHeader");

        int useCrc = useCrcBool;

        encodedData.push_back( static_cast<uint8_t>(0x80 | (useCrc<<6) | ((encodedLength >> 7) & 0x3f)) );
        encodedData.push_back( static_cast<uint8_t>(encodedLength & 0x7f) );
      }



      //0  0[7-1]          0 0000000                [0]>>1
      //1  0[0]   1[7-2]   0 0111111   [0]&01 <<6 | [1]>>2
      //2  1[1-0] 2[7-3]   0 1122222   [1]&03 <<5 | [2]>>3
      //3  2[2-0] 3[7-4]   0 2223333   [2]&07 <<4 | [3]>>4
      //4  3[3-0] 4[7-5]   0 3333444   [3]&0f <<3 | [4]>>5
      //5  4[4-0] 5[7-6]   0 4444455   [4]&1f <<2 | [5]>>6
      //6  5[5-0] 6[7]     0 5555556   [5]&3f <<1 | [6]>>7
      //7  6[6-0]          0 6666666   [6]&7f <<0

      //8  7[7-1]          0 7777777                [7]>>1
      //9  7[7-1]          0 7888888   [7]&01 <<6 | [8]>>2

      void EncodeData::push_back(uint8_t byte)
      {
        switch (m_i)
        {
          case 0: m_container.push_back( static_cast<uint8_t>(                                  (byte>>1) ) ); ++m_i; break;
          case 1: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 6) & 0x7f) | (byte>>2) ) ); ++m_i; break;
          case 2: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 5) & 0x7f) | (byte>>3) ) ); ++m_i; break;
          case 3: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 4) & 0x7f) | (byte>>4) ) ); ++m_i; break;
          case 4: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 3) & 0x7f) | (byte>>5) ) ); ++m_i; break;
          case 5: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 2) & 0x7f) | (byte>>6) ) ); ++m_i; break;
          case 6: m_container.push_back( static_cast<uint8_t>( ((m_previousByte << 1) & 0x7f) | (byte>>7) ) );
                  m_container.push_back( static_cast<uint8_t>( ((byte               ) & 0x7f)             ) ); m_i = 0;
        }
        m_previousByte = byte;
      }



      EncodeData::EncodeData(Container & container, uint8_t const * begin, uint8_t const * end, bool useCrc)
      :
        m_container(container),
        m_i(0),
        m_previousByte(0)
      {
        //std::for_each(begin, end, std::bind1st(std::mem_fn(&EncodeData::push_back), this));
        std::for_each(begin, end, [this](uint8_t data) { push_back(data); });
        if (useCrc)
        {
          uint16_t crc = crc16_ansi(begin, end);
          push_back( static_cast<uint8_t>(crc & 0xff) );
          push_back( static_cast<uint8_t>(crc >> 8  ) );
        }
        if (m_i)
        {
          push_back(0);
        }
      }








    } // namespace SerialProtocol

  } // namespace STU

} // namespace WacomGSS
