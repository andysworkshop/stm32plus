/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/stream.h"


namespace stm32plus {

  /**
   * Constructor.
   * @param[in] initialSize The size to allocate.
   * @param[in] resizeAmount The amount to increase allocation by when new space needed
   */

  ByteArrayOutputStream::ByteArrayOutputStream(uint32_t initialSize,uint32_t resizeAmount)
    : _memblock(initialSize) {

    _currentUsage=0;
    _initialSize=initialSize;
    _resizeAmount=resizeAmount;
  }


  /**
   * Clear the stream back to zero size
   */

  void ByteArrayOutputStream::clear() {
    _memblock.reset(_initialSize);
    _currentUsage=0;
  }

  /**
   * get the current size of the data written to the stream.
   * @return The current byte size actually used
   */

  uint32_t ByteArrayOutputStream::getSize() const {
    return _currentUsage;
  }


  /**
   * get a pointer to the internal buffer containing the data written to the stream.
   * This buffer is owned by the stream and should not be free'd by you. Modifications
   * to the data in this buffer directly affect the data in the stream.
   * @return A pointer to the internal buffer
   */

  uint8_t* ByteArrayOutputStream::getBuffer() const {
    return _memblock.getData();
  }


  /**
   * write a byte to the buffer
   */

  bool ByteArrayOutputStream::write(uint8_t c) {
    return write(&c,1);
  }


  /*
   * Write a buffer to the stream
   */

  bool ByteArrayOutputStream::write(const void *buffer,uint32_t size) {

    uint32_t newSize;

    if(_currentUsage+size>_memblock.getSize()) {

      // more space needed: reallocate to the required amount plus margin

      newSize=_currentUsage+size+_resizeAmount;
      _memblock.reallocate(newSize);
    }

    memcpy(_memblock.getData()+_currentUsage,buffer,size);
    _currentUsage+=size;

    return true;
  }


  /*
   * Can't close
   */

  bool ByteArrayOutputStream::close() {
    return true;
  }


  /*
   * Can't flush
   */

  bool ByteArrayOutputStream::flush() {
    return true;
  }
}

