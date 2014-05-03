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
   * Template base class for the trigger definition classes. Trigger edge selection is not available on
   * the F1 - it's always on the rising edge.
   */

  template<uint32_t TEvent>
  struct AdcTriggerFeature : AdcFeatureBase {

    AdcTriggerFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      // set up the trigger value

      ((ADC_InitTypeDef *)adc)->ADC_ExternalTrigConv=TEvent;
    }

    void initialise() {
      ADC_ExternalTrigConvCmd(_adc,ENABLE);
    }
  };


  /*
   * Typedef for the trigger on EXTI 11
   */

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_Ext_IT11_TIM8_TRGO> AdcExti11TriggerFeature;

  /*
   * Typedefs for the channel trigger features
   */

  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC1> AdcTimer1Channel1TriggerFeature;   // ADC1,2
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC2> AdcTimer1Channel2TriggerFeature;   // ADC1,2
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T1_CC3> AdcTimer1Channel3TriggerFeature;   // ADC1,2,3
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC2> AdcTimer2Channel2TriggerFeature;   // ADC1,2
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T2_CC3> AdcTimer2Channel3TriggerFeature;   // ADC3 only
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T3_CC1> AdcTimer3Channel1TriggerFeature;   // ADC3 only
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T4_CC4> AdcTimer4Channel4TriggerFeature;   // ADC1,2
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC1> AdcTimer5Channel1TriggerFeature;   // ADC3 only
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T5_CC3> AdcTimer5Channel2TriggerFeature;   // ADC3 only
  typedef AdcTriggerFeature<ADC_ExternalTrigConv_T8_CC1> AdcTimer8Channel1TriggerFeature;   // ADC3 only


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

  template<PeripheralName TTimerPeripheral,AdcTriggerSource TSource,uint32_t TEvent>
  struct AdcTimerTriggerFeature : AdcTriggerFeature<TEvent>  {

    /**
     * Constructor
     * @param adc The adc class reference
     */

    AdcTimerTriggerFeature(Adc& adc)
      : AdcTriggerFeature<TEvent>(adc) {

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
   * Template typedefs for the trigger features - timer 3
   */

  template<AdcTriggerSource TSource>
  using AdcTimer3TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER3,TSource,ADC_ExternalTrigConv_T3_TRGO>;

  /*
   * Timer 8
   */

  template<AdcTriggerSource TSource>
  using AdcTimer8TriggerRisingFeature=AdcTimerTriggerFeature<PERIPHERAL_TIMER8,TSource,ADC_ExternalTrigConv_T8_TRGO>;
}
