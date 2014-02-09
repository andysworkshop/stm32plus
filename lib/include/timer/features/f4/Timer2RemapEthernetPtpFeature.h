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
   * Remap ethernet PTP to timer 2 ITR1
   */

  class Timer2RemapEthernetPtpFeature : public TimerFeatureBase {

    public:
      Timer2RemapEthernetPtpFeature(Timer& timer);
  };


  /**
   * Constructor
   * @param timer
   */

  inline Timer2RemapEthernetPtpFeature::Timer2RemapEthernetPtpFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_RemapConfig(TIM2,TIM2_ETH_PTP);
  }
}
