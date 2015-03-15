/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"
#include "config/stream.h"


namespace stm32plus {


  /**
   * Constructor
   * @param buffer
   * @param size
   */

  LinearBufferInputOutputStream::LinearBufferInputOutputStream(void *buffer,uint32_t size)
    : BufferedInputOutputStream(buffer,size) {
  }


  /**
   * Constructor with initial size
   * @param initialSize
   */

  LinearBufferInputOutputStream::LinearBufferInputOutputStream(uint32_t initialSize)
    : BufferedInputOutputStream(initialSize) {
  }


  /**
   * Read a single byte unless we're at the end of the stream
   * @return E_END_OF_STREAM (actually -1) or the data byte in the low 8 bits.
   */

  int16_t LinearBufferInputOutputStream::read() {

    if(_readPtr==_buffer+_bufferSize)
      return E_END_OF_STREAM;

    return *_readPtr++;
  }


  /**
   * Read a block of data from the buffer. Always succeeds but will return actuallyRead=0 at end of stream
   *
   * @param[in] buffer Where to read the data out to.
   * @param[in] size The maximum size to read.
   * @param[out] actuallyRead The actual size read.
   * @return Always true
   */

  bool LinearBufferInputOutputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    uint32_t available;

    if((available=_bufferSize-(_readPtr-_buffer))<size)
      actuallyRead=available;
    else
      actuallyRead=size;

    if(actuallyRead>0) {

      memcpy(buffer,(void *)_readPtr,actuallyRead);
      _readPtr+=actuallyRead;
    }

    return true;
  }


  /**
   * Skip forward (read pointer)
   * @param[in] howMuch How far to skip
   * @return false if would be past end of stream
   */

  bool LinearBufferInputOutputStream::skip(uint32_t howMuch) {

    uint32_t available;

    if((available=_bufferSize-(_readPtr-_buffer))<howMuch)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BUFFERED_IOSTREAM,E_INVALID_SEEK_SIZE);

    _readPtr+=howMuch;
    return true;
  }


  /**
   * Return true if at least one byte can be read without error
   * @return true if a byte can be read
   */

  bool LinearBufferInputOutputStream::available() {
    return _bufferSize-(_readPtr-_buffer)>0;
  }


  /**
   * Reset the input stream pointer.
   * @return Always true.
   */

  bool LinearBufferInputOutputStream::reset() {
    _readPtr=_buffer;
    return true;
  }


  /**
   * Reset the output pointer.
   */

  void LinearBufferInputOutputStream::resetOutput() {
    _writePtr=_buffer;
  }


  /**
   * Write a byte to the output pointer
   * @param[in] c The byte to write.
   * @return false if out of space
   */

  bool LinearBufferInputOutputStream::write(uint8_t c) {

    if(_writePtr==_buffer+_bufferSize)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BUFFERED_IOSTREAM,E_BUFFER_FULL);

    *_writePtr++=c;
    return true;
  }


  /**
   * Write a block of bytes. There must be space for it all
   * @param[in] buffer Where to write from.
   * @param[in] size The number of bytes to write
   */

  bool LinearBufferInputOutputStream::write(const void *buffer,uint32_t size) {

    // check for space

    if(_bufferSize-(_writePtr-_buffer)<size)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BUFFERED_IOSTREAM,E_BUFFER_FULL);

    // write the data

    memcpy((void *)_writePtr,buffer,size);
    _writePtr+=size;

    return true;
  }
}
