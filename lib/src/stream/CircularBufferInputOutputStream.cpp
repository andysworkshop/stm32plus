/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/stream.h"


namespace stm32plus {

  /**
   * Read a single byte, returned in the low 8 bits of the return value.
   * @return E_END_OF_STREAM (actually -1) or the data byte in the low 8 bits.
   */

  int16_t CircularBufferInputOutputStream::read() {

    uint8_t value;

    if(_readPtr == _writePtr)
      return E_END_OF_STREAM; // EOF

    value=*_readPtr++;

    if(_readPtr >= _buffer + _bufferSize)
      _readPtr=_buffer;

    return value;
  }

  /**
   * Read multiple.
   * @param[in,out] buffer Where to place the data.
   * @param[in] size The amount to try and read.
   * @param[out] actuallyRead The number of bytes read without error.
   * @return false if it failed, or the end of stream is reached. In the case of the end of stream actuallyRead will
   *    hold the number of bytes that were read before the end of stream was hit.
   */

  bool CircularBufferInputOutputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    actuallyRead=0;
    int16_t value;
    uint8_t *ptr;

    ptr=static_cast<uint8_t *> (buffer);

    // read until EOF or got all

    while(size-- && (value=read()) != E_END_OF_STREAM) {
      *ptr++=value;
      actuallyRead++;
    }

    return true;
  }

  /*
   * Skip forward
   */

  bool CircularBufferInputOutputStream::skip(uint32_t howMuch) {

    uint32_t available;

    // get the amount available to read

    if(_writePtr >= _readPtr)
      available=_writePtr - _readPtr;
    else
      available=_bufferSize - (_readPtr - _writePtr);

    // check range

    if(howMuch > available)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BUFFERED_IOSTREAM,E_INVALID_SEEK_SIZE);

    _readPtr+=howMuch;

    if(_readPtr >= _buffer + _bufferSize)
      _readPtr=_buffer + (_readPtr - _buffer + _bufferSize);

    return true;
  }

  /**
   * Check if can read without error.
   * @return true if there is data to read.
   */

  bool CircularBufferInputOutputStream::available() {
    return _readPtr != _writePtr;
  }

  /**
   * Write a byte.
   * @param[in] c The byte to write.
   * @return false and E_BUFFER_FULL if the buffer has no space available for writing.
   */

  bool CircularBufferInputOutputStream::write(uint8_t c) {

    if(_readPtr == _writePtr && _wrappedWrite)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BUFFERED_IOSTREAM,E_BUFFER_FULL);

    *_writePtr++=c;

    if(_writePtr == _buffer + _bufferSize) {
      _writePtr=_buffer;
      _wrappedWrite=true;
    }

    return true;
  }

  /**
   * Write many bytes.
   * @param[in] buffer The buffer of data to write.
   * @param[in] size The amount of bytes in the buffer.
   * @return false and E_BUFFER_FULL if the buffer has no space available for writing. In this case no data is written.
   */

  bool CircularBufferInputOutputStream::write(const void *buffer,uint32_t size) {

    uint32_t available;
    const uint8_t *ptr;

    // get the amount available to write

    if(_readPtr == _writePtr)
      available=_wrappedWrite ? 0 : _bufferSize;
    else if(_readPtr > _writePtr)
      available=_readPtr - _writePtr;
    else
      available=_bufferSize - (_writePtr - _readPtr);

    if(size > available)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BUFFERED_IOSTREAM,E_BUFFER_FULL);

    // do the write

    ptr=static_cast<const uint8_t *> (buffer);
    while(size--)
      write(*ptr++);

    return true;
  }
}
