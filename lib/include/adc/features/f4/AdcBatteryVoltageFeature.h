/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Support for the VBAT ADC channel is a simple extension of the regular channel
   * configuration
   */

  template<uint8_t TSampleCycles>
  struct AdcBatteryVoltageFeature : AdcRegularChannelFeature<1,TSampleCycles,18> {

    /**
     * Constructor, initialise upwards then enable the VBAT feature
     */

    AdcBatteryVoltageFeature(Adc& adc)
      : AdcRegularChannelFeature<1,TSampleCycles,18>(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
      AdcRegularChannelFeature<1,TSampleCycles,18>::initialise();
      ADC_VBATCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on each ADC
   */

  typedef AdcBatteryVoltageFeature<ADC_SampleTime_3Cycles> Adc1Cycle3BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_15Cycles> Adc1Cycle15BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_28Cycles> Adc1Cycle28BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_56Cycles> Adc1Cycle56BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_84Cycles> Adc1Cycle84BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_112Cycles> Adc1Cycle112BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_144Cycles> Adc1Cycle144BatteryVoltageFeature;
  typedef AdcBatteryVoltageFeature<ADC_SampleTime_480Cycles> Adc1Cycle480BatteryVoltageFeature;
}
