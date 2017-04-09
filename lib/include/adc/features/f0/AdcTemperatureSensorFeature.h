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
   * Support for the temp sensor ADC channel
   */

  template<uint8_t TSampleCycles,uint16_t TV25=1963>        // default to 1.43V at 3.0V
  struct AdcTemperatureSensorFeature : AdcFeatureBase {

    /**
     * Constant used by the conversion function to improve accuracy
     * of the integer calculations.
     */

    enum {
      SCALER = 1000
    };


    /**
     * Constructor, initialise upwards then enable the sensor feature
     */

    AdcTemperatureSensorFeature(Adc& adc)
      : AdcFeatureBase(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
      ADC_ChannelConfig(_adc,ADC_Channel_16,TSampleCycles);
      ADC_TempSensorCmd(ENABLE);
    }


    /**
     * Convert the value read from the ADC to a temperature in centigrade. We scale up the
     * necessary constants for the calculation to avoid floating point numbers.
     * @param vsense The value read from the ADC
     * @return The temperature in degrees C
     */

    uint8_t getTemperature(int16_t vsense) const {

      int32_t vsense30,vsense110,slope;

      // get the values for 30 and 110 degrees stored by ST in system memory
      // the scaled calculations get us decent accuracy without having to resort
      // to expensive floating point calculations.

      vsense30=*reinterpret_cast<const int16_t *>(0x1FFFF7B8)*SCALER;
      vsense110=*reinterpret_cast<const int16_t *>(0x1FFFF7C2)*SCALER;

      slope=(vsense30-vsense110)/80;

      return ((((TV25*SCALER)-(vsense*SCALER))/slope)/SCALER)+25;
    }
  };


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
}
