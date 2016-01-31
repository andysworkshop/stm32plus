/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Trivial feature class to enable continuous-mode
   */

  struct AdcContinuousModeFeature : AdcFeatureBase {

    /**
     * Constructor
     * @param adc
     */

    AdcContinuousModeFeature(Adc& adc)
      : AdcFeatureBase(adc) {
      ((ADC_InitTypeDef *)adc)->ADC_ContinuousConvMode=ENABLE;
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
    }
  };
}
