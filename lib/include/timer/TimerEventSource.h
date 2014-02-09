/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Timer event types
   */

  enum class TimerEventType : uint8_t {
    EVENT_COMPARE1,
    EVENT_COMPARE2,
    EVENT_COMPARE3,
    EVENT_COMPARE4,
    EVENT_UPDATE,
    EVENT_TRIGGER,
    EVENT_BREAK,
    EVENT_COM
  };


  /**
   * The signature for timer events: void myHandler(TimerEventType tet,uint8_t timerNumber);
   * timerNumber is required to differentiate between timers multiplexed on the same IRQ
   */

  DECLARE_EVENT_SIGNATURE(TimerInterrupt,void(TimerEventType,uint8_t));


  /**
   * Base structure that holds just the event subscriber/publisher for timers
   */

  struct TimerEventSource {
    DECLARE_EVENT_SOURCE(TimerInterrupt);
  };
}
