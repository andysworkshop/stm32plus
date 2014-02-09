/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Subclass of the I2CMasterPollingFeature that specialises it for the case
   * where both the address (which in this case is probably an EEPROM index) is
   * two bytes wide. The MSB of the address is the first to be transmitted,
   * the LSB the second.
   */

  class I2CTwoByteMasterPollingFeature : public I2CMasterPollingFeature {

    public:
      I2CTwoByteMasterPollingFeature(I2C& i2c);

      bool writeByte(uint16_t address,uint8_t input) const;
      bool readByte(uint16_t address,uint8_t& output) const;

      bool prepareRead(uint16_t address) const;
      bool readBytes(uint16_t address,uint8_t *output,uint32_t count) const;

      bool prepareWrite(uint16_t address) const;
      bool writeBytes(uint16_t address,const uint8_t *input,uint32_t count) const;
  };


  /**
   * Constructor
   * @param i2c The base peripheral class
   */

  inline I2CTwoByteMasterPollingFeature::I2CTwoByteMasterPollingFeature(I2C& i2c)
    : I2CMasterPollingFeature(i2c) {

    // force the address width to two bytes

    i2c.setAddressSize(2);

    // default timeout is 5 seconds

    _timeout=5000;
  }


  /**
   * Prepare for reading
   * @param address The register index
   * @return true if it worked
   */

  inline bool I2CTwoByteMasterPollingFeature::prepareRead(uint16_t address) const {

    uint8_t bytes[2];

    bytes[0]=address>>8;
    bytes[1]=address;

    return I2CMasterPollingFeature::prepareRead(bytes);
  }


  /**
   * Read multiple bytes
   * @param address The register index
   * @param output Where to store them
   * @param count how many to read
   * @return true if it worked
   */

  inline bool I2CTwoByteMasterPollingFeature::readBytes(uint16_t address,uint8_t *output,uint32_t count) const {

    uint8_t bytes[2];

    bytes[0]=address>>8;
    bytes[1]=address;

    return I2CMasterPollingFeature::readBytes(bytes,output,count);
  }


  /**
   * Prepare for writing
   * @param address The register index
   * @return true if it worked
   */

  inline bool I2CTwoByteMasterPollingFeature::prepareWrite(uint16_t address) const {

    uint8_t bytes[2];

    bytes[0]=address>>8;
    bytes[1]=address;

    return I2CMasterPollingFeature::prepareWrite(bytes);
  }


  /**
   * Write multiple bytes
   * @param address The register index
   * @param input The bytes to write
   * @param count how many to write
   * @return true if it worked
   */

  inline bool I2CTwoByteMasterPollingFeature::writeBytes(uint16_t address,const uint8_t *input,uint32_t count) const {

    uint8_t bytes[2];

    bytes[0]=address>>8;
    bytes[1]=address;

    return I2CMasterPollingFeature::writeBytes(bytes,input,count);
  }


  /**
   * Read a byte from the slave
   * @param address The register number
   * @param output Where to put the byte
   * @return false if there's an error, true otherwise
   */

  inline bool I2CTwoByteMasterPollingFeature::readByte(uint16_t address,uint8_t& output) const {

    uint8_t bytes[2];

    bytes[0]=address>>8;
    bytes[1]=address;

    return I2CMasterPollingFeature::readBytes(bytes,&output,1);
  }


  /**
   * Write a single byte
   * @param address The register address
   * @param input The byte to write
   * @return true if it works, false if not
   */

  inline bool I2CTwoByteMasterPollingFeature::writeByte(uint16_t address,uint8_t input) const {

    uint8_t bytes[2];

    bytes[0]=address>>8;
    bytes[1]=address;

    return I2CMasterPollingFeature::writeBytes(bytes,&input,1);
  }
}
