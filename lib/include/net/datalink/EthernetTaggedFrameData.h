/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Ethernet frame data structure with Qtag. This can be cast directly on to a frame memory
     * buffer. Note multi-byte members will be in network byte order
     */

    struct EthernetTaggedFrameData {

      MacAddress eth_destinationAddress;        ///< destination MAC
      MacAddress eth_sourceAddress;             ///< source MAC
      uint16_t eth_qtagType;                    ///< qtag type field
      uint16_t eth_qtagControlInfo;             ///< qtag control information
      uint16_t eth_etherType;                   ///< ethertype for ethernet v2
      uint8_t eth_data[42];                     ///< at least 42 bytes, plus 4 for the FCS
    } __attribute__((packed));
  }
}
