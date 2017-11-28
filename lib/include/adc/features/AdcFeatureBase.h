/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for ADC features
   */

  class AdcFeatureBase {
    protected:
      Adc& _adc;

    public:
      AdcFeatureBase(Adc& adc);
      operator Adc&();
  };


  /**
   * Constructor
   * @param adc
   */

  inline AdcFeatureBase::AdcFeatureBase(Adc& adc)
    : _adc(adc) {
  }


  /**
   * Cast to Adc reference
   */

  inline AdcFeatureBase::operator Adc&() {
    return _adc;
  }
}
