/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {

    /**
     * UDP datagram structure. This structure may be cast directly on to an incoming
     * packet which of course means that 16 and 32 bit values are in
     * network byte order.
     */

    struct UdpDatagram {

      uint16_t udp_sourcePort;              ///< sender port number or zero if not used
      uint16_t udp_destinationPort;         ///< destination port number
      uint16_t udp_length;                  ///< overall length including this header
      uint16_t udp_checksum;                ///< checksum
      uint8_t udp_data[1];                  ///< user's data (may be zero but zero length arrays are not allowed)

      static uint32_t getHeaderSize() {
        return sizeof(UdpDatagram)-1;
      }
    } __attribute__((packed));
  }
}
