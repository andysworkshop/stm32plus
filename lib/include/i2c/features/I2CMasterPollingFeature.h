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

  class I2CMasterPollingFeature : public I2CIoFeature {

    protected:
      uint8_t _slaveAddress;

    protected:
      bool checkEvent(uint32_t eventId) const;

    public:
      I2CMasterPollingFeature(I2C& i2c);

      bool prepareRead(const uint8_t *address) const;
      bool readBytes(const uint8_t *address,uint8_t *output,uint32_t count) const;

      bool prepareWrite(const uint8_t *address) const;
      bool writeBytes(const uint8_t *address,const uint8_t *input,uint32_t count) const;

      void setSlaveAddress(uint8_t address);
  };


  /**
   * Constructor
   * @param i2c The i2c feature base reference
   */

  inline I2CMasterPollingFeature::I2CMasterPollingFeature(I2C& i2c)
    : I2CIoFeature(i2c) {
  }


  /**
   * Set the slave peripheral address
   * @param address The 7-bit slave address is left aligned within the byte. i.e. the MSB
   * of the slave address is bit 7 (the 8th bit) in this parameter.
   */

  inline void I2CMasterPollingFeature::setSlaveAddress(uint8_t address) {
    _slaveAddress=address;
  }
}
