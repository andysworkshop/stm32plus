/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace net {


    /**
     * ARP receive frame event
     */

    struct ArpReceiveEvent : NetEventDescriptor {

      /**
       * The ARP frame that arrived
       */

      const ArpFrameData& arpFrameData;


      /**
       * Constructor
       */

      ArpReceiveEvent(const ArpFrameData& afd)
        : NetEventDescriptor(NetEventType::ARP_FRAME),
          arpFrameData(afd) {
      }
    };
  }
}
