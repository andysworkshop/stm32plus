/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * CAN event types
   */

  enum class CanEventType : uint8_t {
    EVENT_SLEEP,
    EVENT_WAKEUP,
    EVENT_ERROR,
    EVENT_LAST_ERROR_CODE,
    EVENT_BUS_OFF,
    EVENT_ERROR_PASSIVE,
    EVENT_ERROR_WARNING,
    EVENT_FIFO1_OVR,
    EVENT_FIFO1_FULL,
    EVENT_FIFO1_MESSAGE_PENDING,
    EVENT_FIFO0_OVR,
    EVENT_FIFO0_FULL,
    EVENT_FIFO0_MESSAGE_PENDING,
    EVENT_TRANSMIT_MAILBOX_EMPTY
  };


  /**
   * The signature for CAN events: void myHandler(CanEventType cet,uint8_t canNumber);
   */

  DECLARE_EVENT_SIGNATURE(CanInterrupt,void(CanEventType));


  /**
   * Base structure that holds just the event subscriber/publisher for the ADC
   */

  struct CanEventSource {
    DECLARE_EVENT_SOURCE(CanInterrupt);
  };
}
