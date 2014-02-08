/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/stream.h"


namespace stm32plus {

  /**
   * Read a single byte using the stream input operator syntax.
   * @param[out] c_ Reference to the byte to hold the result.
   * @return A self reference to allow chaining of the operator.
   */

  InputStream& InputStream::operator>>(int8_t& c_) {
    return opRead(&c_,sizeof(c_));
  }

  /**
   * Read a 16-bit integer using the stream input operator syntax.
   * @param[out] c_ Reference to the integer to hold the result.
   * @return A self reference to allow chaining of the operator.
   */

  InputStream& InputStream::operator>>(int16_t& c_) {
    return opRead(&c_,sizeof(c_));
  }

  /**
   * Read a 32-bit integer using the stream input operator syntax.
   * @param[out] c_ Reference to the integer to hold the result.
   * @return A self reference to allow chaining of the operator.
   */

  InputStream& InputStream::operator>>(int32_t& c_) {
    return opRead(&c_,sizeof(c_));
  }

  /**
   * Read a single unsigned byte using the stream input operator syntax.
   * @param[out] c_ Reference to the byte to hold the result.
   * @return A self reference to allow chaining of the operator.
   */

  InputStream& InputStream::operator>>(uint8_t& c_) {
    return opRead(&c_,sizeof(c_));
  }

  /**
   * Read a single unsigned 16-bit integer using the stream input operator syntax.
   * @param[out] c_ Reference to the integer to hold the result.
   * @return A self reference to allow chaining of the operator.
   */

  InputStream& InputStream::operator>>(uint16_t& c_) {
    return opRead(&c_,sizeof(c_));
  }

  /**
   * Read a single unsigned 32-bit integer using the stream input operator syntax.
   * @param[out] c_ Reference to the integer to hold the result.
   * @return A self reference to allow chaining of the operator.
   */

  InputStream& InputStream::operator>>(uint32_t& c_) {
    return opRead(&c_,sizeof(c_));
  }


  /*
   * Read some data for one of the operators
   */

  InputStream& InputStream::opRead(void *buffer_,uint32_t bufferSize_) {
    uint32_t actuallyRead;

    read(buffer_,bufferSize_,actuallyRead);
    return *this;
  }
}
