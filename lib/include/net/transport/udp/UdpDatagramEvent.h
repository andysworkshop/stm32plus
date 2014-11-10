/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a UDP datagram arrival
     */

    struct UdpDatagramEvent : NetEventDescriptor {

      UdpDatagram& udpDatagram;             ///< the UDP datagram
      IpPacket& ipPacket;                   ///< the underlying IP packet

      bool handled;                         ///< set to true if this datagram was handled

      /**
       * Constructor
       * @param udp The UDP packet structure
       * @param ip The IP packet structure
       */

      UdpDatagramEvent(UdpDatagram& udp,IpPacket& ipp)
        : NetEventDescriptor(NetEventType::UDP_DATAGRAM),
          udpDatagram(udp),
          ipPacket(ipp),
          handled(false) {
      }
    };
  }
}
