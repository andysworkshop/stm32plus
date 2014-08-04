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
   * Handle the ADC injected channels (0..3 on the F4). ST's ADC_Channel_XX constants are simple
   * numbers 0..18 so we can use this feature easily, for example...
   *
   * Adc1Cycle3InjectedChannelFeature<1,2,6,7>
   *
   * ...will initialise channels 1,2,6 and 7 with 3 cycle sample time on ADC1.
   * @tparam TAdcNumber The ADC number (1..3)
   * @tparam The peripheral library constant for the number of conversion cycles, e.g. ADC_SampleTime_3Cycles
   * @tparam TChannelNumbers List of channel numbers, 0..15.
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  class AdcInjectedChannelFeature : public AdcInjectedChannelFeatureBase {

    protected:

      template<uint8_t TChannelNumber>
      void init();

      template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
      void init();

    public:
      AdcInjectedChannelFeature(Adc& adc);
      void initialise();
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  template<uint8_t... TChannelNumbers> using Adc1Cycle3InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_3Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle15InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_15Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle28InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_28Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle56InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_56Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle84InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_84Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle112InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_112Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle144InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_144Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle480InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_480Cycles,TChannelNumbers...>;

  template<uint8_t... TChannelNumbers> using Adc2Cycle3InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_3Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle15InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_15Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle28InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_28Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle56InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_56Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle84InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_84Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle112InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_112Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle144InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_144Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle480InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_480Cycles,TChannelNumbers...>;

  template<uint8_t... TChannelNumbers> using Adc3Cycle3InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_3Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle15InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_15Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle28InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_28Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle56InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_56Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle84InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_84Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle112InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_112Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle144InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_144Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle480InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_480Cycles,TChannelNumbers...>;


  /**
   * Constructor
   * @param adc Adc class reference
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline AdcInjectedChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::AdcInjectedChannelFeature(Adc& adc)
    : AdcInjectedChannelFeatureBase(adc) {

    static_assert(TAdcNumber>=1 && TAdcNumber<=3,"Invalid ADC number, expecting 1/2/3");

    // increase the number of channels being converted

    adc.incrementInjectedChannelCount(sizeof...(TChannelNumbers));
  }


  /**
   * Initialise after ADC_Init
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline void AdcInjectedChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::initialise() {

    // expand and initialise

    init<TChannelNumbers...>();
  }


  /**
   * Base case initialiser for when parameter pack expansion gets to the last one
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TChannelNumber>
  inline void AdcInjectedChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::init() {

    AdcChannelGpioInitialiser<TAdcNumber,TChannelNumber>::initialiseGpioPin();
    ADC_InjectedChannelConfig(_adc,TChannelNumber,_adc.getAndIncrementInjectedChannelRank(TAdcNumber),TSampleCycles);
  }


  /**
   * Recursive initialise for the template parameter pack.
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
  inline void AdcInjectedChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::init() {

    AdcChannelGpioInitialiser<TAdcNumber,TFirst>::initialiseGpioPin();
    ADC_InjectedChannelConfig(_adc,TFirst,_adc.getAndIncrementInjectedChannelRank(TAdcNumber),TSampleCycles);
    init<TNext,TRest...>();
  }
}
