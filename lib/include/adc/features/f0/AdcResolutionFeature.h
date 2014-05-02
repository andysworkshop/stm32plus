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
   * Feature class to allow changing the resolution. You specify the resolution: 12/10/8/6 as a template parameter
   * and the code selects the correct constant at compile time.
   */

  template<uint8_t TResolution>
  struct AdcResolutionFeature : AdcFeatureBase {

    /**
     * Constructor, customise the resolution
     * @param adc The adc object holding the setup values
     */

    AdcResolutionFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      // this is a switch on a constant - the optimiser will eliminate everything except
      // the selected assignment

      switch(TResolution) {

        case 12:
          ((ADC_InitTypeDef *)adc)->ADC_Resolution=ADC_Resolution_12b;
          break;

        case 10:
          ((ADC_InitTypeDef *)adc)->ADC_Resolution=ADC_Resolution_10b;
          break;

        case 8:
          ((ADC_InitTypeDef *)adc)->ADC_Resolution=ADC_Resolution_8b;
          break;

        case 6:
          ((ADC_InitTypeDef *)adc)->ADC_Resolution=ADC_Resolution_6b;
          break;
      }
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
    }
  };
}
