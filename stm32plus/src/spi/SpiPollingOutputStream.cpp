/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"


namespace stm32plus {

  /**
   * Constructor for using the output stream in polling mode.
   * @param[in] spi The SPI peripheral for transmitting data.
   */

  SpiPollingOutputStream::SpiPollingOutputStream(Spi& spi) :
    _spi(spi) {
  }

  /*
   * Write one byte
   */

  bool SpiPollingOutputStream::write(uint8_t c) {

    while(!_spi.readyToSend());
    return _spi.send(&c,1);
  }

  /*
   * Write many bytes
   */

  bool SpiPollingOutputStream::write(const void *buffer,uint32_t size) {

    while(!_spi.readyToSend());
    return _spi.send(static_cast<const uint8_t *>(buffer),size);
  }
}
