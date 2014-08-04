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
   * Handle the ADC regular channels (0..17 on the F1). ST's ADC_Channel_XX constants are simple
   * numbers 0..17 so we can use this feature easily, for example...
   *
   * Adc1Cycle55RegularChannelFeature<1,2,6,7>
   *
   * ...will initialise channels 1,2,6 and 7 with 55.5 cycle sample time on ADC1.
   * @tparam TAdcNumber The ADC number (1..3)
   * @tparam The peripheral library constant for the number of conversion cycles, e.g. ADC_SampleTime_7Cycles
   * @tparam TChannelNumbers List of channel numbers, 0..15.
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  class AdcRegularChannelFeature : public AdcFeatureBase {

    protected:

      template<uint8_t TChannelNumber>
      void init();

      template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
      void init();

    public:
      AdcRegularChannelFeature(Adc& adc);
      void initialise();
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  template<uint8_t... TChannelNumbers> using Adc1Cycle1RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_1Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle7RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_7Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle13RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_13Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle28RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_28Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle41RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_41Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle55RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_55Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle71RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_71Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle239RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_239Cycles5,TChannelNumbers...>;

  template<uint8_t... TChannelNumbers> using Adc2Cycle1RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_1Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle7RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_7Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle13RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_13Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle28RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_28Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle41RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_41Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle55RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_55Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle71RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_71Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc2Cycle239RegularChannelFeature = AdcRegularChannelFeature<2,ADC_SampleTime_239Cycles5,TChannelNumbers...>;

  template<uint8_t... TChannelNumbers> using Adc3Cycle1RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_1Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle7RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_7Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle13RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_13Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle28RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_28Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle41RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_41Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle55RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_55Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle71RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_71Cycles5,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc3Cycle239RegularChannelFeature = AdcRegularChannelFeature<3,ADC_SampleTime_239Cycles5,TChannelNumbers...>;


  /**
   * Constructor
   * @param adc Adc class reference
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::AdcRegularChannelFeature(Adc& adc)
    : AdcFeatureBase(adc) {

    // increase the number of channels being converted

    ((ADC_InitTypeDef *)adc)->ADC_NbrOfChannel+=sizeof...(TChannelNumbers);
  }


  /**
   * Initialise after ADC_Init
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline void AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::initialise() {

    // expand and initialise

    init<TChannelNumbers...>();
  }


  /**
   * Base case initialiser for when parameter pack expansion gets to the last one
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TChannelNumber>
  inline void AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::init() {

    if(TChannelNumber<=15)
      AdcChannelGpioInitialiser<TAdcNumber,TChannelNumber>::initialiseGpioPin();

    ADC_RegularChannelConfig(_adc,TChannelNumber,_adc.getAndIncrementRegularChannelRank(TAdcNumber),TSampleCycles);
  }


  /**
   * Recursive initialise for the template parameter pack.
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
  inline void AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::init() {

    if(TFirst<=15)
      AdcChannelGpioInitialiser<TAdcNumber,TFirst>::initialiseGpioPin();

    ADC_RegularChannelConfig(_adc,TFirst,_adc.getAndIncrementRegularChannelRank(TAdcNumber),TSampleCycles);
    init<TNext,TRest...>();
  }
}
