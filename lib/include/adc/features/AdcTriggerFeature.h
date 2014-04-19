/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Template base class for the trigger definition classes
   */

  template<uint32_t TEvent,uint32_t TEdge>
  struct AdcTriggerFeature : AdcFeatureBase {

    AdcTriggerFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      // set up the two trigger values

      ((ADC_InitTypeDef *)adc)->ADC_ExternalTrigConv=TEvent;
      ((ADC_InitTypeDef *)adc)->ADC_ExternalTrigConvEdge=TEdge;
    }

    void initialise() {
    }
  };


#if defined(STM32PLUS_F4)

  /*
   * Typedef for the trigger on EXTI 11
   */

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_Ext_IT11,ADC_ExternalTrigConvEdge_Rising> AdcExti11RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_Ext_IT11,ADC_ExternalTrigConvEdge_Falling> AdcExti11FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_Ext_IT11,ADC_ExternalTrigConvEdge_RisingFalling> AdcExti11RisingFallingTriggerFeature;

  /*
   * Typedefs for the channel trigger features
   */


  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC1,ADC_ExternalTrigConvEdge_Rising> AdcTimer1Channel1RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC1,ADC_ExternalTrigConvEdge_Falling> AdcTimer1Channel1FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC1,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer1Channel1RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC2,ADC_ExternalTrigConvEdge_Rising> AdcTimer1Channel2RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC2,ADC_ExternalTrigConvEdge_Falling> AdcTimer1Channel2FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC2,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer1Channel2RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC3,ADC_ExternalTrigConvEdge_Rising> AdcTimer1Channel3RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC3,ADC_ExternalTrigConvEdge_Falling> AdcTimer1Channel3FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC3,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer1Channel3RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC2,ADC_ExternalTrigConvEdge_Rising> AdcTimer2Channel2RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC2,ADC_ExternalTrigConvEdge_Falling> AdcTimer2Channel2FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC2,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer2Channel2RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC3,ADC_ExternalTrigConvEdge_Rising> AdcTimer2Channel3RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC3,ADC_ExternalTrigConvEdge_Falling> AdcTimer2Channel3FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC3,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer2Channel3RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T3_CC1,ADC_ExternalTrigConvEdge_Rising> AdcTimer3Channel1RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T3_CC1,ADC_ExternalTrigConvEdge_Falling> AdcTimer3Channel1FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T3_CC1,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer3Channel1RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T4_CC4,ADC_ExternalTrigConvEdge_Rising> AdcTimer4Channel4RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T4_CC4,ADC_ExternalTrigConvEdge_Falling> AdcTimer4Channel4FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T4_CC4,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer4Channel4RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC1,ADC_ExternalTrigConvEdge_Rising> AdcTimer5Channel1RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC1,ADC_ExternalTrigConvEdge_Falling> AdcTimer5Channel1FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC1,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer5Channel1RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC2,ADC_ExternalTrigConvEdge_Rising> AdcTimer5Channel2RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC2,ADC_ExternalTrigConvEdge_Falling> AdcTimer5Channel2FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC2,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer5Channel2RisingFallingTriggerFeature;

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T8_CC1,ADC_ExternalTrigConvEdge_Rising> AdcTimer8Channel1RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T8_CC1,ADC_ExternalTrigConvEdge_Falling> AdcTimer8Channel1FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T8_CC1,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer8Channel1RisingFallingTriggerFeature;

#elif defined(STM32PLUS_F0)

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC4,ADC_ExternalTrigConvEdge_Rising> AdcTimer1Channel4RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC4,ADC_ExternalTrigConvEdge_Falling> AdcTimer1Channel4FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC4,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer1Channel4RisingFallingTriggerFeature;

#else
#error "Unsupported MCU"
#endif


  /**
   * Timer trigger outputs need you to define the trigger source. Here's
   * the template we'll use to do that.
   */

  /**
   * Possible trigger sources
   */

  enum class AdcTriggerSource {
    Reset,           //!< Reset
    Enable,          //!< Enable
    Update,          //!< Update
    CaptureCompare1, //!< CaptureCompare1
    OutputReference1,//!< OutputReference1
    OutputReference2,//!< OutputReference2
    OutputReference3,//!< OutputReference3
    OutputReference4,//!< OutputReference4
  };

  template<PeripheralName TTimerPeripheral,AdcTriggerSource TSource,uint32_t TEvent,uint32_t TEdge>
  struct AdcTimerTriggerFeature : AdcTriggerFeature<TEvent,TEdge>  {

    /**
     * Constructor
     * @param adc The adc class reference
     */

    AdcTimerTriggerFeature(Adc& adc)
      : AdcTriggerFeature<TEvent,TEdge>(adc) {

      static_assert(TSource>=AdcTriggerSource::Reset && TSource<=AdcTriggerSource::OutputReference4,"Invalid trigger source");

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
   * Template typedefs for the trigger features - timer 2
   */

  template<AdcTriggerSource TSource>
  using AdcTimer2TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER2,TSource,ADC_ExternalTrigConv_T2_TRGO,ADC_ExternalTrigConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer2TriggerFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER2,TSource,ADC_ExternalTrigConv_T2_TRGO,ADC_ExternalTrigConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer2TriggerRisingFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER2,TSource,ADC_ExternalTrigConv_T2_TRGO,ADC_ExternalTrigConvEdge_RisingFalling>;

  /*
   * Timer 3
   */

  template<AdcTriggerSource TSource>
  using AdcTimer3TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER3,TSource,ADC_ExternalTrigConv_T3_TRGO,ADC_ExternalTrigConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer3TriggerFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER3,TSource,ADC_ExternalTrigConv_T3_TRGO,ADC_ExternalTrigConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer3TriggerRisingFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER3,TSource,ADC_ExternalTrigConv_T3_TRGO,ADC_ExternalTrigConvEdge_RisingFalling>;

#if defined(STM32PLUS_F4)

  /*
   * Timer 8
   */

  template<AdcTriggerSource TSource>
  using AdcTimer8TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER8,TSource,ADC_ExternalTrigConv_T8_TRGO,ADC_ExternalTrigConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer8TriggerFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER8,TSource,ADC_ExternalTrigConv_T8_TRGO,ADC_ExternalTrigConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer8TriggerRisingFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER8,TSource,ADC_ExternalTrigConv_T8_TRGO,ADC_ExternalTrigConvEdge_RisingFalling>;

#elif defined(STM32PLUS_F0)

  /*
   * Timer 1
   */

  template<AdcTriggerSource TSource>
  using AdcTimer1TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigConv_T1_TRGO,ADC_ExternalTrigConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer1TriggerFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigConv_T1_TRGO,ADC_ExternalTrigConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer1TriggerRisingFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER1,TSource,ADC_ExternalTrigConv_T1_TRGO,ADC_ExternalTrigConvEdge_RisingFalling>;

  /*
   * Timer 15
   */

  template<AdcTriggerSource TSource>
  using AdcTimer15TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER15,TSource,ADC_ExternalTrigConv_T15_TRGO,ADC_ExternalTrigConvEdge_Rising>;

  template<AdcTriggerSource TSource>
  using AdcTimer15TriggerFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER15,TSource,ADC_ExternalTrigConv_T15_TRGO,ADC_ExternalTrigConvEdge_Falling>;

  template<AdcTriggerSource TSource>
  using AdcTimer15TriggerRisingFallingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER15,TSource,ADC_ExternalTrigConv_T15_TRGO,ADC_ExternalTrigConvEdge_RisingFalling>;

#else
#error "Unsupported MCU"
#endif

}
