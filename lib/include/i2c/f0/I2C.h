/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Base class for I2C peripherals. Takes care of the common operations
   * that each peripheral can have
   */

  class I2C {

    public:

      /**
       * I2C parameters class
       */

      struct Parameters {
        uint32_t i2c_timing;
        bool     i2c_analogFilter;
        uint8_t  i2c_digitalFilter;       // number of i2c clocks to ignore spikes
        bool     i2c_ack;
        uint16_t i2c_ackAddress;
        uint16_t i2c_ownAddress;
        uint8_t  i2c_addressSize;        // byte-size of the 'address' or 'register' index on the peripheral

        Parameters(uint32_t timing=0x00201D2B) {
          i2c_timing=timing;
          i2c_analogFilter=true;
          i2c_digitalFilter=0;
          i2c_ack=true;
          i2c_ackAddress=I2C_AcknowledgedAddress_7bit;
          i2c_ownAddress=0xFE;
          i2c_addressSize=1;
        }
      };

      enum {
        E_I2C_TIMEOUT=1         ///< timed out waiting for a response
      };

    protected:
      I2C_TypeDef *_peripheralAddress;
      uint8_t _addressSize;

    protected:
      I2C(const Parameters& params,I2C_TypeDef *peripheralAddress);

    public:
      void enablePeripheral() const;
      void disablePeripheral() const;

      void reset() const;

      uint8_t getAddressSize() const;
      void setAddressSize(uint8_t addressSize);

      operator I2C_TypeDef *();
  };


  /**
   * Constructor
   */

  inline I2C::I2C(const Parameters& params,I2C_TypeDef *peripheralAddress)
    : _peripheralAddress(peripheralAddress),
      _addressSize(params.i2c_addressSize) {
  }


  /**
   * Enable the peripheral
   */

  inline void I2C::enablePeripheral() const {
    I2C_Cmd(_peripheralAddress,ENABLE);
  }


  /**
   * Disable the peripheral
   */

  inline void I2C::disablePeripheral() const {
    I2C_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Do a software reset. The peripheral library actually does a peripheral enable/disable on the F0.
   */

  inline void I2C::reset() const {
    I2C_SoftwareResetCmd(_peripheralAddress);
  }


  /**
   * Cast to a I2C peripheral structure
   */

  inline I2C::operator I2C_TypeDef *() {
    return _peripheralAddress;
  }


  /**
   * Get the address or register size in bytes. Often this is just one byte where the IC has
   * less than 255 registers but in EEPROMs etc. could be multiple bytes to fit in an 'address'
   * to read or write from/to.
   * @return The byte width
   */

  inline uint8_t I2C::getAddressSize() const {
    return _addressSize;
  }


  /**
   * Set the address size
   * @param addressSize The new address size
   */

  inline void I2C::setAddressSize(uint8_t addressSize) {
    _addressSize=addressSize;
  }
}
