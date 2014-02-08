/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Transmit request event to the ICMP module
     */

    struct IcmpTransmitRequestEvent  : NetEventDescriptor {

      const IcmpPacket& packet;                   ///< the ICMP packet structure (and data following)
      uint16_t packetSize;                        ///< total size of the packet and any following data
      const IpAddress& destinationIpAddress;      ///< destination IP address (will be copied out)
      uint8_t ttl;                                ///< The ttl if to be overriden
      bool succeeded;                             ///< true if the transmit succeeded

      /**
       * Constructor
       * @param pkt The packet to send
       * @param pktSize The total packet size include data
       * @param address The destination address
       * @param overrideTtl The TTL for the packet if it is to be overridden. The default of zero means
       *   use the value configured in the initialisation parameters.
       */

      IcmpTransmitRequestEvent(const IcmpPacket& pkt,
                                uint16_t pktSize,
                                const IpAddress& address,
                                uint8_t overrideTtl=0)
        : NetEventDescriptor(NetEventType::ICMP_TRANSMIT_REQUEST),
          packet(pkt),
          packetSize(pktSize),
          destinationIpAddress(address),
          ttl(overrideTtl),
          succeeded(false) {
      }
    };
  }
}
