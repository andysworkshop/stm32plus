/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for the ADC regular channel features
   */

  class AdcRegularChannelFeatureBase : public AdcFeatureBase {

    protected:
      static uint8_t _rank;                // we can have multiple instances of this feature

    public:
      AdcRegularChannelFeatureBase(Adc& adc);
  };


  /**
   * Constructor
   */

  inline AdcRegularChannelFeatureBase::AdcRegularChannelFeatureBase(Adc& adc)
    : AdcFeatureBase(adc) {
  }
}
