/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for i2c features
   */

  class I2CFeatureBase {
    protected:
      I2C& _i2c;

    public:
      I2CFeatureBase(I2C& i2c);
      operator I2C&();
  };


  /**
   * Constructor
   * @param i2c
   */

  inline I2CFeatureBase::I2CFeatureBase(I2C& i2c)
    : _i2c(i2c) {
  }


  /**
   * Cast to I2C reference
   */

  inline I2CFeatureBase::operator I2C&() {
    return _i2c;
  }
}
