/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * Remap HSE/32 to TIM14 channel 1
   */

  class Timer14RemapHseDiv32Feature : public TimerFeatureBase {

    public:
      Timer14RemapHseDiv32Feature(Timer& timer);
  };


  /**
   * Constructor
   * @param timer
   */

  inline Timer14RemapHseDiv32Feature::Timer14RemapHseDiv32Feature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_RemapConfig(TIM14,TIM14_HSEDiv32);
  }
}
