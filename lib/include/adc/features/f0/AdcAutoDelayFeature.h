/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * Provide support for the auto-delay feature of the F0 ADC. When this is enabled the
   * ADC will not start converting until the results of the previous conversion have been
   * read out. This is useful if your MCU may be temporarily held up processing something else
   * but is generally fast enough to cope with your A/D requirements. The feature is enabled
   * by default and methods are provided to enable/disable at runtime.
   */

  struct AdcAutoDelayFeature : AdcFeatureBase {

    /**
     * Constructor
     * @param adc The adc peripheral
     */

    AdcAutoDelayFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      enableAutoDelay();
    }


    /**
     * Enable the auto delay mode
     */

    void enableAutoDelay() const {
      ADC_WaitModeCmd(_adc,ENABLE);
    }


    /**
     * Disable the auto delay mode
     */

    void disableAutoDelay() const {
      ADC_WaitModeCmd(_adc,DISABLE);
    }
  };
}
