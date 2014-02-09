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
   * Remap USB HS SOF to timer 2 ITR1
   */

  class Timer2RemapUsbHighSpeedSofFeature : public TimerFeatureBase {

    public:
      Timer2RemapUsbHighSpeedSofFeature(Timer& timer);
  };


  /**
   * Constructor
   * @param timer
   */

  inline Timer2RemapUsbHighSpeedSofFeature::Timer2RemapUsbHighSpeedSofFeature(Timer& timer)
    : TimerFeatureBase(timer) {

    TIM_RemapConfig(TIM2,TIM2_USBHS_SOF);
  }
}
