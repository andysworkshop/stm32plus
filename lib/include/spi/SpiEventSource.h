/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * SPI event types
   */

  enum class SpiEventType : uint8_t {
    EVENT_RECEIVE,
    EVENT_READY_TO_TRANSMIT
  };


  /**
   * The signature for SPI events: void myHandler(SpiEventType set);
   */

  DECLARE_EVENT_SIGNATURE(SpiInterrupt,void(SpiEventType));


  /**
   * Base structure that holds just the event subscriber/publisher for SPI
   */

  struct SpiEventSource {
    DECLARE_EVENT_SOURCE(SpiInterrupt);
  };
}
