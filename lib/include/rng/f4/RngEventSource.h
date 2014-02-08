/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * RNG event types
   */

  enum class RngEventType : uint8_t {
    EVENT_DATA_READY,       ///< next random is ready
    EVENT_SEED_ERROR,       ///< error in random seed
    EVENT_CLOCK_ERROR       ///< error in clock
  };


  /**
   * The signature for RNG events: void myHandler(RngEventType ret,uint32_t randomNumber);
   */

  DECLARE_EVENT_SIGNATURE(RngInterrupt,void(RngEventType,uint32_t));


  /**
   * Base structure that holds just the event subscriber/publisher for SPI
   */

  struct RngEventSource {
    DECLARE_EVENT_SOURCE(RngInterrupt);
  };
}
