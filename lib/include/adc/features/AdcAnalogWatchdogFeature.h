/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Support for configuring the analog watchdog feature. The AWD allows you to
   * trigger an interrupt if a voltage crosses pre-defined upper or lower bounds.
   *
   * This is a base class that supports the threshold setting ability common to
   * all MCUs
   */

  template<uint16_t TLow,uint16_t THigh>
  struct AdcAnalogWatchdogFeature : public AdcFeatureBase {


    /**
     * Constructor
     * @param adc reference to main ADC instance
     */

    AdcAnalogWatchdogFeature(Adc& adc)
      : AdcFeatureBase(adc) {
    }


    /**
     * Set the thresholds, post initialisation
     */

    void initialise() {
      setWatchdogThresholds(TLow,THigh);
    }


    /**
     * Set the thresholds for the watchdog. Allows adjustment post-construction if required.
     * @param low lower threshold
     * @param high higher threshold
     */

    void setWatchdogThresholds(uint16_t low,uint16_t high) {
      ADC_AnalogWatchdogThresholdsConfig(_adc,high,low);
    }
  };
}
