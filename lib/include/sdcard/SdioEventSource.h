/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * SDIO event types
   */

  enum class SdioEventType : uint8_t {
    EVENT_DATA_END,
    EVENT_CRC_FAIL,
    EVENT_TIMEOUT,
    EVENT_RX_OVERRUN,
    EVENT_TX_OVERRUN,
    EVENT_START_BIT_ERROR
  };


  /**
   * The signature for SDIO events: void myHandler(SdioEventType set);
   */

  DECLARE_EVENT_SIGNATURE(SdioInterrupt,void(SdioEventType));


  /**
   * Base structure that holds just the event subscriber/publisher for SDIO
   */

  struct SdioEventSource {
    DECLARE_EVENT_SOURCE(SdioInterrupt);
  };
}
