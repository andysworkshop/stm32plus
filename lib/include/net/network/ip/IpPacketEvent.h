/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for an IP packet arrival
     */

    struct IpPacketEvent : NetEventDescriptor {

      /**
       * The IP packet reference
       */

      IpPacket& ipPacket;


      /**
       * Constructor
       * @param frame
       */

      IpPacketEvent(IpPacket& packet)
        : NetEventDescriptor(NetEventType::IP_PACKET),
          ipPacket(packet) {
      }
    };
  }
}
