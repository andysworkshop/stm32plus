/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {
  namespace net {


    /**
     * Datalink layer frame information
     */

    struct DatalinkFrame {

      enum class FrameSource {
        ETHERNET_FRAME
      };

      uint8_t *payload;           //!< frame payload data - do not attempt to free
      uint32_t payloadLength;     //!< length of the frame payload
      uint16_t protocol;          //!< values match EtherType
      FrameSource frameSource;    //!< identify this frame to enable safe casting
    };
  }
}
