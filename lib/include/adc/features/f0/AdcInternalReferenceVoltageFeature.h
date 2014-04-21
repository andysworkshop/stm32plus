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
   * Support for the Vrefint ADC channel
   */

  template<uint8_t TSampleCycles>
  struct AdcInternalReferenceVoltageFeature : AdcFeatureBase {

    /**
     * Constructor, initialise upwards then enable the vrefint feature
     */

    AdcInternalReferenceVoltageFeature(Adc& adc)
      : AdcFeatureBase(adc) {
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
      ADC_ChannelConfig(_adc,ADC_Channel_17,TSampleCycles);
      ADC_VrefintCmd(ENABLE);
    }
  };


  /*
   * Typedefs for the difference cycles on ADC1
   */

  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_1_5Cycles> Adc1Cycle1InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_7_5Cycles> Adc1Cycle7InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_13_5Cycles> Adc1Cycle13InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_28_5Cycles> Adc1Cycle28InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_41_5Cycles> Adc1Cycle41InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_55_5Cycles> Adc1Cycle55InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_71_5Cycles> Adc1Cycle71InternalReferenceVoltageFeature;
  typedef AdcInternalReferenceVoltageFeature<ADC_SampleTime_239_5Cycles> Adc1Cycle239InternalReferenceVoltageFeature;
}
