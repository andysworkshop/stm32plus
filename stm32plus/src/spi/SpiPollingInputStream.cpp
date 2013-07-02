/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"


namespace stm32plus {

  /*
   * Constructor
   */

  SpiPollingInputStream::SpiPollingInputStream(Spi& spi) :
    _spi(spi) {
  }


  /*
   * Read a byte - can block
   */

  int16_t SpiPollingInputStream::read() {

    uint8_t data;

    while(!_spi.readyToReceive());

    if(!_spi.receive(data))
      return E_STREAM_ERROR;

    return data;
  }

  /*
   * Read many bytes
   */

  bool SpiPollingInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    uint8_t *ptr;
    int16_t data;

    ptr=static_cast<uint8_t *> (buffer);
    actuallyRead=size;

    while(size--) {

      if((data=read())==E_STREAM_ERROR)
        return false;

      *ptr++=data;
    }

    return true;
  }


  /*
   * Cannot skip
   */

  bool SpiPollingInputStream::skip(uint32_t howMuch __attribute__((unused))) {
    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
  }


  /*
   * Check if data is available - avoids blocking
   */

  bool SpiPollingInputStream::available() {
    return _spi.readyToReceive();
  }
}
