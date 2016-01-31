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

  /*
   * Methods for sending and receiving 8 and 16-bit data
   */

  struct SpiDeviceIo {

    /*
     * Senders
     */

    void sendData8(SPI_TypeDef *peripheralAddress,uint8_t data) const {
      SPI_SendData8(peripheralAddress,data);
    }

    void sendData16(SPI_TypeDef *peripheralAddress,uint16_t data) const {
      SPI_I2S_SendData16(peripheralAddress,data);
    }

    /*
     * Receivers
     */

    uint8_t receiveData8(SPI_TypeDef *peripheralAddress) const {
      return SPI_ReceiveData8(peripheralAddress);
    }

    uint16_t receiveData16(SPI_TypeDef *peripheralAddress) const {
      return SPI_I2S_ReceiveData16(peripheralAddress);
    }
  };
}
