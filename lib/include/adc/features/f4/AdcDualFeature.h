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
   * The dual ADC feature declares an instance of the second ADC and holds
   * it locally
   */

  template<class TAdc2,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  class AdcDualFeature : public AdcMultiFeature<TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay> {

    protected:
      scoped_ptr<TAdc2> _adc2;

    public:
      AdcDualFeature(Adc& adc);

      void initialise();

      TAdc2& getAdc2() const;
      uint32_t getDualConversionValue() const;
  };


  /**
   * Constructor
   * @param adc The adc instance
   */

  template<class TAdc2,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::AdcDualFeature(Adc& adc)
    : AdcMultiFeature<TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>(adc) {
  }


  /**
   * Get the results of a dual-conversion
   * @return The results of the conversion
   */

  template<class TAdc2,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline uint32_t AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::getDualConversionValue() const {
    return ADC_GetMultiModeConversionValue();
  }


  /**
   * Get a reference to the second ADC
   * @return the second ADC reference
   */

  template<class TAdc2,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline TAdc2& AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::getAdc2() const {
    return *_adc2.get();
  }


  /**
   * Post ADC_Init() initialisation
   */

  template<class TAdc2,uint32_t TDmaModeType,uint32_t TAdcMultiModeType,uint8_t TTwoSamplingDelay>
  inline void AdcDualFeature<TAdc2,TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay>::initialise() {
    _adc2.reset(new TAdc2(&this->_adc));
  }


  /*
   * 2 ADC, Independent mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualIndependentNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualIndependentDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualIndependentDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualIndependentDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_Mode_Independent,TTwoSamplingDelay>;

  /*
   * 2 ADC, Injected simultaneous mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInjectedSimultaneousNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInjectedSimultaneousDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInjectedSimultaneousDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInjectedSimultaneousDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Regular simultaneous mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_RegSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Interleaved mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInterleavedNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInterleavedDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInterleavedDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualInterleavedDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_Interl,TTwoSamplingDelay>;

  /*
   * 2 ADC, Alternate trigger mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualAlternateTriggerNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualAlternateTriggerDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualAlternateTriggerDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualAlternateTriggerDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;

  /*
   * 2 ADC, Combined regular/injected simultaneous mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularInjectedSimultaneousNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularInjectedSimultaneousDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularInjectedSimultaneousDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularInjectedSimultaneousDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Combined regular simultaneous alternate trigger mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousAlternateTriggerNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousAlternateTriggerDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousAlternateTriggerDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using AdcDualRegularSimultaneousAlternateTriggerDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
}
