/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Ethernet frame information
     */

    struct EthernetFrame : DatalinkFrame {

      /**
       * Posible values for the flags
       */

      enum {
        FLAG_IP  = 0x1,     //!< FLAG_IP
        FLAG_IP6  = 0x2,    //!< FLAG_IP6
        FLAG_UDP  = 0x4,    //!< FLAG_UDP
        FLAG_TCP  = 0x8,    //!< FLAG_TCP
        FLAG_ICMP = 0x10    //!< FLAG_ICMP
      };

      uint32_t flags;               //!< misc flags about the frame detected by the ST MAC
      MacAddress *sourceMac;        // source MAC address
      MacAddress *destinationMac;   // destination MAC address

      /**
       * Constructor, reset the flags
       */

      EthernetFrame() :
        flags(0) {
      }
    };
  }
}
