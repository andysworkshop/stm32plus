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
   * Remap HSE/prescaler to TIM11 channel 1 input
   */

  class Timer11RemapHseFeature : public TimerFeatureBase {

    public:
      Timer11RemapHseFeature(Timer& timer);
  };


  /**
   * Constructor
   * @param timer
   */

  inline Timer11RemapHseFeature::Timer11RemapHseFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_RemapConfig(TIM11,TIM11_HSE);
  }
}
