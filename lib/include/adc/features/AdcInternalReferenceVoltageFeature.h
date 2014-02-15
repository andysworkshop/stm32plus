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


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
    }
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_3Cycles> Adc1Cycle3InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_15Cycles> Adc1Cycle15InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_28Cycles> Adc1Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_56Cycles> Adc1Cycle56InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_84Cycles> Adc1Cycle84InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_112Cycles> Adc1Cycle112InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_144Cycles> Adc1Cycle144InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<1,ADC_SampleTime_480Cycles> Adc1Cycle480InternalReferenceVoltageFeature;

  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_3Cycles> Adc2Cycle3InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_15Cycles> Adc2Cycle15InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_28Cycles> Adc2Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_56Cycles> Adc2Cycle56InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_84Cycles> Adc2Cycle84InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_112Cycles> Adc2Cycle112InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_144Cycles> Adc2Cycle144InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<2,ADC_SampleTime_480Cycles> Adc2Cycle480InternalReferenceVoltageFeature;

  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_3Cycles> Adc3Cycle3InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_15Cycles> Adc3Cycle15InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_28Cycles> Adc3Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_56Cycles> Adc3Cycle56InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_84Cycles> Adc3Cycle84InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_112Cycles> Adc3Cycle112InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_144Cycles> Adc3Cycle144InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<3,ADC_SampleTime_480Cycles> Adc3Cycle480InternalReferenceVoltageFeature;
}
