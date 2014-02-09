/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/net.h"


namespace stm32plus {
  namespace net {


    /**
     * Read event for a PHY. Sent as a notification by the physical layer PHY and
     * actioned in the datalink layer by the MAC. The interface to a PHY is via
     * 16-bit registers.
     */

    struct PhyReadRequestEvent : NetEventDescriptor {

      uint16_t phyAddress;          ///< The station id of the PHY
      uint16_t regNumber;           ///< The register number to read
      uint32_t timeoutMillis;       ///< How long to wait in ms

      bool succeeded;               ///< set by the link layer to true if able to read
      uint16_t result;              ///< the result of the read if it was successful


      /**
       * Constructor
       */

      PhyReadRequestEvent(uint16_t address,uint16_t rnum,uint32_t timeout)
        : NetEventDescriptor(NetEventType::PHY_READ_REQUEST),
          phyAddress(address),
          regNumber(rnum),
          timeoutMillis(timeout) {

        succeeded=false;
      }
    };
  }
}
