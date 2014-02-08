/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * Base class for Usart peripherals. Takes care of the common operations
   * that each peripheral can have
   */

  class Usart {

    public:

      enum {
        /// There was an error using the USART
        E_USART_ERROR=1
      };

    protected:
      USART_TypeDef *_peripheralAddress;

    protected:
      Usart(USART_TypeDef *peripheralAddress);

    public:
      bool hasError(uint32_t& errorCode) const;

      uint8_t receive() const;
      bool dataAvailable() const;

      bool send(uint8_t byte) const;
      bool readyToSend() const;

      void enablePeripheral() const;
      void disablePeripheral() const;

      operator USART_TypeDef *();
  };


  /**
   * Constructor
   */

  inline Usart::Usart(USART_TypeDef *peripheralAddress)
    : _peripheralAddress(peripheralAddress) {
  }


  /**
   * check for an error state
   * @param[out] errorCode The peripheral error code (USART_FLAG_ORE, USART_FLAG_NE, USART_FLAG_FE,USART_FLAG_PE).
   * @return true if there was an error, false if no error.
   */

  inline bool Usart::hasError(uint32_t& errorCode) const {
    errorCode=USART_GetFlagStatus(_peripheralAddress,USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);

    if(errorCode) {
      USART_ReceiveData(_peripheralAddress);    // required to clear error state
      return !errorProvider.set(ErrorProvider::ERROR_PROVIDER_USART,E_USART_ERROR,errorCode);
    }

    // no error

    return false;
  }


  /**
   * Enable the peripheral
   */

  inline void Usart::enablePeripheral() const {
    USART_Cmd(_peripheralAddress,ENABLE);
  }


  /**
   * Disable the peripheral
   */

  inline void Usart::disablePeripheral() const {
    USART_Cmd(_peripheralAddress,DISABLE);
  }


  /**
   * Cast to a USART peripheral structure
   */

  inline Usart::operator USART_TypeDef *() {
    return _peripheralAddress;
  }


  /**
   * Receive a byte, blocking.
   * @return The byte, or 0xff if it failed. Of course the data byte itself may be 0xff. The caller should check
   *  the error provider to see if the 0xff is indicating an error or a data byte.
   */

  inline uint8_t Usart::receive() const {

    uint32_t errorCode;

    do {
      if(hasError(errorCode))
        return 0xff; // caller to check global error status

    } while(!dataAvailable());

    return USART_ReceiveData(_peripheralAddress) & 0xff;
  }


  /**
   * Send a byte and wait until it's gone
   * @param byte The byte to send.
   * @return false if it failed.
   */

  inline bool Usart::send(uint8_t byte) const {

    uint32_t errorCode;

    do {
      if(hasError(errorCode))
        return false;

    } while(!readyToSend());

    USART_SendData(_peripheralAddress,byte);
    return true;
  }


  /**
   * Check if ready to send.
   * @return true if the peripheral is ready to send data.
   */

  inline bool Usart::readyToSend() const {
    return USART_GetFlagStatus(_peripheralAddress,USART_FLAG_TXE)==SET;
  }


  /**
   * Check if there is data available.
   * @return true if there is data available.
   */

  inline bool Usart::dataAvailable() const {
    return USART_GetFlagStatus(_peripheralAddress,USART_FLAG_RXNE)==SET;
  }
}
