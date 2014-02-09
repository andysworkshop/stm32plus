/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for an ICMP packet arrival
     */

    struct IcmpPacketEvent : NetEventDescriptor {

      /**
       * The IP packet reference
       */

      IpPacket& ipPacket;


      /**
       * The ICMP packet data encapsulated in the above packet
       */

      IcmpPacket& icmpPacket;


      /**
       * Constructor
       * @param icmp The ICMP packet
       * @param ip The IP packet holding the ICMP packet
       */

      IcmpPacketEvent(IcmpPacket& icmp,IpPacket& ip)
        : NetEventDescriptor(NetEventType::ICMP_PACKET),
          ipPacket(ip),
          icmpPacket(icmp) {
      }
    };
  }
}
