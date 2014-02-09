/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/stream.h"


namespace stm32plus {

  /**
   * Write a signed byte to the stream.
   * @param[in] c The byte to write.
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(int8_t c) {
    return opWrite(&c,sizeof(c));
  }

  /**
   * Write a signed 16-bit integer to the stream.
   * @param[in] c The 16-bit integer to write.
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(int16_t c) {
    return opWrite(&c,sizeof(c));
  }

  /**
   * Write a signed 32-bit integer to the stream.
   * @param[in] c The 32-bit integer to write.
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(int32_t c) {
    return opWrite(&c,sizeof(c));
  }

  /**
   * Write an unsigned byte to the stream.
   * @param[in] c The byte to write.
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(uint8_t c) {
    return opWrite(&c,sizeof(c));
  }

  /**
   * Write an unsigned 16-bit integer to the stream.
   * @param[in] c The 16-bit integer to write.
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(uint16_t c) {
    return opWrite(&c,sizeof(c));
  }

  /**
   * Write an unsigned 32-bit integer to the stream.
   * @param[in] c The 32-bit integer to write.
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(uint32_t c) {
    return opWrite(&c,sizeof(c));
  }

  /*
   * Write some data for one of the operators
   */

  OutputStream& OutputStream::opWrite(void *buffer,uint32_t bufferSize) {
    write(buffer,bufferSize);
    return *this;
  }


  /**
   * Write a 'C' style nul-terminated string
   * @param string The string
   * @return A self reference to allow chaining of the operator.
   */

  OutputStream& OutputStream::operator<<(const char *string) {
    write(string,strlen(string));
    return *this;
  }
}
