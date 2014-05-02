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
   * Template base class for the trigger definition classes
   */

  template<uint32_t TEvent>
  struct AdcInjectedTriggerFeature : AdcFeatureBase {

    AdcInjectedTriggerFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      // set up the two trigger values

      ADC_ExternalTrigInjectedConvConfig(adc,TEvent);
    }

    void initialise() {
      ADC_ExternalTrigInjectedConvCmd(_adc,ENABLE);
    }
  };


  /*
   * Typedef for the trigger on EXTI 15
   */

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4> AdcExti15RisingTriggerFeature;


  /*
   * Typedefs for the channel trigger features
   */

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T1_CC4> AdcTimer1Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T2_CC1> AdcTimer2Channel1RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC4> AdcTimer3Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC3> AdcTimer4Channel3RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T5_CC4> AdcTimer5Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC2> AdcTimer8Channel2RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC4> AdcTimer8Channel4RisingTriggerFeature;


  /**
   * Timer trigger outputs need you to define the trigger source. Here's
   * the template we'll use to do that.
   */

  template<PeripheralName TTimerPeripheral,AdcTriggerSource TSource,uint32_t TEvent>
  struct AdcTimerInjectedTriggerFeature : AdcInjectedTriggerFeature<TEvent>  {

    /**
     * Constructor
     * @param adc The adc class reference
     */

    AdcTimerInjectedTriggerFeature(Adc& adc)
      : AdcInjectedTriggerFeature<TEvent>(adc) {

      static_assert(TSource>=AdcTriggerSource::Reset && TSource<=AdcTriggerSource::OutputReference4,"Invalid injected trigger source");

      TIM_SelectOutputTrigger(
          reinterpret_cast<TIM_TypeDef *>(PeripheralTraits<TTimerPeripheral>::PERIPHERAL_BASE),
            TSource==AdcTriggerSource::Reset ? TIM_TRGOSource_Reset :
            TSource==AdcTriggerSource::Enable ? TIM_TRGOSource_Enable :
            TSource==AdcTriggerSource::Update ? TIM_TRGOSource_Update :
            TSource==AdcTriggerSource::CaptureCompare1 ? TIM_TRGOSource_OC1 :
            TSource==AdcTriggerSource::OutputReference1 ? TIM_TRGOSource_OC1Ref :
            TSource==AdcTriggerSource::OutputReference2 ? TIM_TRGOSource_OC2Ref :
            TSource==AdcTriggerSource::OutputReference3 ? TIM_TRGOSource_OC3Ref :
            TIM_TRGOSource_OC4Ref
      );
    }
  };


  /**
   * Template typedefs for the trigger features - timer 1
   */

  template<AdcTriggerSource TSource>
  using AdcTimer1InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T1_TRGO>;

  /**
   * Template typedefs for the trigger features - timer 2
   */

  template<AdcTriggerSource TSource>
  using AdcTimer2InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T2_TRGO>;

  /**
   * Template typedefs for the trigger features - timer 4
   */

  template<AdcTriggerSource TSource>
  using AdcTimer4InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T4_TRGO>;

  /**
   * Template typedefs for the trigger features - timer 5
   */

  template<AdcTriggerSource TSource>
  using AdcTimer5InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T5_TRGO>;
}
