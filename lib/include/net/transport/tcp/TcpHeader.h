/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Definition of a TCP version 4 header. This is designed to be cast directly
     * to a memory buffer which of course means that 16 and 32 bit values are in
     * network byte order.
     */

    struct TcpHeader {

      uint16_t tcp_sourcePort;              ///< sender port number or zero if not used
      uint16_t tcp_destinationPort;         ///< destination port number
      uint32_t tcp_sequenceNumber;          ///< sequence number of the incoming data
      uint32_t tcp_ackNumber;               ///< acknowledgement number of data received
      uint16_t tcp_headerLengthAndFlags;    ///< 4-bit header length and various flags
      uint16_t tcp_windowSize;              ///< number of bytes willing to receive
      uint16_t tcp_checksum;                ///< checksum
      uint16_t tcp_urgentPointer;           ///< offset from sequence number of last urgent byte


      /**
       * Initialise the header with some values
       * @param localPort,
       * @param remotePort,
       * @param localSequenceNumber
       * @param remoteAckNumber
       * @param localReceiveWindowSize
       * @param options
       */

      void initialise(uint16_t localPort,
                      uint16_t remotePort,
                      uint32_t localSequenceNumber,
                      uint32_t remoteAckNumber,
                      uint16_t localReceiveWindowSize,
                      TcpHeaderFlags options) {

        // set up default options in the header

        tcp_sourcePort=NetUtil::htons(localPort);
        tcp_destinationPort=NetUtil::htons(remotePort);
        tcp_sequenceNumber=NetUtil::htonl(localSequenceNumber);
        tcp_ackNumber=NetUtil::htonl(remoteAckNumber);
        tcp_windowSize=NetUtil::htons(localReceiveWindowSize);
        tcp_headerLengthAndFlags=static_cast<uint16_t>(options) | 0x50;     // 5 words default header size
        tcp_checksum=0;
        tcp_urgentPointer=0;
      }


      /**
       * Set the total size of the header including options
       * @param size The header size in bytes
       */

      void setSize(uint16_t size) {

        // we're manipulating this field in network byte order

        tcp_headerLengthAndFlags=(tcp_headerLengthAndFlags & 0xff0f) | ((size/4) << 4);
      }


      /**
       * Get the total header size
       * @return the total size
       */

      uint16_t getHeaderSize() const {
        return getDataOffset();
      }


      /**
       * Get the offset of the data in bytes
       * @return data offset (20 is minimum)
       */

      uint16_t getDataOffset() const {
        return (NetUtil::ntohs(tcp_headerLengthAndFlags) & 0xf000) >> 10;
      }


      /**
       * Return true if this segment has the SYN flag
       * @return true if it has
       */

      bool hasSyn() const {
        return (tcp_headerLengthAndFlags & TcpHeaderFlags::SYN)!=0;   // takes into account net byte order
      }


      /**
       * Return true if this segment has the ACK flag
       * @return true if it has
       */

      bool hasAck() const {
        return (tcp_headerLengthAndFlags & TcpHeaderFlags::ACK)!=0;   // takes into account net byte order
      }


      /**
       * Return true if this segment has the RST flag
       * @return true if it has
       */

      bool hasRst() const {
        return (tcp_headerLengthAndFlags & TcpHeaderFlags::RST)!=0;   // takes into account net byte order
      }


      /**
       * Return true if this segment has the FIN flag
       * @return true if it has
       */

      bool hasFin() const {
        return (tcp_headerLengthAndFlags & TcpHeaderFlags::FIN)!=0;   // takes into account net byte order
      }


      /**
       * Get the size of a header with no options
       */

      constexpr static uint16_t getNoOptionsHeaderSize() {
        return 20;
      }


      /**
       * Search for an option in the header
       * @param optionKind The option id to search for
       * @return a pointer to the option or nullptr if it's not there
       */

      template<class TOption>
      const TOption *findOption() const {
        return reinterpret_cast<const TOption *>(findOption(TOption::getOptionKind()));
      }


      /**
       * Search for a pointer to an option in the header
       * @param optionKind The option id to search for
       * @return a pointer to the option or nullptr if it's not there
       */

      const void *findOption(TcpOptionKind optionKind) const {

        const uint8_t *ptr,*endptr;

        ptr=reinterpret_cast<const uint8_t *>(this)+getNoOptionsHeaderSize();
        endptr=ptr+getHeaderSize()-getNoOptionsHeaderSize();

        while(ptr!=endptr) {

          if(*ptr==0)       // end of options
            break;
          else if(*ptr==1)  // padding
            ptr++;
          else {

            if(*ptr==optionKind)
              return ptr;
            else
              ptr+=ptr[1];      // step past length
          }
        }

        // not found

        return nullptr;
      }

    } __attribute__((packed));

    // options follow...
  }
}
