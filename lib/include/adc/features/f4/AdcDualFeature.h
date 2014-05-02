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
  class AdcDualFeature : AdcMultiFeature<TDmaModeType,TAdcMultiModeType,TTwoSamplingDelay> {

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
    _adc2.reset(new TAdc2(AdcOperatingMode::MULTI_ADC));
  }


  /*
   * 2 ADC, Independent mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcIndependentNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcIndependentDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcIndependentDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_Mode_Independent,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcIndependentDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_Mode_Independent,TTwoSamplingDelay>;

  /*
   * 2 ADC, Injected simultaneous mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInjectedSimultaneousDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_InjecSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Regular simultaneous mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_RegSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_RegSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Interleaved mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInterleavedNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInterleavedDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInterleavedDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_Interl,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcInterleavedDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_Interl,TTwoSamplingDelay>;

  /*
   * 2 ADC, Alternate trigger mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcAlternateTriggerDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_AlterTrig,TTwoSamplingDelay>;

  /*
   * 2 ADC, Combined regular/injected simultaneous mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularInjectedSimultaneousDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_RegSimult_InjecSimult,TTwoSamplingDelay>;

  /*
   * 2 ADC, Combined regular simultaneous alternate trigger mode
   */

  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerNoDmaFeature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_Disabled,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerDmaMode1Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_1,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerDmaMode2Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_2,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
  template<class TAdc2,uint8_t TTwoSamplingDelay> using DualAdcRegularSimultaneousAlternateTriggerDmaMode3Feature=AdcDualFeature<TAdc2,ADC_DMAAccessMode_3,ADC_DualMode_RegSimult_AlterTrig,TTwoSamplingDelay>;
}
