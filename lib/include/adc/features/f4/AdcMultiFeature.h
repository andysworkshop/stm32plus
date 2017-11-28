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
   * Template class that will be used to initialise the multi-adc mode. You're expected
   * to use one of the template typedefs down below instead of instantiating this
   * directly.
   */

  template<uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  struct AdcMultiFeature : AdcFeatureBase {

    AdcMultiFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      static_assert(TTwoSamplingDelay>=5 && TTwoSamplingDelay<=20,"TTwoSamplingDelay must be between 5 and 20");

      // modes for ADC and DMA

      ((ADC_CommonInitTypeDef *)adc)->ADC_Mode=TAdcMultiModeType;
      ((ADC_CommonInitTypeDef *)adc)->ADC_DMAAccessMode=TDmaModeType;

      // initialise this such that the optimised will eliminate all the conditional tests

      ((ADC_CommonInitTypeDef *)adc)->ADC_TwoSamplingDelay=
          TTwoSamplingDelay==5 ? ADC_TwoSamplingDelay_5Cycles :
          TTwoSamplingDelay==6 ? ADC_TwoSamplingDelay_6Cycles :
          TTwoSamplingDelay==7 ? ADC_TwoSamplingDelay_7Cycles :
          TTwoSamplingDelay==8 ? ADC_TwoSamplingDelay_8Cycles :
          TTwoSamplingDelay==9 ? ADC_TwoSamplingDelay_9Cycles :
          TTwoSamplingDelay==10 ? ADC_TwoSamplingDelay_10Cycles :
          TTwoSamplingDelay==11 ? ADC_TwoSamplingDelay_11Cycles :
          TTwoSamplingDelay==12 ? ADC_TwoSamplingDelay_12Cycles :
          TTwoSamplingDelay==13 ? ADC_TwoSamplingDelay_13Cycles :
          TTwoSamplingDelay==14 ? ADC_TwoSamplingDelay_14Cycles :
          TTwoSamplingDelay==15 ? ADC_TwoSamplingDelay_15Cycles :
          TTwoSamplingDelay==16 ? ADC_TwoSamplingDelay_16Cycles :
          TTwoSamplingDelay==17 ? ADC_TwoSamplingDelay_17Cycles :
          TTwoSamplingDelay==18 ? ADC_TwoSamplingDelay_18Cycles :
          TTwoSamplingDelay==19 ? ADC_TwoSamplingDelay_19Cycles :
          ADC_TwoSamplingDelay_20Cycles;
    }


    /**
     * Get the results of a multi-conversion
     * @return The results of the conversion
     */

    uint32_t getMultiConversionValue() const {
      return ADC_GetMultiModeConversionValue();
    }
  };
}
