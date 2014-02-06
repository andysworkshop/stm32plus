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
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
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
   * Typedefs for the difference cycles
   */

  template<uint8_t... TChannelNumbers> using Adc3CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_3Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc15CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_15Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc28CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_28Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc56CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_56Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc84CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_84Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc112CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_112Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc144CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_144Cycles,TChannelNumbers...>;
  template<uint8_t... TChannelNumbers> using Adc480CycleRegularChannelFeature = AdcRegularChannelFeature<ADC_SampleTime_480Cycles,TChannelNumbers...>;


  /**
   * Constructor
   * @param adc Adc class reference
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  inline AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::AdcRegularChannelFeature(Adc& adc)
    : AdcFeatureBase(adc) {

    // expand and initialise

    init<TChannelNumbers...>(1);
  }


  /**
   * Recursive init
   * @param head
   * @param tail
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TChannelNumber>
  inline void AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::init(uint8_t rank) {

    ADC_RegularChannelConfig(_adc,TChannelNumber,rank,TSampleCycles);
  }


  /**
   * Base case init
   * @param head
   * @param tail
   */

  template<uint8_t TSampleCycles,uint8_t... TChannelNumbers>
  template<uint8_t TFirst,uint8_t TNext,uint8_t... TRest>
  inline void AdcRegularChannelFeature<TSampleCycles,TChannelNumbers...>::init(uint8_t rank) {

    ADC_RegularChannelConfig(_adc,TFirst,rank,TSampleCycles);
    init<TNext,TRest...>(rank);
  }
}
