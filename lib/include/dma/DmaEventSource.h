/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * DMA event types
   */

  enum class DmaEventType : uint8_t {
    EVENT_COMPLETE,
    EVENT_HALF_COMPLETE,
    EVENT_TRANSFER_ERROR,
  };


  /**
   * The signature for DMA events: void myHandler(DmaEventType det);
   */

  DECLARE_EVENT_SIGNATURE(DmaInterrupt,void(DmaEventType));


  /**
   * Base structure that holds just the event subscriber/publisher for DMA
   */

  struct DmaEventSource {
    DECLARE_EVENT_SOURCE(DmaInterrupt);
  };
}
