/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Ethernet frame data structure. This can be cast directly on to a frame memory
     * buffer. Note multi-byte members will be in network byte order
     */

    struct EthernetFrameData {

      MacAddress eth_destinationAddress;        ///< destination MAC
      MacAddress eth_sourceAddress;             ///< source MAC
      uint16_t eth_etherType;                   ///< ethertype for ethernet v2
      uint8_t eth_data[46];                     ///< at least 46 bytes, plus 4 for the FCS
    } __attribute__((packed));
  }
}
