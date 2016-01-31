/*
 * This file is a part of the open source Arduino Graphics Coprocessor project
 * Copyright (c) 2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Feature template for enabling a second slave address. The address mask allows
   * less than the full 8 bits to be compared (see OA2MSK).
   * @tparam TSecondSlaveAddress Your second slave address.
   */

  template<uint8_t TSecondSlaveAddress>
  struct I2CSecondSlaveFeature : public I2CFeatureBase {
    I2CSecondSlaveFeature(I2C& i2c);
  };


  /**
   * Constructor
   */

  template<uint8_t TSecondSlaveAddress>
  I2CSecondSlaveFeature<TSecondSlaveAddress>::I2CSecondSlaveFeature(I2C& i2c)
    : I2CFeatureBase(i2c) {

    // configure the values

    I2C_OwnAddress2Config(_i2c,TSecondSlaveAddress);
    I2C_DualAddressCmd(_i2c,ENABLE);
  }
}
