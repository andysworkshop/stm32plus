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
   * Extension of the dual ADC feature to provide a third ADC. Only available on devices where
   * there is a third ADC.
   */

  template<class TAdc2,class TAdc3,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  class AdcTripleFeature : public AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay> {

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
    _adc3.reset(new TAdc3(&this->_adc));
  }


  /*
   * 3 ADC, Independent mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleIndependentNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleIndependentDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleIndependentDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleIndependentDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_Mode_Independent,TTwoSamplingDelay>;

  /*
   * 3 ADC, Injected simultaneous mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInjectedSimultaneousNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInjectedSimultaneousDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInjectedSimultaneousDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInjectedSimultaneousDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_InjecSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Regular simultaneous mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Interleaved mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInterleavedNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInterleavedDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInterleavedDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleInterleavedDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_Interl,TTwoSamplingDelay>;

  /*
   * 3 ADC, Alternate trigger mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleAlternateTriggerNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleAlternateTriggerDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleAlternateTriggerDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleAlternateTriggerDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_AlterTrig,TTwoSamplingDelay>;

  /*
   * 3 ADC, Combined regular/injected simultaneous mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularInjectedSimultaneousNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularInjectedSimultaneousDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularInjectedSimultaneousDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularInjectedSimultaneousDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult_InjecSimult,TTwoSamplingDelay>;

  /*
   * 3 ADC, Combined regular simultaneous alternate trigger mode
   */

  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousAlternateTriggerNoDmaFeature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_Disabled,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousAlternateTriggerDmaMode1Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_1,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousAlternateTriggerDmaMode2Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_2,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,class TAdc3,uint8_t TTwoSamplingDelay> using AdcTripleRegularSimultaneousAlternateTriggerDmaMode3Feature=AdcTripleFeature<TAdc2,TAdc3,ADC_DMAAccessMode_3,ADC_TripleMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
}
