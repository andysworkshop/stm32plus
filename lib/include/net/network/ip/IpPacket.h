/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * IP packet information
     */

    struct IpPacket {

      /*
       * Pointers directly into the frame data
       */

      IpPacketHeader *header;           // pointer to the header
      uint32_t headerLength;              // total bytes in the header
      uint8_t *payload;                   // pointer to the payload
      uint16_t payloadLength;             // the size of the payload


      /**
       * Check if this IP packet is fragmented. The packet is fragmented if the
       * MF (more fragments) bit is set in the flags or the fragment offset is
       * non-zero.
       */

      bool isFragmented() const {

        uint16_t flagsAndOffset;

        flagsAndOffset=NetUtil::ntohs(header->ip_hdr_flagsAndOffset);
        return (flagsAndOffset & 0x1fff)!=0 || (flagsAndOffset & 0x2000)!=0;
      }


      /**
       * Check if this is the last of a fragmented sequence. The last fragment has
       * MF == 0 and a non-zero offset
       * @return true if it's the last
       */

      bool isLastFragment() const {

        uint16_t flagsAndOffset;

        flagsAndOffset=NetUtil::ntohs(header->ip_hdr_flagsAndOffset);
        return (flagsAndOffset & 0x1fff)!=0 && (flagsAndOffset & 0x2000)==0;
      }


      /**
       * Check if this packet has the MF flag set
       * @return true if it has MF set
       */

      bool hasMoreFragments() const {

        uint16_t flagsAndOffset;

        flagsAndOffset=NetUtil::ntohs(header->ip_hdr_flagsAndOffset);
        return (flagsAndOffset & 0x2000)!=0;
      }


      /**
       * Get the 16-bit id field used to correlate fragmented packets
       * @return The identifier
       */

      uint16_t getIdentifier() const {
        return NetUtil::ntohs(header->ip_hdr_identification);
      }


      /**
       * Get the offset of this fragmented packet's payload
       * @return The payload offset
       */

      uint16_t getFragmentOffset() const {

        uint16_t headerOffset;

        headerOffset=NetUtil::ntohs(header->ip_hdr_flagsAndOffset) & 0x1FFF;
        headerOffset*=8;

        return headerOffset;
      }
    };
  }
}
