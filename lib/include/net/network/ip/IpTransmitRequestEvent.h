/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Transmit request event to the IP layer
     */

    struct IpTransmitRequestEvent  : NetEventDescriptor {

      NetBuffer *networkBuffer;                   ///< buffer to send
      const IpAddress& destinationIpAddress;      ///< destination IP address (will be copied out)
      IpProtocol protocol;                        ///< protocol number for the request
      uint8_t ttl;                                ///< The ttl if to be overriden

      bool succeeded;                             ///< set to true if the send succeeded

      /**
       * Constructor
       * @param nb The buffer pointer to send (the receive owns it)
       * @param address The destination address
       * @param proto The IP protocol encapsulated here
       * @param overrideTtl The TTL for the packet if it is to be overridden. The default of zero means
       * use the value configured in the initialisation parameters.
       */

      IpTransmitRequestEvent(NetBuffer *nb,const IpAddress& address,IpProtocol proto,uint8_t overrideTtl=0)
        : NetEventDescriptor(NetEventType::IP_TRANSMIT_REQUEST),
          networkBuffer(nb),
          destinationIpAddress(address),
          protocol(proto),
          ttl(overrideTtl),
          succeeded(false) {
      }
    };
  }
}
