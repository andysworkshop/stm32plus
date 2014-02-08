/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event that is sent when a TCP segment is received from the IP module
     */

    struct TcpSegmentEvent : NetEventDescriptor {

      IpPacket& ipPacket;               ///< the original IP packet
      TcpHeader& tcpHeader;             ///< the TCP header structure
      uint8_t *payload;                   ///< pointer to the payload
      uint16_t payloadLength;             ///< the size of the payload
      uint16_t sourcePort;                ///< taken from the header and converted to host order
      uint16_t destinationPort;           ///< taken from the header and converted to host order
      bool handled;                       ///< set if any connection or server recognises this segment

      TcpSegmentEvent(IpPacket& packet,TcpHeader& header,uint8_t *data,uint16_t datalen,uint16_t sPort,uint16_t dPort)
        : NetEventDescriptor(NetEventType::TCP_SEGMENT),
          ipPacket(packet),
          tcpHeader(header),
          payload(data),
          payloadLength(datalen),
          sourcePort(sPort),
          destinationPort(dPort),
          handled(false) {
      }
    };
  }
}
