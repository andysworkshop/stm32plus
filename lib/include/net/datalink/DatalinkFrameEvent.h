/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Event descriptor for a datalink frame
     */

    struct DatalinkFrameEvent : NetEventDescriptor {

      /**
       * The frame reference
       */

      DatalinkFrame& datalinkFrame;


      /**
       * Constructor
       * @param frame
       */

      DatalinkFrameEvent(DatalinkFrame& frame)
        : NetEventDescriptor(NetEventType::DATALINK_FRAME),
          datalinkFrame(frame) {
      }
    };
  }
}
