/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Support for the temp sensor ADC channel is a simple extension of the regular channel
   * configuration
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles>
  struct AdcTemperatureSensorFeature : AdcRegularChannelFeature<TAdcNumber,TSampleCycles,ADC_Channel_TempSensor> {

    /**
     * Constructor, initialise upwards then enable the sensor feature
     */

    AdcTemperatureSensorFeature(Adc& adc)
      : AdcRegularChannelFeature<TAdcNumber,TSampleCycles,ADC_Channel_TempSensor>(adc) {

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

  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_3Cycles> Adc1Cycle3TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_15Cycles> Adc1Cycle15TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_28Cycles> Adc1Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_56Cycles> Adc1Cycle56TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_84Cycles> Adc1Cycle84TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_112Cycles> Adc1Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_144Cycles> Adc1Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_480Cycles> Adc1Cycle480TemperatureSensorFeature;

  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_3Cycles> Adc2Cycle3TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_15Cycles> Adc2Cycle15TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_28Cycles> Adc2Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_56Cycles> Adc2Cycle56TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_84Cycles> Adc2Cycle84TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_112Cycles> Adc2Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_144Cycles> Adc2Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_480Cycles> Adc2Cycle480TemperatureSensorFeature;

  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_3Cycles> Adc3Cycle3TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_15Cycles> Adc3Cycle15TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_28Cycles> Adc3Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_56Cycles> Adc3Cycle56TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_84Cycles> Adc3Cycle84TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_112Cycles> Adc3Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_144Cycles> Adc3Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_480Cycles> Adc3Cycle480TemperatureSensorFeature;
}
