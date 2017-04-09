/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Template class to manage a circular buffer of types. Separate read and write pointers are
   * maintained. Methods are provided to read from and write to the buffer. More methods are provided
   * to find out how much read space and write space is available without overrunning the other
   * position.
   *
   * The 'last operation type' flag is stored in the high bit of the read index. This allows the
   * availableToWrite() and availableToRead() methods to get at the read index and the flag in an
   * atomic operation. This makes this class safe to use for an IRQ-based writer and a normal code
   * reader - but not the other way around.
   */

  template<typename T>
  class circular_buffer {

    protected:

      enum {
        LAST_OP_READ_FLAG = 0x80000000,
        READ_INDEX_MASK = ~LAST_OP_READ_FLAG
      };

      T *_buffer;
      uint32_t _size;
      uint32_t _readIndex;
      uint32_t _writeIndex;

    public:
      circular_buffer(uint32_t size);
      ~circular_buffer();

      void read(T *output,uint32_t size) volatile;
      T read() volatile;

      void write(const T *input,uint32_t size) volatile;
      void write(const T& input) volatile;

      uint32_t availableToWrite() const volatile;
      uint32_t availableToRead() const volatile;
  };


  /**
   * Constructor
   * @param size The number of types to store in the buffer
   */

  template<typename T>
  inline circular_buffer<T>::circular_buffer(uint32_t size)
    : _buffer(new T[size]),
      _size(size),
      _readIndex(LAST_OP_READ_FLAG),
      _writeIndex(0) {
  }


  /**
   * Destructor
   */

  template<typename T>
  inline circular_buffer<T>::~circular_buffer() {
    delete[] _buffer;
  }


  /**
   * get the number of types that available to write without overrunning the read pointer. In
   * an IRQ-writer, normal reader scenario this method is safe for the IRQ writer to call.
   * @return The number of available types
   */

  template<typename T>
  inline uint32_t circular_buffer<T>::availableToWrite() const volatile {

    volatile uint32_t readIndex;
    bool lastOpRead;

    readIndex=_readIndex;
    lastOpRead=(readIndex & LAST_OP_READ_FLAG)!=0;
    readIndex&=READ_INDEX_MASK;

    if(_writeIndex==readIndex)
      return lastOpRead ? _size : 0;
    else if(_writeIndex>readIndex)
      return _size-_writeIndex+readIndex;
    else
      return readIndex-_writeIndex;
  }


  /**
   * get the number of types that available to read without overrunning the write pointer
   * @return The number of available types
   */

  template<typename T>
  inline uint32_t circular_buffer<T>::availableToRead() const volatile {

    volatile uint32_t readIndex;
    uint32_t ret;
    bool lastOpRead;

    // the loop catches a nasty case where an interrupt gets in between the lines labelled
    // 1 and 2 below and the write index subsequently wraps below the read index

    do {
      readIndex=_readIndex;
      lastOpRead=(readIndex & LAST_OP_READ_FLAG)!=0;
      readIndex&=READ_INDEX_MASK;

      if(_writeIndex==readIndex)
        ret=lastOpRead ? 0 : _size;
      else if(_writeIndex>readIndex)    // 1
        ret=_writeIndex-readIndex;      // 2
      else
        ret=_size-readIndex+_writeIndex;
    } while(ret>_size);

    return ret;
  }


  /**
   * Read a number of types from the buffer. It's your responsibility to ensure that the
   * types are there to read.
   * @param output Where to store the data
   * @param size How many types to read
   */

  template<typename T>
  inline void circular_buffer<T>::read(T *output,uint32_t size) volatile {

    uint32_t pos;

    for(pos=(_readIndex & READ_INDEX_MASK);size!=0;size--) {

      *output++=_buffer[pos];

      if(++pos==_size)
        pos=0;
    }

    _readIndex=pos | LAST_OP_READ_FLAG;
  }


  /*
   * Read a single type
   * @return The next type
   */

  template<typename T>
  inline T circular_buffer<T>::read() volatile {

    uint32_t pos;
    T retval;

    pos=_readIndex & READ_INDEX_MASK;
    retval=_buffer[pos];

    if(++pos==_size)
      pos=0;

    _readIndex=pos | LAST_OP_READ_FLAG;
    return retval;
  }


  /**
   * Write a number of types to the internal buffer. It's your responsibility to ensure that there
   * is sufficient space in the circular buffer to write your data.
   * @param input Your buffer to copy from
   * @param size The number of types to write
   */

  template<typename T>
  inline void circular_buffer<T>::write(const T *input,uint32_t size) volatile {

    uint32_t pos;

    // operate on local version of write index until we're sure it's valid when written back.

    for(pos=_writeIndex;size!=0;size--) {

      _buffer[pos]=*input++;

      if(++pos==_size)
        pos=0;
    }

    _writeIndex=pos;
    _readIndex&=~LAST_OP_READ_FLAG;
  }


  /*
   * Write a single type
   * @param input the single type to write
   */

  template<typename T>
  inline void circular_buffer<T>::write(const T& input) volatile {

    uint32_t pos;

    // operate on local version of write index until we're sure it's valid when written back.

    pos=_writeIndex;
    _buffer[pos]=input;

    if(++pos==_size)
      pos=0;

    _writeIndex=pos;
    _readIndex&=~LAST_OP_READ_FLAG;
  }
}
