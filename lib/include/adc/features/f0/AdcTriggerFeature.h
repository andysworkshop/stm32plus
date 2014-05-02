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

      // the F0 will ignore hardware triggers until ADSTART is set

      ADC_StartOfConversion(ADC1);
    }
  };


  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC4,ADC_ExternalTrigConvEdge_Rising> AdcTimer1Channel4RisingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC4,ADC_ExternalTrigConvEdge_Falling> AdcTimer1Channel4FallingTriggerFeature;
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC4,ADC_ExternalTrigConvEdge_RisingFalling> AdcTimer1Channel4RisingFallingTriggerFeature;


  /**
   * Timer trigger outputs need you to define the trigger source. Here's
   * the template we'll use to do that.
   */

  /**
   * Possible trigger sources
   */

  enum class AdcTriggerSource {
    Reset,              //!< Reset
    Enable,             //!< Enable
    Update,             //!< Update
    CaptureCompare1,    //!< CaptureCompare1
    OutputReference1,   //!< OutputReference1
    OutputReference2,   //!< OutputReference2
    OutputReference3,   //!< OutputReference3
    OutputReference4,   //!< OutputReference4
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
}
