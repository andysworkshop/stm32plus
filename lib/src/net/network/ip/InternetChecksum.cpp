/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net.h"


namespace stm32plus {
  namespace net {


    /**
     * Calculate the checksum for a UDP packet
     * @param sourceAddress Our IP address
     * @param destinationAddress The destination IP address
     * @param protocol The internet protocol - which must be UDP for this overload
     * @param nb The netbuffer containing the UDP packet including the header
     */

    void InternetChecksum::calculate(const IpAddress& sourceAddress,const IpAddress& destinationAddress,NetBuffer& nb) {

      PseudoHeader ph;
      uint32_t sum,bufferSize;
      uint16_t temp;
      UdpDatagram *datagram;
      const uint8_t *ptr;

      // the internal buffer is always there and always has the UDP header

      datagram=reinterpret_cast<UdpDatagram *>(nb.getWritePointer());

      // set up the pseudo-header

      ph.sourceAddress=sourceAddress;
      ph.destinationAddress=destinationAddress;
      ph.zero=0;
      ph.protocol=IpProtocol::UDP;
      ph.length=datagram->udp_length;

      // sum the pseudo-header

      sum=0;
      sumit(&ph,sizeof(PseudoHeader),sum);

      // sum the internal buffer - the UDP API guarantees that there will be an internal buffer

      ptr=static_cast<uint8_t *>(nb.getWritePointer());
      bufferSize=nb.getSizeFromWritePointerToEnd();

      sumit(ptr,bufferSize & ~1,sum);

      // if there's one left, get it

      if((bufferSize & 1)!=0) {

        if(nb.getUserBufferSize()) {

          temp=ptr[bufferSize-1];

          ptr=static_cast<const uint8_t *>(nb.getUserBuffer());
          temp=(temp << 8) | *ptr++;

          bufferSize=nb.getUserBufferSize()-1;
          sumit(&temp,2,sum);
        }
      }
      else {
        if(nb.getUserBufferSize()) {
          ptr=static_cast<const uint8_t *>(nb.getUserBuffer());
          bufferSize=nb.getUserBufferSize();
        }
      }

      // if there's a user buffer, sum it

      if(nb.getUserBufferSize())
        sumit(ptr,bufferSize & ~1,sum);

      // take care of any left over byte

      if((bufferSize & 1)!=0)
        sum+=ptr[bufferSize-1];

      while(sum >> 16)
        sum=(sum & 0xFFFF)+(sum >> 16);

      // insert the complement into the datagram checksum

      datagram->udp_checksum=~sum;
    }


    /**
     * Sum up a buffer up to the last even byte
     * @param vptr buffer address
     * @param length length in bytes
     * @param[in,out] sum The running total
     */

    void InternetChecksum::sumit(const void *vptr,uint16_t length,uint32_t& sum) {

      const uint16_t *ptr;

      ptr=reinterpret_cast<const uint16_t *>(vptr);
      length>>=1;

      while(length--) {

        sum+=*ptr++;

        if(sum & 0x80000000)    // if high order bit set, fold
          sum=(sum & 0xFFFF) + (sum >> 16);
      }
    }
  }
}

#endif
