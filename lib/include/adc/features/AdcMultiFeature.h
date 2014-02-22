/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Template class that will be used to initialise the multi-adc mode. You're expected
   * to use one of the template typedefs down below instead of instantiating this
   * directly.
   */

  template<uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  struct AdcMultiFeature : AdcFeatureBase {

    AdcMultiFeature(Adc& adc)
      : AdcFeatureBase(adc) {

      static_assert(TTwoSamplingDelay>=5 && TTwoSamplingDelay<=20,"TTwoSamplingDelay must be between 5 and 20");

      // modes for ADC and DMA

      ((ADC_CommonInitTypeDef *)adc)->ADC_Mode=TAdcMultiModeType;
      ((ADC_CommonInitTypeDef *)adc)->ADC_DMAAccessMode=TDmaModeType;

      // initialise this such that the optimised will eliminate all the conditional tests

      ((ADC_CommonInitTypeDef *)adc)->ADC_TwoSamplingDelay=
          TTwoSamplingDelay==5 ? ADC_TwoSamplingDelay_5Cycles :
          TTwoSamplingDelay==6 ? ADC_TwoSamplingDelay_6Cycles :
          TTwoSamplingDelay==7 ? ADC_TwoSamplingDelay_7Cycles :
          TTwoSamplingDelay==8 ? ADC_TwoSamplingDelay_8Cycles :
          TTwoSamplingDelay==9 ? ADC_TwoSamplingDelay_9Cycles :
          TTwoSamplingDelay==10 ? ADC_TwoSamplingDelay_10Cycles :
          TTwoSamplingDelay==11 ? ADC_TwoSamplingDelay_11Cycles :
          TTwoSamplingDelay==12 ? ADC_TwoSamplingDelay_12Cycles :
          TTwoSamplingDelay==13 ? ADC_TwoSamplingDelay_13Cycles :
          TTwoSamplingDelay==14 ? ADC_TwoSamplingDelay_14Cycles :
          TTwoSamplingDelay==15 ? ADC_TwoSamplingDelay_15Cycles :
          TTwoSamplingDelay==16 ? ADC_TwoSamplingDelay_16Cycles :
          TTwoSamplingDelay==17 ? ADC_TwoSamplingDelay_17Cycles :
          TTwoSamplingDelay==18 ? ADC_TwoSamplingDelay_18Cycles :
          TTwoSamplingDelay==19 ? ADC_TwoSamplingDelay_19Cycles :
          ADC_TwoSamplingDelay_20Cycles;
    }
  };


  /*
   * 2 ADC, Independent mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcIndependentNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcIndependentDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcIndependentDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcIndependentDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_Mode_Independent,TTwoSamplingDelay>;

  /*
   * 2 ADC, Injected simultaneous mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Regular simultaneous mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_DualMode_RegSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Interleaved mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcInterleavedNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcInterleavedDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcInterleavedDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcInterleavedDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_DualMode_Interl,TTwoSamplingDelay>;

  /*
   * 2 ADC, Alternate trigger mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;

  /*
   * 2 ADC, Combined regular/injected simultaneous mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Combined regular simultaneous alternate trigger mode
   */

  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;

  /*
   * 3 ADC, Independent mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcIndependentNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcIndependentDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcIndependentDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcIndependentDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_Mode_Independent,TTwoSamplingDelay>;

  /*
   * 3 ADC, Injected simultaneous mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Regular simultaneous mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Interleaved mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcInterleavedNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcInterleavedDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcInterleavedDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcInterleavedDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_TripleMode_Interl,TTwoSamplingDelay>;

  /*
   * 3 ADC, Alternate trigger mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;

  /*
   * 3 ADC, Combined regular/injected simultaneous mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Combined regular simultaneous alternate trigger mode
   */

  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerNoDmaFeature=AdcMultiFeature<ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerDmaMode1Feature=AdcMultiFeature<ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerDmaMode2Feature=AdcMultiFeature<ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerDmaMode3Feature=AdcMultiFeature<ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
}
