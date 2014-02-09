/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a datalink frame-sent event. This event is
     * sent when a frame has been put on to the wire and the NetBuffer
     * is about to be released.
     */

    struct DatalinkFrameSentEvent : NetEventDescriptor {

      /**
       * The net buffer reference
       */

      NetBuffer& netBuffer;


      /**
       * Constructor
       * @param nb The NetBuffer holding the frame that's just gone
       */

      DatalinkFrameSentEvent(NetBuffer& nb)
        : NetEventDescriptor(NetEventType::DATALINK_FRAME_SENT),
          netBuffer(nb) {
      }
    };
  }
}
