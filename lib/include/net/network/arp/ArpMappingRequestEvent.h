/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Request an ARP mapping (get MAC from IP)
     */

    struct ArpMappingRequestEvent : NetEventDescriptor {

      const IpAddress& ipAddress;       ///< the address to lookup

      MacAddress macAddress;              ///< the returned MAC or nullptr
      bool found;

      ArpMappingRequestEvent(const IpAddress& address)
        : NetEventDescriptor(NetEventType::ARP_MAPPING_REQUEST),
          ipAddress(address),
          found(false) {
      }
    };
  }
}
