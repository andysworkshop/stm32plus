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
   * Handle the ADC regular channels (0..18 on the F0). ST's ADC_Channel_XX constants are single bits
   * e.g. 1,2,4,8,16 = channel 0,1,2,3,4 and so on.
   *
   * Adc1Cycle7RegularChannelFeature<1,2,6,7>
   *
   * ...will initialise channels 1,2,6 and 7 with 7.5 cycle sample time.
   * @tparam The peripheral library constant for the number of conversion cycles, e.g. ADC_SampleTime_3Cycles
   * @tparam TChannelNumbers List of channel numbers, 0..15.
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
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


  template<uint8_t... TChannelNumbers> using Adc1Cycle1RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_1_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle7RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_7_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle13RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_13_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle28RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_28_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle41RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_41_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle55RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_55_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle71RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_71_5Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle239RegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_239_5Cycles,TChannelNumbers...>;


  /**
   * Constructor
   * @param adc Adc class reference
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::AdcRegularChannelFeature(Adc& adc)
    : AdcFeatureBase(adc) {
  }


  /**
   * Initialise after ADC_Init
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline void AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::initialise() {

    // expand and initialise

    init<TChannelNumbers...>();
  }


  /**
   * Base case initialiser for when parameter pack expansion gets to the last one
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TChannelNumber>
  inline void AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::init() {

    if(TChannelNumber<=15)
      AdcChannelGpioInitialiser<TChannelNumber>::initialiseGpioPin();

    ADC_ChannelConfig(_adc,1 << TChannelNumber,TSampleCycles);
  }


  /**
   * Recursive initialise for the template parameter pack.
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
  inline void AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::init() {

    if(TFirst<=15)
      AdcChannelGpioInitialiser<TFirst>::initialiseGpioPin();

    ADC_ChannelConfig(_adc,1 << TFirst,TSampleCycles);
    init<TNext,TRest...>();
  }
}
