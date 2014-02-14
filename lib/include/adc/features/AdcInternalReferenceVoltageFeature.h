/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Support for the Vrefint ADC channel is a simple extension of the regular channel
   * configuration
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles>
  struct AdcInternalReferenceVoltageFeature : AdcRegularChannelFeature<TAdcNumber,TSampleCycles,ADC_Channel_Vbat> {

    /**
     * Constructor, initialise upwards then enable the vrefint feature
     */

    AdcInternalReferenceVoltageFeature(Adc& adc)
      : AdcRegularChannelFeature<TAdcNumber,TSampleCycles,ADC_Channel_Vbat>(adc) {

      ADC_TempSensorVrefintCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_3Cycles,TChannelNumbers> Adc1Cycle3InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_15Cycles,TChannelNumbers> Adc1Cycle15InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_28Cycles,TChannelNumbers> Adc1Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_56Cycles,TChannelNumbers> Adc1Cycle56InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_84Cycles,TChannelNumbers> Adc1Cycle84InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_112Cycles,TChannelNumbers> Adc1Cycle112InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_144Cycles,TChannelNumbers> Adc1Cycle144InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_480Cycles,TChannelNumbers> Adc1Cycle480InternalReferenceVoltageFeature;

  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_3Cycles,TChannelNumbers> Adc2Cycle3InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_15Cycles,TChannelNumbers> Adc2Cycle15InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_28Cycles,TChannelNumbers> Adc2Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_56Cycles,TChannelNumbers> Adc2Cycle56InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_84Cycles,TChannelNumbers> Adc2Cycle84InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_112Cycles,TChannelNumbers> Adc2Cycle112InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_144Cycles,TChannelNumbers> Adc2Cycle144InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_480Cycles,TChannelNumbers> Adc2Cycle480InternalReferenceVoltageFeature;

  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_3Cycles,TChannelNumbers> Adc3Cycle3InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_15Cycles,TChannelNumbers> Adc3Cycle15InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_28Cycles,TChannelNumbers> Adc3Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_56Cycles,TChannelNumbers> Adc3Cycle56InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_84Cycles,TChannelNumbers> Adc3Cycle84InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_112Cycles,TChannelNumbers> Adc3Cycle112InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_144Cycles,TChannelNumbers> Adc3Cycle144InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_480Cycles,TChannelNumbers> Adc3Cycle480InternalReferenceVoltageFeature;
}
