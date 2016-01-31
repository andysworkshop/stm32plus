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
   * Possible clock sources for the ADC. Asynchronous = 14MHz internal HSI. /2 and /4 are
   * PCLK dividers (24 and 12MHz on a 48MHz core F0).
   */

  enum class ClockModeType {
    ASYNCHRONOUS,
    PCLK_2,
    PCLK_4
  };


  /**
   * Generic forward template declaration
   */

  template<ClockModeType TClockMode>
  struct AdcClockModeFeature;


  /**
   * Specialisation for the async 14MHz HSI source
   */

  template<>
  struct AdcClockModeFeature<ClockModeType::ASYNCHRONOUS> : AdcFeatureBase {

    AdcClockModeFeature(Adc& adc)
        : AdcFeatureBase(adc) {
      ADC_ClockModeConfig(ADC1,ADC_ClockMode_AsynClk);
    }

    void initialise() const {}
  };


  /**
   * Specialisation for the PCLK/2 source
   */

  template<>
  struct AdcClockModeFeature<ClockModeType::PCLK_2> : AdcFeatureBase {

    AdcClockModeFeature(Adc& adc)
        : AdcFeatureBase(adc) {
      ADC_ClockModeConfig(ADC1,ADC_ClockMode_SynClkDiv2);
    }

    void initialise() const {}
  };


  /**
   * Specialisation for the PCLK/2 source
   */

  template<>
  struct AdcClockModeFeature<ClockModeType::PCLK_4> : AdcFeatureBase {

    AdcClockModeFeature(Adc& adc)
        : AdcFeatureBase(adc) {
      ADC_ClockModeConfig(ADC1,ADC_ClockMode_SynClkDiv4);
    }

    void initialise() const {}
  };


  /*
   * Typedefs for easy use
   */

  typedef AdcClockModeFeature<ClockModeType::ASYNCHRONOUS> AdcAsynchronousClockModeFeature;
  typedef AdcClockModeFeature<ClockModeType::PCLK_2> AdcPclk2ClockModeFeature;
  typedef AdcClockModeFeature<ClockModeType::PCLK_4> AdcPclk4ClockModeFeature;
}
