/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {

    /**
     * Notify about an association between a MAC and an IP
     */

    struct IpAddressMappingEvent : NetEventDescriptor {

      const MacAddress& macAddress;
      const IpAddress& ipAddress;

      IpAddressMappingEvent(const MacAddress& mac,const IpAddress& ip)
        : NetEventDescriptor(NetEventType::IP_ADDRESS_MAPPING),
          macAddress(mac),
          ipAddress(ip) {
      }
    };
  }
}
