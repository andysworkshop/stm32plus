/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * USART event types
   */

  enum class UsartEventType : uint8_t {
    EVENT_RECEIVE,
    EVENT_READY_TO_TRANSMIT,
    EVENT_TRANSMIT_COMPLETE,
    EVENT_PARITY_ERROR,
    EVENT_IDLE,
    EVENT_LINE_BREAK_DETECT,
    EVENT_ERROR,
    EVENT_CLEAR_TO_SEND,
  };


  /**
   * The signature for USART events: void myHandler(UsartEventType uet);
   */

  DECLARE_EVENT_SIGNATURE(UsartInterrupt,void(UsartEventType));


  /**
   * Base structure that holds just the event subscriber/publisher for the USART
   */

  struct UsartEventSource {
    DECLARE_EVENT_SOURCE(UsartInterrupt);
  };
}
