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
   * Template base class for the trigger definition classes
   */

  template<uint32_t TEvent,uint32_t TEdge>
  struct AdcInjectedTriggerFeature : AdcFeatureBase {

    AdcInjectedTriggerFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      // set up the two trigger values

      ADC_ExternalTrigInjectedConvConfig(adc,TEvent);
      ADC_ExternalTrigInjectedConvEdgeConfig(adc,TEdge);
    }

    void initialise() {
    }
  };


  /*
   * Typedef for the trigger on EXTI 15
   */

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_Ext_IT15,ADC_ExternalTrigInjecConvEdge_Rising> AdcExti15RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_Ext_IT15,ADC_ExternalTrigInjecConvEdge_Rising> AdcExti15FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_Ext_IT15,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcExti15RisingFallingTriggerFeature;

  /*
   * Typedefs for the channel trigger features
   */


  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T1_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer1Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T1_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer1Channel4FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T1_CC4,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer1Channel4RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T2_CC1,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer2Channel1RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T2_CC1,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer2Channel1FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T2_CC1,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer2Channel1RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC2,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer3Channel2RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC2,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer3Channel2FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC2,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer3Channel2RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer3Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer3Channel4FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T3_CC4,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer3Channel4RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC1,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer4Channel1RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC1,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer4Channel1FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC1,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer4Channel1RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC2,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer4Channel2RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC2,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer4Channel2FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC2,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer4Channel2RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC3,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer4Channel3RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC3,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer4Channel3FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T4_CC3,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer4Channel3RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T5_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer5Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T5_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer5Channel4FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T5_CC4,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer5Channel4RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC2,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer8Channel2RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC2,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer8Channel2FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC2,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer8Channel2RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC3,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer8Channel3RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC3,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer8Channel3FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC3,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer8Channel3RisingFallingTriggerFeature;

  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer8Channel4RisingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC4,ADC_ExternalTrigInjecConvEdge_Rising> AdcTimer8Channel4FallingTriggerFeature;
  typedef AdcInjectedTriggerFeature<ADC_ExternalTrigInjecConv_T8_CC4,ADC_ExternalTrigInjecConvEdge_RisingFalling> AdcTimer8Channel4RisingFallingTriggerFeature;


  /**
   * Timer trigger outputs need you to define the trigger source. Here's
   * the template we'll use to do that.
   */

  template<PeripheralName TTimerPeripheral,AdcTriggerSource TSource,uint32_t TEvent,uint32_t TEdge>
  struct AdcTimerInjectedTriggerFeature : AdcInjectedTriggerFeature<TEvent,TEdge>  {

    /**
     * Constructor
     * @param adc The adc class reference
     */

    AdcTimerInjectedTriggerFeature(Adc& adc)
      : AdcInjectedTriggerFeature<TEvent,TEdge>(adc) {

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
  using AdcTimer1InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T1_TRGO,ADC_ExternalTrigInjecConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer1InjectedTriggerFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T1_TRGO,ADC_ExternalTrigInjecConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer1InjectedTriggerRisingFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T1_TRGO,ADC_ExternalTrigInjecConvEdge_RisingFalling>;

  /**
   * Template typedefs for the trigger features - timer 2
   */

  template<AdcTriggerSource TSource>
  using AdcTimer2InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T2_TRGO,ADC_ExternalTrigInjecConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer2InjectedTriggerFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T2_TRGO,ADC_ExternalTrigInjecConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer2InjectedTriggerRisingFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T2_TRGO,ADC_ExternalTrigInjecConvEdge_RisingFalling>;

  /**
   * Template typedefs for the trigger features - timer 4
   */

  template<AdcTriggerSource TSource>
  using AdcTimer4InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T4_TRGO,ADC_ExternalTrigInjecConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer4InjectedTriggerFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T4_TRGO,ADC_ExternalTrigInjecConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer4InjectedTriggerRisingFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T4_TRGO,ADC_ExternalTrigInjecConvEdge_RisingFalling>;

  /**
   * Template typedefs for the trigger features - timer 5
   */

  template<AdcTriggerSource TSource>
  using AdcTimer5InjectedTriggerRisingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T5_TRGO,ADC_ExternalTrigInjecConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer5InjectedTriggerFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T5_TRGO,ADC_ExternalTrigInjecConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer5InjectedTriggerRisingFallingFeature=AdcTimerInjectedTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigInjecConv_T5_TRGO,ADC_ExternalTrigInjecConvEdge_RisingFalling>;
}
