/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Handle the ADC regular channels (0..18 on the F4). ST's ADC_Channel_XX constants are simple
   * numbers 0..18 so we can use this feature easily, for example...
   *
   * Adc3CycleRegularChannelFeature<1,2,6,7>
   *
   * ...will initialise channels 1,2,6 and 7 with 3 cycle sample time.
   * @tparam TAdcNumber The ADC number (1..3)
   * @tparam The peripheral library constant for the number of conversion cycles, e.g. ADC_SampleTime_3Cycles
   * @tparam TChannelNumbers List of channel numbers, 0..15.
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  class AdcRegularChannelFeature : public AdcFeatureBase {

    protected:
      template<uint8_t TChannelNumber>
      void init(uint8_t rank);

      template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
      void init(uint8_t rank);

    public:
      AdcRegularChannelFeature(Adc& adc);
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  template<uint8_t... TChannelNumbers> using Adc1Cycle3RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_3Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle15RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_15Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle28RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_28Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle56RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_56Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle84RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_84Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle112RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_112Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle144RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_144Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc1Cycle480RegularChannelFeature = AdcRegularChannelFeature<1,ADC_SampleTime_480Cycles,TChannelNumbers...>;


  /**
   * Constructor
   * @param adc Adc class reference
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::AdcRegularChannelFeature(Adc& adc)
    : AdcFeatureBase(adc) {

    // expand and initialise

    init<TChannelNumbers...>(1);
  }


  /**
   * Base case initialiser for when parameter pack expansion gets to the last one
   * @param rank The rank order of the channel (1..16)
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TChannelNumber>
  inline void AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::init(uint8_t rank) {

    AdcChannelGpioInitiaiser<TAdcNumber,TChannelNumber>::initialiseGpioPin();
    ADC_RegularChannelConfig(_adc,TChannelNumber,rank,TSampleCycles);
  }


  /**
   * Recursive initialise for the template parameter pack.
   * @param rank The rank order of the channel (1..16)
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
  inline void AdcRegularChannelFeature<TAdcNumber,TSampleCycles,TChannelNumbers...>::init(uint8_t rank) {

    AdcChannelGpioInitiaiser<TAdcNumber,TFirst>::initialiseGpioPin();
    ADC_RegularChannelConfig(_adc,TFirst,rank,TSampleCycles);
    init<TNext,TRest...>(rank+1);
  }
}
