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
   * Remap RTC CLK to TIM14 channel 1
   */

  class Timer14RemapRtcClkFeature : public TimerFeatureBase {

    public:
      Timer14RemapRtcClkFeature(Timer& timer);
  };


  /**
   * Constructor
   * @param timer
   */

  inline Timer14RemapRtcClkFeature::Timer14RemapRtcClkFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_RemapConfig(TIM14,TIM14_RTC_CLK);
  }
}
