/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
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

  template<uint8_t... TChannelNumbers> using Adc1Cycle1InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_1Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle7InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_7Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle13InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_13Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle28InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_28Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle41InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_41Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle55InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_55Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle71InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_71Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle239InjectedChannelFeature = AdcInjectedChannelFeature<1,ADC_SampleTime_239Cycles5,TChannelNumbers...>;

  template<uint8_t... TChannelNumbers> using Adc2Cycle1InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_1Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle7InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_7Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle13InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_13Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle28InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_28Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle41InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_41Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle55InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_55Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle71InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_71Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle239InjectedChannelFeature = AdcInjectedChannelFeature<2,ADC_SampleTime_239Cycles5,TChannelNumbers...>;

  template<uint8_t... TChannelNumbers> using Adc3Cycle1InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_1Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle7InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_7Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle13InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_13Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle28InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_28Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle41InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_41Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle55InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_55Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle71InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_71Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle239InjectedChannelFeature = AdcInjectedChannelFeature<3,ADC_SampleTime_239Cycles5,TChannelNumbers...>;

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
