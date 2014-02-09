/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for I2S features
   */

  class I2SFeatureBase {
    protected:
      I2S& _i2s;

    public:
      I2SFeatureBase(I2S& _i2s);
      operator I2S&();
  };


  /**
   * Constructor
   * @param spi
   */

  inline I2SFeatureBase::I2SFeatureBase(I2S& i2s)
    : _i2s(i2s) {
  }


  /**
   * Cast to I2S reference
   */

  inline I2SFeatureBase::operator I2S&() {
    return _i2s;
  }
}
