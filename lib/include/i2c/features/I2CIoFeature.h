/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base feature class to enable polling communication in master mode. This class manages
   * the actual I2C protocol interaction. It makes the assumption that the exchange with a
   * peripheral consists of sending N bytes as an "address" and then either reading or
   * writing some data. The address size (N) can be 1..255. That size can be found in
   * the I2C Parameters class.
   */

  class I2CIoFeature : public I2CFeatureBase {

    protected:
      uint32_t _timeout;

    public:
      I2CIoFeature(I2C& i2c);

      void setTimeout(uint32_t timeout);
  };


  /**
   * Constructor
   * @param i2c The i2c feature base reference
   */

  inline I2CIoFeature::I2CIoFeature(I2C& i2c)
    : I2CFeatureBase(i2c) {
    // default timeout is 5 seconds

    _timeout=5000;
  }


  /**
   * Set the timeout, after which an error will be raised
   * @param timeout The number of milliseconds to wait for a reply on the bus before considering it an error
   */

  inline void I2CIoFeature::setTimeout(uint32_t timeout) {
    _timeout=timeout;
  }
}
