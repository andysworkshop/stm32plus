/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Ethernet frame data structure incorporating the 802.3 SNAP header.
     * This can be cast directly on to a frame memory
     * buffer. Note multi-byte members will be in network byte order
     */

    struct EthernetSnapFrameData {

      MacAddress eth_destinationAddress;        ///< destination MAC
      MacAddress eth_sourceAddress;             ///< source MAC
      uint16_t eth_length;                      ///< frame length
      uint8_t eth_dsap;                         ///< destination SAP
      uint8_t eth_ssap;                         ///< source SAP
      uint8_t eth_control;                      ///< control code
      uint16_t eth_etherType;                   ///< ethertype for ethernet v2
      uint8_t eth_data[41];                     ///< at least 41 bytes including 4 for the FCS
    } __attribute__((packed));
  }
}
