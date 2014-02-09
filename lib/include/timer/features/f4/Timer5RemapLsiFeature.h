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
   * Remap LSI output to TIM5 channel 4 input
   */

  class Timer5RemapLsiFeature : public TimerFeatureBase {

    public:
      Timer5RemapLsiFeature(Timer& timer);
  };


  /**
   * Constructor
   * @param timer
   */

  inline Timer5RemapLsiFeature::Timer5RemapLsiFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_RemapConfig(TIM5,TIM5_LSI);
  }
}
