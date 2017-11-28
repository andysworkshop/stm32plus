/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Support for the Vrefint ADC channel is a simple extension of the regular channel
   * configuration
   */

  template<uint8_t TSampleCycles>
  struct AdcInternalReferenceVoltageFeature : AdcRegularChannelFeature<1,TSampleCycles,17> {

    /**
     * Constructor, initialise upwards then enable the vrefint feature
     */

    AdcInternalReferenceVoltageFeature(Adc& adc)
      : AdcRegularChannelFeature<1,TSampleCycles,17>(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
      AdcRegularChannelFeature<1,TSampleCycles,17>::initialise();
      ADC_TempSensorVrefintCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on ADC1
   */

  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_1Cycles5> Adc1Cycle1InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_7Cycles5> Adc1Cycle7InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_13Cycles5> Adc1Cycle13InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_28Cycles5> Adc1Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_41Cycles5> Adc1Cycle41InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_55Cycles5> Adc1Cycle55InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_71Cycles5> Adc1Cycle71InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_239Cycles5> Adc1Cycle239InternalReferenceVoltageFeature;
}
