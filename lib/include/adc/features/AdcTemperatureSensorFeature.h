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

  template<uint8_t TSampleCycles>
  struct AdcTemperatureSensorFeature : AdcRegularChannelFeature<1,TSampleCycles,ADC_Channel_TempSensor> {

    /**
     * Constants used by the conversion function
     */

    enum {
      SCALER = 1000
    };


    /**
     * Constructor, initialise upwards then enable the sensor feature
     */

    AdcTemperatureSensorFeature(Adc& adc)
      : AdcRegularChannelFeature<1,TSampleCycles,ADC_Channel_TempSensor>(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
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

      value=(vsense*3300) & 0xfff;

      // scale up the sensed value by 1000

      value=value*SCALER;
      value=((value-Adc1PeripheralTraits::V25)/Adc1PeripheralTraits::AVG_SLOPE)+(25*SCALER);

      return value/SCALER;
    }
  };


#if defined(STM32PLUS_F4)

  /*
   * Typedefs for the difference cycles on ADC1. There's a minimum conversion time for the
   * temperature so the lower-cycle values are not present.
   */

  typedef AdcTemperatureSensorFeature<ADC_SampleTime_112Cycles> Adc1Cycle112TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_144Cycles> Adc1Cycle144TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_480Cycles> Adc1Cycle480TemperatureSensorFeature;

#elif defined(STM32PLUS_F0)

  /*
   * Note the sampling time must be greater than 2.2uS
   */

  typedef AdcTemperatureSensorFeature<ADC_SampleTime_1_5Cycles> Adc1Cycle1TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_7_5Cycles> Adc1Cycle7TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_13_5Cycles> Adc1Cycle13TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_28_5Cycles> Adc1Cycle28TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_41_5Cycles> Adc1Cycle41TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_55_5Cycles> Adc1Cycle55TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_71_5Cycles> Adc1Cycle71TemperatureSensorFeature;
  typedef AdcTemperatureSensorFeature<ADC_SampleTime_239_5Cycles> Adc1Cycle239TemperatureSensorFeature;

#else
#error "Unsupported MCU"
#endif
}
