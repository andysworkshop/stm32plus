/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * I2C event types
   */

  enum class I2CEventType : uint8_t {
    EVENT_READY_TO_TRANSMIT,
    EVENT_RECEIVE,
    EVENT_STOP_BIT_RECEIVED,
    EVENT_10_BIT_HEADER_SENT,
    EVENT_BYTE_TRANSFER_SENT,
    EVENT_ADDRESS_SENT,
    EVENT_START_BIT_SENT,
    EVENT_SMB_ALERT,
    EVENT_TIMEOUT,
    EVENT_PEC_ERROR,
    EVENT_OVERRUN,
    EVENT_ACK_FAILURE,
    EVENT_ARBITRATION_LOSS,
    EVENT_BUS_ERROR,
    EVENT_ERROR,                    // F0
    EVENT_TRANSFER_COMPLETE,        // F0
    EVENT_ADDRESS_MATCH,            // F0
    EVENT_NOT_ACKNOWLEDGE           // F0
  };


  /**
   * The signature for I2C events: void myHandler(I2CEventType iet);
   */

  DECLARE_EVENT_SIGNATURE(I2CInterrupt,void(I2CEventType));


  /**
   * Base structure that holds just the event subscriber/publisher for I2C
   */

  struct I2CEventSource {
    DECLARE_EVENT_SOURCE(I2CInterrupt);
  };
}
