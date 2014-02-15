/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Non-template base class for the ADC peripheral
   */

  class Adc {

    protected:
     ADC_TypeDef *_peripheralAddress;
     ADC_CommonInitTypeDef *_commonInit;
     ADC_InitTypeDef *_init;

     static uint8_t _regularChannelRank[3];        // we can have multiple channel feature instances and multiple ADCs

    public:
      Adc(ADC_TypeDef *peripheralAddress);

      void enablePeripheral() const;
      void disablePeripheral() const;

      // get and increment the rank for an ADC

      uint8_t getAndIncrementRegularChannelRank(uint8_t adcNumber);

      // injected channel start/check functionality is in a feature class due to lack of support on all MCUs

      void startRegularConversion() const;
      bool hasRegularConversionStarted() const;
      bool hasRegularConversionFinished() const;
      uint16_t getRegularConversionValue() const;

      // cast operators

      operator ADC_TypeDef *();
      operator ADC_CommonInitTypeDef *();
      operator ADC_InitTypeDef *();
  };


  /**
   * Constructor
   * @param peripheralAddress The peripheral address
   */

  inline Adc::Adc(ADC_TypeDef *peripheralAddress)
    : _peripheralAddress(peripheralAddress) {

    // initialise the ranks back to 1 so that the channel features are ready

    _regularChannelRank[0]=_regularChannelRank[1]=_regularChannelRank[2]=1;

    // set up the default init values
    // the features can customise this before the AdcPeripheral class uses
    // it and frees the memory it used

    _init=new ADC_InitTypeDef;

    _init->ADC_Resolution=ADC_Resolution_12b;
    _init->ADC_ScanConvMode=DISABLE;
    _init->ADC_ContinuousConvMode=DISABLE;
    _init->ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
    _init->ADC_ExternalTrigConv=ADC_ExternalTrigConv_T1_CC1;
    _init->ADC_DataAlign=ADC_DataAlign_Right;
    _init->ADC_NbrOfConversion=0;

    // set up the default common init values
    // the features can customise this before the AdcPeripheral class uses
    // it and frees the memory it used

    _commonInit=new ADC_CommonInitTypeDef;

    _commonInit->ADC_Mode=ADC_Mode_Independent;
    _commonInit->ADC_Prescaler=ADC_Prescaler_Div2;
    _commonInit->ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;
    _commonInit->ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles;
  }


  /**
   * Get the rank (order) number for a regular channel
   * @param adcNumber The ADC number (1..3)
   * @return the current rank number
   */

  inline uint8_t Adc::getAndIncrementRegularChannelRank(uint8_t adcNumber) {
    return _regularChannelRank[adcNumber-1]++;
  }


  /**
   * Return the ADC peripheral address
   */

  inline Adc::operator ADC_TypeDef *() {
    return _peripheralAddress;
  }


  /**
   * Return the ADC common init structure
   */

  inline Adc::operator ADC_CommonInitTypeDef *() {
    return _commonInit;
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
    ADC_Cmd(_peripheralAddress,ENABLE);
  }


  /**
   * Disable the ADC
   */

  inline void Adc::disablePeripheral() const {
    ADC_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Start the conversion by software command. This will set SWSTART in CR2.
   */

  inline void Adc::startRegularConversion() const {
    ADC_SoftwareStartConv(_peripheralAddress);
  }


  /**
   * Return true if a software conversion has started. SWSTART in CR2 is cleared
   * by the MCU when the conversion starts
   * @return true if the conversion has started.
   */

  inline bool Adc::hasRegularConversionStarted() const {
    return ADC_GetSoftwareStartConvStatus(_peripheralAddress)==RESET;
  }


  /**
   * Return true if a conversion has finished. Returns the status of the EOC flag
   * @return true if the conversion has finished.
   */

  inline bool Adc::hasRegularConversionFinished() const {
    return ADC_GetFlagStatus(_peripheralAddress,ADC_FLAG_EOC)==SET;
  }


  /**
   * Get the result of a regular conversion
   * @return The conversion result
   */

  inline uint16_t Adc::getRegularConversionValue() const {
    return ADC_GetConversionValue(_peripheralAddress);
  }
}
