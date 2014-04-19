/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for all I2S peripherals
   */

  class I2S : public SpiDeviceIo {

    protected:
      SPI_TypeDef *_peripheralAddress;

    public:
      enum {
        E_I2S_ERROR = 1
      };

    protected:
      I2S(SPI_TypeDef *address);

    public:
      bool readyToSend() const;
      bool send(const uint16_t *dataToSend,uint32_t numHalfWords) const;

      bool hasError() const;

      operator SPI_TypeDef *() const;

      void enablePeripheral() const;
      void disablePeripheral() const;
  };


  /**
   * Constructor
   * @param[in] address The peripheral address, e.g. USART1
   * @param[in] nssPort The chip select port
   * @param[in] nssPin The chip select pin - we control it manually
   */

  inline I2S::I2S(SPI_TypeDef *address) {
    _peripheralAddress=address;
  }


  /**
   * Enable the peripheral
   */

  inline void I2S::enablePeripheral() const {
    I2S_Cmd(_peripheralAddress,ENABLE);
  }


  /**
   * Disable the peripheral
   */

  inline void I2S::disablePeripheral() const {
    I2S_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Cast this class to the SPI peripheral address.
   * @return The SPI peripheral address.
   */

  inline I2S::operator SPI_TypeDef *() const {
    return _peripheralAddress;
  }


  /**
   * Check if the device has an error status
   */

  inline bool I2S::hasError() const {

    FlagStatus status;

    if((status=SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_FLAG_CRCERR | SPI_FLAG_MODF | SPI_I2S_FLAG_OVR))!=0)
      return !errorProvider.set(ErrorProvider::ERROR_PROVIDER_I2S,E_I2S_ERROR,status);

    return false;
  }


  /**
   * Check for TXE
   */

  inline bool I2S::readyToSend() const {
    return !!SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_TXE);
  }


  /**
   * Send a block of half words, blocking.
   */

  inline bool I2S::send(const uint16_t *dataToSend,uint32_t numHalfWords) const {

    // wait for ready to send

    while(numHalfWords--) {

      while(!readyToSend())
        if(hasError())
          return false;

      // send the 16-bit halfword

      sendData16(_peripheralAddress,*dataToSend++);
    }

    return true;
  }
}

