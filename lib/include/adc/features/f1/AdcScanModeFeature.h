/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * Feature class to enable scan-mode. EOC is raised after each channel conversion. Typically
   * you'll use DMA to shift out the data as each EOC is raised.
   */

  struct AdcScanModeFeature : AdcFeatureBase {

    AdcScanModeFeature(Adc& adc)
      : AdcFeatureBase(adc) {
      ((ADC_InitTypeDef *)adc)->ADC_ScanConvMode=ENABLE;
    }


    /**
     * Post ADC_Init() initialisation
     */

    void initialise() {
    }
  };
}
