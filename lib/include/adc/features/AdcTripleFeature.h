/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Extension of the dual ADC feature to provide a third ADC. Only available on devices where
   * there is a third ADC.
   */

  template<class TAdc2,class TAdc3,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  class AdcTripleFeature : AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay> {

    protected:
      scoped_ptr<TAdc3> _adc3;

    public:
      AdcTripleFeature(Adc& adc);

      TAdc3& getAdc3() const;
      void initialise();
  };


  /**
   * Constructor
   * @param adc The adc instance
   */

  template<class TAdc2,class TAdc3,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline AdcTripleFeature<TAdc2,TAdc3,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::AdcTripleFeature(Adc& adc)
    : AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>(adc) {
  }


  /**
   * Get a reference to the second ADC
   * @return the second ADC reference
   */

  template<class TAdc2,class TAdc3,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline TAdc3& AdcTripleFeature<TAdc2,TAdc3,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::getAdc3() const {
    return *_adc3.get();
  }


  /**
   * Post ADC_Init() initialisation
   */

  template<class TAdc2,class TAdc3,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline void AdcTripleFeature<TAdc2,TAdc3,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::initialise() {

    // call the base class to initialise ADC2 then initialise ADC3 here

    AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::initialise();
    _adc3.reset(new TAdc3(AdcOperatingMode::MULTI_ADC));
  }


  /*
   * 3 ADC, Independent mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcIndependentNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcIndependentDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcIndependentDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcIndependentDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_Mode_Independent,TTwoSamplingDelay>;

  /*
   * 3 ADC, Injected simultaneous mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInjectedSimultaneousDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Regular simultaneous mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Interleaved mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInterleavedNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInterleavedDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInterleavedDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcInterleavedDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_Interl,TTwoSamplingDelay>;

  /*
   * 3 ADC, Alternate trigger mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcAlternateTriggerDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;

  /*
   * 3 ADC, Combined regular/injected simultaneous mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularInjectedSimultaneousDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Combined regular simultaneous alternate trigger mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using TripleAdcRegularSimultaneousAlternateTriggerDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
}
