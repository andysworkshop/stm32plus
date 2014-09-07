/*
 * This file is a part of the open source Arduino Graphics Coprocessor project
 * Copyright (c) 2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * Feature template for enabling a second slave address. The address mask allows
   * less than the full 8 bits to be compared (see OA2MSK).
   * @tparam TSecondSlaveAddress Your second slave address.
   * @tparam TAddressMask The number of bits to use. Can be I2C_OA2_Mask01..07 or
   * I2C_OA2_NoMask for all bits. For 01..07 values the bits from 01..07 are masked off
   * (not used) as requested. Defaults to I2C_OA2_NoMask for code compatibility with the
   * F1 where masks are not supported.
   */

  template<uint8_t TSecondSlaveAddress,uint8_t TAddressMask=I2C_OA2_NoMask>
  struct I2CSecondSlaveFeature : public I2CFeatureBase {
    I2CSecondSlaveFeature(I2C& i2c);
  };


  /**
   * Constructor
   */

  template<uint8_t TSecondSlaveAddress,uint8_t TAddressMask>
  I2CSecondSlaveFeature<TSecondSlaveAddress,TAddressMask>::I2CSecondSlaveFeature(I2C& i2c)
    : I2CFeatureBase(i2c) {

    // configure the values

    I2C_OwnAddress2Config(_i2c,TSecondSlaveAddress,TAddressMask);
    I2C_DualAddressCmd(_i2c,ENABLE);
  }
}
