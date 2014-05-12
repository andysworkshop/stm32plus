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
   * Provide support for the auto-power off feature of the F0 ADC. When this feature is
   * enabled the ADC will be powered-down between conversions. You do not have to do anything
   * to power it back up, it's all automatic. This feature will automatically enable low
   * power mode and provides methods for you to enable/disable at runtime if required.
   */

  struct AdcAutoPowerOffFeature : AdcFeatureBase {

    /**
     * Constructor
     * @param adc The adc peripheral
     */

    AdcAutoPowerOffFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      enableAutoPowerOff();
    }


    /**
     * Enable the auto power-off mode
     */

    void enableAutoPowerOff() const {
      ADC_AutoPowerOffCmd(_adc,ENABLE);
    }


    /**
     * Disable the auto power-off mode
     */

    void disableAutoPowerOff() const {
      ADC_AutoPowerOffCmd(_adc,DISABLE);
    }
  };
}
