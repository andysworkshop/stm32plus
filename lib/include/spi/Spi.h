/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Base class for all SPI peripherals. Supports synchronous sending and receiving of
   * data encoded into 8 or 16-bit values. The SPI peripheral on the F0 can be initialised
   * to send/receive bit streams that are no a multiple of 8.
   *
   * This class inherits from the very small device specifc IO class that handles the differences
   * in the ST peripheral library between devices.
   */

  class Spi : public SpiDeviceIo {

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
      bool receive(uint8_t *data,uint32_t numBytes);
      bool receive(uint16_t& halfWord) const;
      bool receive(uint16_t *data,uint32_t numHalfWords);

      bool readyToSend() const;
      bool send(uint8_t dataToSend) const;
      bool send(const uint8_t *dataToSend,uint32_t numBytes,uint8_t *dataReceived=nullptr) const;
      bool send(uint16_t dataToSend) const;
      bool send(const uint16_t *dataToSend,uint32_t numHalfWords,uint16_t *dataReceived=nullptr) const;

      void waitForIdle() const;
      void set1WireReadMode() const;
      void set1WireWriteMode() const;

      void setNss(bool value) const;
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

  inline void Spi::setNss(bool value) const {
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
   * @param byte[out] The byte read out
   */

  inline bool Spi::receive(uint8_t& byte) const {

    while(!readyToReceive())
      if(hasError())
        return false;

    byte=receiveData8(_peripheralAddress);
    return true;
  }


  /**
   * Read a half-word from the peripheral
   * @param hword[out] The half-word that was read out
   */

  inline bool Spi::receive(uint16_t& hword) const {

    // wait until ready to receive or there's an error

    while(!readyToReceive())
      if(hasError())
        return false;

    // read the word

    hword=receiveData16(_peripheralAddress);
    return true;
  }


  /**
   * This overload reads a number of bytes from the peripheral. It transmits dummy zero bytes to
   * cause the clock to tick and data to be received.
   * @param data The data buffer
   * @param numBytes The number of bytes to read
   * @return true if it worked
   */

  inline bool Spi::receive(uint8_t *data,uint32_t numBytes) {

    static const uint16_t zero=0;

    while(numBytes--) {

      // wait for ready to send

      while(!readyToSend())
        if(hasError())
          return false;

      // send the dummy byte, i.e. cause the SPI clock to tick

      sendData8(_peripheralAddress,zero);

      while(SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_RXNE)==RESET)
        if(hasError())
          return false;

      // read the byte to clear RXNE and save/discard

      *data++=receiveData8(_peripheralAddress);
    }

    return true;
  }


  /**
   * This overload reads a number of half-words from the peripheral. It transmits dummy zero bits to
   * cause the clock to tick and data to be received.
   * @param data The data buffer
   * @param numHalfWords The number of half-words to read
   * @return true if it worked
   */

  inline bool Spi::receive(uint16_t *data,uint32_t numHalfWords) {

    static const uint16_t zero=0;

    while(numHalfWords--) {

      // wait for ready to send

      while(!readyToSend())
        if(hasError())
          return false;

      // send the dummy byte, i.e. cause the SPI clock to tick

      sendData16(_peripheralAddress,zero);

      while(SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_RXNE)==RESET)
        if(hasError())
          return false;

      // read the byte to clear RXNE and save/discard

      *data++=receiveData16(_peripheralAddress);
    }

    return true;
  }


  /**
   * Check for TXE
   * @return true if ready to send
   */

  inline bool Spi::readyToSend() const {
    return !!SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_TXE);
  }


  /**
   * Send a single byte.
   * @param dataToSend The byte to send
   * @return true if it worked, false on error
   */

  inline bool Spi::send(uint8_t dataToSend) const {

    // wait for ready to send

    while(!readyToSend())
      if(hasError())
        return false;

    // send the byte

    sendData8(_peripheralAddress,dataToSend);
    return true;
  }


  /**
   * Send a block of bytes, blocking. Optionally receive data at the same time
   * @param dataToSend The buffer of bytes to send
   * @param numBytes The number of bytes to send
   * @param[out] dataReceived Where to store the data received. Set to nullptr if you're not receiving any data.
   */

  inline bool Spi::send(const uint8_t *dataToSend,uint32_t numBytes,uint8_t *dataReceived) const {

    while(numBytes--) {

      send(*dataToSend++);

      if(_direction==SPI_Direction_2Lines_FullDuplex) {

        // in duplex mode and we want data, wait for it to come

        while(SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_RXNE)==RESET)
          if(hasError())
            return false;

        // read the byte to clear RXNE and save

        if(dataReceived!=nullptr)
          *dataReceived++=receiveData8(_peripheralAddress);
        else
          receiveData8(_peripheralAddress);
      }
    }

    return true;
  }


  /**
   * Send a single half-word.
   * @param dataToSend The hald-word to send
   * @return true if it worked, false on error
   */

  inline bool Spi::send(uint16_t dataToSend) const {
      // wait for ready to send

      while(!readyToSend())
        if(hasError())
          return false;

      // send the half-word

      sendData16(_peripheralAddress,dataToSend);
      return true;
  }


  /**
   * Send a block of half-words, blocking. Optionally receive data at the same time
   * @param dataToSend The buffer of half-words to send
   * @param numHalfWords The number of half-words to send
   * @param[out] dataReceived Where to store the data received. Set to nullptr if you're not receiving any data.
   */

  inline bool Spi::send(const uint16_t *dataToSend,uint32_t numHalfWords,uint16_t *dataReceived) const {

    while(numHalfWords--) {

      send(*dataToSend++);

      if(_direction==SPI_Direction_2Lines_FullDuplex) {

        // in duplex mode and we want data, wait for it to come

        while(SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_RXNE)==RESET)
          if(hasError())
            return false;

        // read the byte to clear RXNE and save it

        if(dataReceived!=nullptr)
          *dataReceived++=receiveData16(_peripheralAddress);
        else
          receiveData16(_peripheralAddress);
      }
    }

    return true;
  }


  /**
   * Wait for the peripheral to become idle. If transmitting then the peripheral becomes
   * idle when the last word written to the TX register has been shifted out to the bus.
   */

  inline void Spi::waitForIdle() const {
    while(SPI_I2S_GetFlagStatus(_peripheralAddress,SPI_I2S_FLAG_BSY)==SET);
  }


  /**
   * Set read mode when in bi-directional 1-wire mode
   */

  inline void Spi::set1WireReadMode() const {
    SPI_BiDirectionalLineConfig(_peripheralAddress,SPI_Direction_Rx);
  }


  /**
   * Set write mode when in bi-directional 1-wire mode
   */

  inline void Spi::set1WireWriteMode() const {
    SPI_BiDirectionalLineConfig(_peripheralAddress,SPI_Direction_Tx);
  }
}
