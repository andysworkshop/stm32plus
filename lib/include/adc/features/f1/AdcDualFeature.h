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
   * The dual ADC feature declares an instance of the second ADC and holds
   * it locally
   */

  template<class TAdc2,uint32_t TAdcMultiModeType>
  class AdcDualFeature : AdcFeatureBase {

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

  template<class TAdc2,uint32_t TAdcMultiModeType>
  inline AdcDualFeature<TAdc2,TAdcMultiModeType>::AdcDualFeature(Adc& adc)
    : AdcFeatureBase(adc) {

    ((ADC_InitTypeDef *)adc)->ADC_Mode=TAdcMultiModeType;
  }


  /**
   * Get the results of a dual-conversion
   * @return The results of the conversion
   */

  template<class TAdc2,uint32_t TAdcMultiModeType>
  inline uint32_t AdcDualFeature<TAdc2,TAdcMultiModeType>::getDualConversionValue() const {
    return ADC_GetDualModeConversionValue();
  }


  /**
   * Get a reference to the second ADC
   * @return the second ADC reference
   */

  template<class TAdc2,uint32_t TAdcMultiModeType>
  inline TAdc2& AdcDualFeature<TAdc2,TAdcMultiModeType>::getAdc2() const {
    return *_adc2.get();
  }


  /**
   * Post ADC_Init() initialisation
   */

  template<class TAdc2,uint32_t TAdcMultiModeType>
  inline void AdcDualFeature<TAdc2,TAdcMultiModeType>::initialise() {
    _adc2.reset(new TAdc2(AdcOperatingMode::MULTI_ADC));
  }


  /*
   * 2 ADC, Independent mode
   */

  template<class TAdc2> using DualAdcIndependentFeature=AdcDualFeature<TAdc2,ADC_Mode_Independent>;
  template<class TAdc2> using DualAdcRegularInjectedSimultaneousFeature=AdcDualFeature<TAdc2,ADC_Mode_RegInjecSimult>;
  template<class TAdc2> using DualAdcRegularSimultaneousAlternateTriggerFeature=AdcDualFeature<TAdc2,ADC_Mode_RegSimult_AlterTrig>;
  template<class TAdc2> using DualAdcInjectedSimultaneousFastInterleavedFeature=AdcDualFeature<TAdc2,ADC_Mode_InjecSimult_FastInterl>;
  template<class TAdc2> using DualAdcInjectedSimultaneousSlowInterleavedFeature=AdcDualFeature<TAdc2,ADC_Mode_InjecSimult_SlowInterl>;
  template<class TAdc2> using DualAdcInjectedSimultaneousFeature=AdcDualFeature<TAdc2,ADC_Mode_InjecSimult>;
  template<class TAdc2> using DualAdcRegularSimultaneousFeature=AdcDualFeature<TAdc2,ADC_Mode_RegSimult>;
  template<class TAdc2> using DualAdcFastInterleavedFeature=AdcDualFeature<TAdc2,ADC_Mode_FastInterl>;
  template<class TAdc2> using DualAdcSlowInterleavedFeature=AdcDualFeature<TAdc2,ADC_Mode_SlowInterl>;
  template<class TAdc2> using DualAdcAlternateTriggerFeature=AdcDualFeature<TAdc2,ADC_Mode_AlterTrig>;
}
