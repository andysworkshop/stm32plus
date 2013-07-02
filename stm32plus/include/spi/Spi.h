/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for all SPI peripherals
   */

  class Spi {

    protected:
      SPI_TypeDef *_peripheralAddress;
      GPIO_TypeDef *_nssPort;
      int _nssPin;
      uint16_t _direction;

    public:
      enum {
        E_SPI_ERROR = 1
      };

    protected:
      Spi(SPI_TypeDef *address,
          GPIO_TypeDef* nssPort,
          int nssPin,
          uint16_t direction);

    public:
      bool readyToReceive() const;
      bool receive(uint8_t& byte) const;

      bool readyToSend() const;
      bool send(const uint8_t *dataToSend,uint32_t numBytes,uint8_t *dataReceived=nullptr) const;

      void setNss(bool value);
      operator SPI_TypeDef *() const;
      bool hasError() const;

      void enablePeripheral() const;
      void disablePeripheral() const;
  };


  /**
   * Constructor
   * @param[in] address The peripheral address, e.g. SPI1
   * @param[in] nssPort The chip select port
   * @param[in] nssPin The chip select pin - we control it manually
   */

  inline Spi::Spi(
      SPI_TypeDef *address,
      GPIO_TypeDef * nssPort,
      int nssPin,
      uint16_t direction)
        : _nssPort(nssPort) {

    _peripheralAddress=address;
    _nssPin=nssPin;
    _direction=direction;
  }


  /**
   * Enable the peripheral
   */

  inline void Spi::enablePeripheral() const {
    SPI_Cmd(_peripheralAddress,ENABLE);
  }


  /**
   * Disable the peripheral
   */

  inline void Spi::disablePeripheral() const {
    SPI_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Cast this class to the SPI peripheral address.
   * @return The SPI peripheral address.
   */

  inline Spi::operator SPI_TypeDef *() const {
    return _peripheralAddress;
  }


  /**
   * Check if the device has an error status
   */

  inline bool Spi::hasError() const {

    FlagStatus status;

    if((status=SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_FLAG_CRCERR | SPI_FLAG_MODF | SPI_I2S_FLAG_OVR))!=0)
      return !errorProvider.set(ErrorProvider::ERROR_PROVIDER_SPI,E_SPI_ERROR,status);

    return false;
  }


  /**
   * Set or reset the NSS (chip select) pin
   * @param value true = high, false = low
   */

  inline void Spi::setNss(bool value) {
    if(value)
      GPIO_SetBits(_nssPort,_nssPin);
    else
      GPIO_ResetBits(_nssPort,_nssPin);
  }


  /**
   * Check if the peripheral is ready to receive
   * @return true if it's ready
   */

  inline bool Spi::readyToReceive() const {
    return !!SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_RXNE);
  }


  /**
   * Read a byte from the peripheral
   */

  inline bool Spi::receive(uint8_t& byte) const {

    while(!readyToReceive())
      if(hasError())
        return false;

    byte=SPI_I2S_ReceiveData(_peripheralAddress);
    return true;
  }

  /**
   * Check for TXE
   */

  inline bool Spi::readyToSend() const {
    return !!SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_TXE);
  }


  /**
   * Send a block of bytes, blocking.
   */

  inline bool Spi::send(const uint8_t *dataToSend,uint32_t numBytes,uint8_t *dataReceived) const {

    // wait for ready to send

    while(numBytes--) {

      while(!readyToSend())
        if(hasError())
          return false;

      // send the byte

      SPI_I2S_SendData(_peripheralAddress,*dataToSend++);

      if(_direction==SPI_Direction_2Lines_FullDuplex) {

        // in duplex mode and we want data, wait for it to come

        while(SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_RXNE)==RESET)
          if(hasError())
            return false;

        // read the byte to clear RXNE and save/discard

        if(dataReceived!=nullptr)
          *dataReceived++=SPI_I2S_ReceiveData(_peripheralAddress);
        else
          SPI_I2S_ReceiveData(_peripheralAddress);
      }
    }

    return true;
  }
}
