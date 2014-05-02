/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Feature class to allow changing the prescaler for the ADC analog clock. The ADC clock on the F4 runs off
   * APB2 divided by a prescaler. You specify the prescaler: 2/4/8 as a template parameter and the code
   * selects the correct constant at compile time.
   */

  template<uint8_t TPrescaler>
  struct AdcClockPrescalerFeature : AdcFeatureBase {

    /**
     * Constructor, customise the prescaler
     * @param adc The adc object holding the setup values
     */

    AdcClockPrescalerFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      static_assert(TPrescaler==2 || TPrescaler==4 || TPrescaler==8,"Invalid ADC prescaler: 2,4,8 only");

      // this is a switch on a constant - the optimiser will eliminate everything except
      // the selected assignment

      switch(TPrescaler) {

        case 2:
          ((ADC_CommonInitTypeDef *)adc)->ADC_Prescaler=ADC_Prescaler_Div2;
          break;

        case 4:
          ((ADC_CommonInitTypeDef *)adc)->ADC_Prescaler=ADC_Prescaler_Div4;
          break;

        case 8:
          ((ADC_CommonInitTypeDef *)adc)->ADC_Prescaler=ADC_Prescaler_Div8;
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
