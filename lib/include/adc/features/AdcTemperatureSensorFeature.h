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
      : AdcTemperatureSensorFeature<TAdcNumber,TSampleCycles,ADC_Channel_TempSensor>(adc) {

      ADC_TempSensorVrefintCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_3Cycles,TChannelNumbers> Adc1Cycle3TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_15Cycles,TChannelNumbers> Adc1Cycle15TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_28Cycles,TChannelNumbers> Adc1Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_56Cycles,TChannelNumbers> Adc1Cycle56TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_84Cycles,TChannelNumbers> Adc1Cycle84TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_112Cycles,TChannelNumbers> Adc1Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_144Cycles,TChannelNumbers> Adc1Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<1,ADC_SampleTime_480Cycles,TChannelNumbers> Adc1Cycle480TemperatureSensorFeature;

  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_3Cycles,TChannelNumbers> Adc2Cycle3TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_15Cycles,TChannelNumbers> Adc2Cycle15TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_28Cycles,TChannelNumbers> Adc2Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_56Cycles,TChannelNumbers> Adc2Cycle56TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_84Cycles,TChannelNumbers> Adc2Cycle84TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_112Cycles,TChannelNumbers> Adc2Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_144Cycles,TChannelNumbers> Adc2Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<2,ADC_SampleTime_480Cycles,TChannelNumbers> Adc2Cycle480TemperatureSensorFeature;

  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_3Cycles,TChannelNumbers> Adc3Cycle3TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_15Cycles,TChannelNumbers> Adc3Cycle15TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_28Cycles,TChannelNumbers> Adc3Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_56Cycles,TChannelNumbers> Adc3Cycle56TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_84Cycles,TChannelNumbers> Adc3Cycle84TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_112Cycles,TChannelNumbers> Adc3Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_144Cycles,TChannelNumbers> Adc3Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<3,ADC_SampleTime_480Cycles,TChannelNumbers> Adc3Cycle480TemperatureSensorFeature;
}
