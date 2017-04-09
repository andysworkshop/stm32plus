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
   * Base class for the injected channel feature. Members that don't need to
   * be a part of the template are here.
   */

  class AdcInjectedChannelFeatureBase : public AdcFeatureBase {

    protected:
      uint8_t channelIndexToChannelNumber(uint8_t channelIndex) const;

    public:
      AdcInjectedChannelFeatureBase(Adc& adc);

      void setInjectedChannelOffset(uint8_t channelIndex,uint16_t offset);

      void enableAutoInjectedMode() const;
      void disableAutoInjectedMode() const;

      void enableDiscontinuousInjectedMode() const;
      void disableDiscontinuousInjectedMode() const;

      void startInjectedConversion() const;
      bool hasInjectedConversionStarted() const;
      bool hasInjectedConversionFinished() const;
      uint16_t getInjectedConversionValue(uint8_t channelIndex) const;
  };


  /**
   * Constructor
   */

  inline AdcInjectedChannelFeatureBase::AdcInjectedChannelFeatureBase(Adc& adc)
    : AdcFeatureBase(adc) {
  }


  /**
   * Set the 12-bit offset for the conversion values of an injected channel. This value is subtracted
   * from the converted value for the specified channel. This means that the converted values can
   * be negative.
   * @param channel The channel number (0..3)
   * @param offset The 12-bit offset
   */

  inline void AdcInjectedChannelFeatureBase::setInjectedChannelOffset(uint8_t channel,uint16_t offset) {

    ADC_SetInjectedOffset(_adc,channelIndexToChannelNumber(channel),offset);
  }


  /**
   * The peripheral library confusingly uses channel indexes for setup and then some magic
   * constants for access later. We'll still to indices and map to constants where the
   * API needs them.
   */

  inline uint8_t AdcInjectedChannelFeatureBase::channelIndexToChannelNumber(uint8_t channelIndex) const {

    return channelIndex==0 ? ADC_InjectedChannel_1 :
           channelIndex==1 ? ADC_InjectedChannel_2 :
           channelIndex==2 ? ADC_InjectedChannel_3 :
          ADC_InjectedChannel_4;
  }


  /**
   * Start the conversion by software command. This will set JSWSTART in CR2.
   */

  inline void AdcInjectedChannelFeatureBase::startInjectedConversion() const {
    ADC_SoftwareStartInjectedConvCmd(_adc,ENABLE);
  }


  /**
   * Return true if a software conversion has started. JSWSTART in CR2 is cleared
   * by the MCU when the conversion starts
   * @return true if the conversion has started.
   */

  inline bool AdcInjectedChannelFeatureBase::hasInjectedConversionStarted() const {
    return ADC_GetSoftwareStartInjectedConvCmdStatus(_adc)==RESET;
  }


  /**
   * Return true if a conversion has finished. Returns the status of the JEOC flag
   * @return true if the conversion has finished.
   */

  inline bool AdcInjectedChannelFeatureBase::hasInjectedConversionFinished() const {
    return ADC_GetFlagStatus(_adc,ADC_FLAG_JEOC)==SET;
  }


  /**
   * Get the result of an injected conversion
   * @param channelIndex The 0..3 index of the injected channel
   * @return The conversion result
   */

  inline uint16_t AdcInjectedChannelFeatureBase::getInjectedConversionValue(uint8_t channelIndex) const {
    return ADC_GetInjectedConversionValue(_adc,channelIndexToChannelNumber(channelIndex));
  }


  /**
   * Enable auto-injected mode (injected channels converted after regular)
   */

  inline void AdcInjectedChannelFeatureBase::enableAutoInjectedMode() const {
    ADC_AutoInjectedConvCmd(_adc,ENABLE);
  }


  /**
   * Disable auto-injected mode (injected channels converted after regular)
   */

  inline void AdcInjectedChannelFeatureBase::disableAutoInjectedMode() const {
    ADC_AutoInjectedConvCmd(_adc,DISABLE);
  }


  /**
   * Enable discontinuous injected mode (one-by-one conversion of a group)
   */

  inline void AdcInjectedChannelFeatureBase::enableDiscontinuousInjectedMode() const {
    ADC_InjectedDiscModeCmd(_adc,ENABLE);
  }


  /**
   * Disable discontinuous injected mode (one-by-one conversion of a group)
   */

  inline void AdcInjectedChannelFeatureBase::disableDiscontinuousInjectedMode() const {
    ADC_InjectedDiscModeCmd(_adc,DISABLE);
  }
}
