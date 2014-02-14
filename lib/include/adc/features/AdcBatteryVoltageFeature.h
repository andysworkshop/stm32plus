/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Support for the VBAT ADC channel is a simple extension of the regular channel
   * configuration
   */

  template<uint8_t TAdcNumber,uint8_t TSampleCycles>
  struct AdcBatteryVoltageFeature : AdcRegularChannelFeature<TAdcNumber,TSampleCycles,ADC_Channel_Vbat> {

    /**
     * Constructor, initialise upwards then enable the VBAT feature
     */

    AdcBatteryVoltageFeature(Adc& adc)
      : AdcRegularChannelFeature<TAdcNumber,TSampleCycles,ADC_Channel_Vbat>(adc) {

      ADC_VBATCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_3Cycles,TChannelNumbers> Adc1Cycle3BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_15Cycles,TChannelNumbers> Adc1Cycle15BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_28Cycles,TChannelNumbers> Adc1Cycle28BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_56Cycles,TChannelNumbers> Adc1Cycle56BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_84Cycles,TChannelNumbers> Adc1Cycle84BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_112Cycles,TChannelNumbers> Adc1Cycle112BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_144Cycles,TChannelNumbers> Adc1Cycle144BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<1,ADC_SampleTime_480Cycles,TChannelNumbers> Adc1Cycle480BatteryVoltageFeature;

  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_3Cycles,TChannelNumbers> Adc2Cycle3BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_15Cycles,TChannelNumbers> Adc2Cycle15BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_28Cycles,TChannelNumbers> Adc2Cycle28BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_56Cycles,TChannelNumbers> Adc2Cycle56BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_84Cycles,TChannelNumbers> Adc2Cycle84BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_112Cycles,TChannelNumbers> Adc2Cycle112BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_144Cycles,TChannelNumbers> Adc2Cycle144BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<2,ADC_SampleTime_480Cycles,TChannelNumbers> Adc2Cycle480BatteryVoltageFeature;

  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_3Cycles,TChannelNumbers> Adc3Cycle3BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_15Cycles,TChannelNumbers> Adc3Cycle15BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_28Cycles,TChannelNumbers> Adc3Cycle28BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_56Cycles,TChannelNumbers> Adc3Cycle56BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_84Cycles,TChannelNumbers> Adc3Cycle84BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_112Cycles,TChannelNumbers> Adc3Cycle112BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_144Cycles,TChannelNumbers> Adc3Cycle144BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<3,ADC_SampleTime_480Cycles,TChannelNumbers> Adc3Cycle480BatteryVoltageFeature;
}
