/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Support for the temp sensor ADC channel is a simple extension of the regular channel
   * configuration.
   */

  template<uint8_t TSampleCycles,uint16_t TV25=Adc1PeripheralTraits::V25,uint16_t TAvgSlope=Adc1PeripheralTraits::AVG_SLOPE>   // from the datasheet (scaled)
  struct AdcTemperatureSensorFeature : AdcRegularChannelFeature<1,TSampleCycles,16> {

    /**
     * Constants used by the conversion function to get good accuracy from
     * the integer calculation
     */

    enum {
      SCALER = 1000
    };


    /**
     * Constructor, initialise upwards then enable the sensor feature
     */

    AdcTemperatureSensorFeature(Adc& adc)
      : AdcRegularChannelFeature<1,TSampleCycles,16>(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
      AdcRegularChannelFeature<1,TSampleCycles,16>::initialise();
      ADC_TempSensorVrefintCmd(ENABLE);
    }


    /**
     * Convert the value read from the ADC to a temperature in centigrade. We scale up the
     * necessary constants for the calculation to avoid floating point numbers.
     * @param vsense The value read from the ADC
     * @return The temperature in degrees C
     */

    uint8_t getTemperature(uint16_t vsense) const {

      uint32_t value;
      uint32_t v25= TV25*SCALER;

      uint16_t slope =TAvgSlope;
      value=(vsense*3300) / 0xfff;

      // scale up the sensed value by 1000

      value=value*SCALER;
      value=((v25-value)/slope)*SCALER+(25*SCALER);

      return value/SCALER;
    }
  };


  /*
   * Typedefs for the difference cycles on ADC1.
   */

  typedef AdcTemperatureSensorFeature<ADC_SampleTime_1Cycles5> Adc1Cycle1TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_7Cycles5> Adc1Cycle7TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_13Cycles5> Adc1Cycle13TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_28Cycles5> Adc1Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_41Cycles5> Adc1Cycle41TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_55Cycles5> Adc1Cycle55TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_71Cycles5> Adc1Cycle71TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_239Cycles5> Adc1Cycle239TemperatureSensorFeature;
}
