/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Utility to create and initialise an ICMP error type
     * @param icmpCode The ICMP code corresponding to the error
     * @param ipPacket The IP packet that caused this error
     * @param[out] The size of the allocated packet
     * @return A pointer to the ICMP packet (free it with free() )
     */

    template<class T>
    T *createIcmpErrorPacket(IcmpCode icmpCode,const IpPacket& ipPacket,uint16_t& packetSize) {

      T *packet;
      uint16_t dataSize;

      // up to 8 bytes of the payload go back in an error packet

      dataSize=std::min(static_cast<uint16_t>(8),ipPacket.payloadLength);

      // allocate space for it

      packetSize=T::getHeaderSize()+ipPacket.headerLength+dataSize;
      packet=reinterpret_cast<T *>(malloc(packetSize));

      // initialise it

      packet->initialise(icmpCode);

      // copy the IP header and data fragment into place

      memcpy(reinterpret_cast<uint8_t *>(packet)+T::getHeaderSize(),
             ipPacket.header,
             ipPacket.headerLength+dataSize);

      // ready

      return packet;
    }
  }
}
