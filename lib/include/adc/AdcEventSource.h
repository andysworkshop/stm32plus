/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * ADC event types
   */

  enum class AdcEventType : uint8_t {
    EVENT_REGULAR_END_OF_CONVERSION,
    EVENT_INJECTED_END_OF_CONVERSION,
    EVENT_ANALOG_WATCHDOG,
    EVENT_OVERFLOW,
    EVENT_END_OF_SEQUENCE,
    EVENT_END_OF_SAMPLING,
    EVENT_ADC_READY
  };


  /**
   * The signature for ADC events: void myHandler(UsartEventType uet,uint8_t adcNumber);
   */

  DECLARE_EVENT_SIGNATURE(AdcInterrupt,void(AdcEventType,uint8_t adcNumber));


  /**
   * Base structure that holds just the event subscriber/publisher for the ADC
   */

  struct AdcEventSource {
    DECLARE_EVENT_SOURCE(AdcInterrupt);
  };
}
