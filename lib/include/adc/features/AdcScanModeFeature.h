/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Trivial feature class to enable scan-mode
   */

  struct AdcScanModeFeature : AdcFeatureBase {

    AdcScanModeFeature(Adc& adc)
      : AdcFeatureBase(adc) {
      ((ADC_InitTypeDef *)adc)->ADC_ScanConvMode=ENABLE;
    }
  };
}
