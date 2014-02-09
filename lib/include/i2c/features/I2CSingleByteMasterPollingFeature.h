/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Subclass of the I2CMasterPollingFeature that specialises it for the case
   * where both the data and the register address are just a single byte each.
   * This caters for a lot of the simple I2C devices. Convenience functions are
   * provided to set and clear single bits in the register.
   */

  class I2CSingleByteMasterPollingFeature : public I2CMasterPollingFeature {

    public:
      I2CSingleByteMasterPollingFeature(I2C& i2c);

      bool writeByte(uint8_t address,uint8_t input) const;
      bool readByte(uint8_t address,uint8_t& output) const;

      bool prepareRead(uint8_t address) const;
      bool readBytes(uint8_t address,uint8_t *output,uint32_t count) const;

      bool prepareWrite(uint8_t address) const;
      bool writeBytes(uint8_t address,const uint8_t *input,uint32_t count) const;

      bool resetBits(uint8_t address,uint8_t mask) const;
      bool setBits(uint8_t address,uint8_t mask) const;

      bool setMask(uint8_t address,uint8_t newbits,uint8_t mask) const;
  };


  /**
   * Constructor
   * @param i2c
   */

  inline I2CSingleByteMasterPollingFeature::I2CSingleByteMasterPollingFeature(I2C& i2c)
    : I2CMasterPollingFeature(i2c) {

    // default timeout is 5 seconds

    _timeout=5000;
  }


  /**
   * Prepare for reading
   * @param address The register index
   * @return true if it worked
   */

  inline bool I2CSingleByteMasterPollingFeature::prepareRead(uint8_t address) const {
    return I2CMasterPollingFeature::prepareRead(&address);
  }


  /**
   * Read multiple bytes
   * @param address The register index
   * @param output Where to store them
   * @param count how many to read
   * @return true if it worked
   */

  inline bool I2CSingleByteMasterPollingFeature::readBytes(uint8_t address,uint8_t *output,uint32_t count) const {
    return I2CMasterPollingFeature::readBytes(&address,output,count);
  }


  /**
   * Prepare for writing
   * @param address The register index
   * @return true if it worked
   */

  inline bool I2CSingleByteMasterPollingFeature::prepareWrite(uint8_t address) const {
    return I2CMasterPollingFeature::prepareWrite(&address);
  }


  /**
   * Write multiple bytes
   * @param address The register index
   * @param input The bytes to write
   * @param count how many to write
   * @return true if it worked
   */

  inline bool I2CSingleByteMasterPollingFeature::writeBytes(uint8_t address,const uint8_t *input,uint32_t count) const {
    return I2CMasterPollingFeature::writeBytes(&address,input,count);
  }


  /**
   * Read a byte from the slave
   * @param address The register number
   * @param output Where to put the byte
   * @return false if there's an error, true otherwise
   */

  inline bool I2CSingleByteMasterPollingFeature::readByte(uint8_t address,uint8_t& output) const {
    return readBytes(address,&output,1);
  }


  /**
   * Write a single byte
   * @param address The register address
   * @param input The byte to write
   * @return true if it works, false if not
   */

  inline bool I2CSingleByteMasterPollingFeature::writeByte(uint8_t address,uint8_t input) const {
    return writeBytes(address,&input,1);
  }
}
