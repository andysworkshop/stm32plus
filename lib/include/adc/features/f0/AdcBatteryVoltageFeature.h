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
   * Support for the VBAT ADC channel is a simple extension of the regular channel
   * configuration
   */

  template<uint8_t TSampleCycles>
  struct AdcBatteryVoltageFeature : AdcFeatureBase {

    /**
     * Constructor, initialise upwards then enable the VBAT feature
     */

    AdcBatteryVoltageFeature(Adc& adc)
      : AdcFeatureBase(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
      ADC_ChannelConfig(_adc,ADC_Channel_18,TSampleCycles);
      ADC_VbatCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  typedef AdcBatteryVoltageFeature<ADC_SampleTime_1_5Cycles> Adc1Cycle1BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_7_5Cycles> Adc1Cycle7BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_13_5Cycles> Adc1Cycle13BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_28_5Cycles> Adc1Cycle28BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_41_5Cycles> Adc1Cycle41BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_55_5Cycles> Adc1Cycle55BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_71_5Cycles> Adc1Cycle71BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_239_5Cycles> Adc1Cycle239BatteryVoltageFeature;
}
