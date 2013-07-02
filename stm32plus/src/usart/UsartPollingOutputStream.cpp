/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


namespace stm32plus {

  /**
   * Constructor for using the output stream in polling mode.
   * @param[in] usart The USART peripheral for transmitting data.
   */

  UsartPollingOutputStream::UsartPollingOutputStream(Usart& usart) :
    _usart(usart) {
  }

  /*
   * Write one byte
   */

  bool UsartPollingOutputStream::write(uint8_t c) {
    _usart.send(c);
    return true;
  }

  /*
   * Write many bytes
   */

  bool UsartPollingOutputStream::write(const void *buffer,uint32_t size) {

    const uint8_t *ptr;

    ptr=static_cast<const uint8_t *>(buffer);

    while(size--)
      _usart.send(*ptr++);

    return true;
  }
}
