/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a request to transmit an ethernet frame
     */

    struct EthernetTransmitRequestEvent : NetEventDescriptor {

      NetBuffer *networkBuffer;             ///< buffer to send
      const MacAddress& macAddress;         ///< reference to MAC (will be copied out)
      EtherType etherType;                  ///< ethertype number
      bool succeeded;                       ///< set to true by the link layer if the transmit was accepted

      /**
       * Constructor
       * @param nb The buffer pointer to send (the receive owns it)
       * @param mac The MAC address
       * @param et The event type
       */

      EthernetTransmitRequestEvent(NetBuffer *nb,const MacAddress& mac,EtherType et,DatalinkChecksum ckreq)
        : NetEventDescriptor(NetEventType::ETHERNET_TRANSMIT_REQUEST),
          networkBuffer(nb),
          macAddress(mac),
          etherType(et),
          succeeded(false) {

        // transfer the checksum request into the netbuffer as they are tightly coupled

        nb->setChecksumRequest(ckreq);
      }
    };
  }
}
