/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor that announces a new set of DNS servers (up to 3)
     */

    struct IpDnsServersAnnouncementEvent : NetEventDescriptor {

      /**
       * Up to 3 DNS servers, call isValid() to find the end of the set if less than 3
       */

      const IpAddress *ipDnsServers;


      /**
       * Constructor
       */

      IpDnsServersAnnouncementEvent(const IpAddress *dnsServers)
        : NetEventDescriptor(NetEventType::DNS_SERVERS_ANNOUNCEMENT),
          ipDnsServers(dnsServers) {
      }
    };
  }
}
