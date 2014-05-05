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
   * Non-template base class for the ADC peripheral
   */

  class Adc {

    protected:
      ADC_InitTypeDef *_init;

    public:
      Adc();

      void enablePeripheral() const;
      void disablePeripheral() const;

      // regular conversion data access functions

      void startRegularConversion() const;
      void stopRegularConversion() const;
      bool hasRegularConversionStarted() const;
      bool hasRegularConversionFinished() const;
      uint16_t getRegularConversionValue() const;

      // cast operators

      operator ADC_TypeDef *();
      operator ADC_InitTypeDef *();
  };


  /**
   * Constructor
   * @param peripheralAddress The peripheral address
   */

  inline Adc::Adc() {

    // set up the default init values
    // the features can customise this before the AdcPeripheral class uses
    // it and frees the memory it used

    _init=new ADC_InitTypeDef;
    ADC_StructInit(_init);
  }


  /**
   * Return the ADC peripheral address
   */

  inline Adc::operator ADC_TypeDef *() {
    return ADC1;
  }


  /**
   * Return the ADC init structure
   */

  inline Adc::operator ADC_InitTypeDef *() {
    return _init;
  }


  /**
   * Enable the ADC
   */

  inline void Adc::enablePeripheral() const {
    ADC_Cmd(ADC1,ENABLE);
  }


  /**
   * Disable the ADC
   */

  inline void Adc::disablePeripheral() const {
    ADC_Cmd(ADC1,DISABLE);
  }


  /**
   * Start the conversion by software command.
   */

  inline void Adc::startRegularConversion() const {
    ADC_StartOfConversion(ADC1);
  }


  /**
   * Stop the conversion by software command.
   */

  inline void Adc::stopRegularConversion() const {
    ADC_StopOfConversion(ADC1);
  }


  /**
   * Return true if a conversion has finished. Returns the status of the EOC flag
   * @return true if the conversion has finished.
   */

  inline bool Adc::hasRegularConversionFinished() const {
    return ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET;
  }


  /**
   * Get the result of a regular conversion
   * @return The conversion result
   */

  inline uint16_t Adc::getRegularConversionValue() const {
    return ADC_GetConversionValue(ADC1);
  }
}
