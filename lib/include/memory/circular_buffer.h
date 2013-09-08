/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Template class to manage a circular buffer of types. Separate read and write pointers are
   * maintained. Methods are provided to read from and write to the buffer. More methods are provided
   * to find out how much read space and write space is available without overrunning the other
   * position.
   */

  template<typename T>
  class circular_buffer {

    protected:

      T *_buffer;
      uint32_t _size;
      uint32_t _readIndex;
      uint32_t _writeIndex;
      bool _lastOpRead;

    public:
      circular_buffer(uint32_t size);
      ~circular_buffer();

      void read(T *output,uint32_t size) volatile;
      void write(const T *input,uint32_t size) volatile;

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
      _readIndex(0),
      _writeIndex(0),
      _lastOpRead(true) {
  }


  /**
   * Destructor
   */

  template<typename T>
  inline circular_buffer<T>::~circular_buffer() {
    delete _buffer;
  }


  /**
   * get the number of types that available to write without overrunning the read pointer
   * @return The number of available types
   */

  template<typename T>
  inline uint32_t circular_buffer<T>::availableToWrite() const volatile {

    if(_writeIndex==_readIndex)
      return _lastOpRead ? _size : 0;
    else if(_writeIndex>_readIndex)
      return _size-_writeIndex+_readIndex;
    else
      return _readIndex-_writeIndex;
  }


  /**
   * get the number of types that available to read without overrunning the write pointer
   * @return The number of available types
   */

  template<typename T>
  inline uint32_t circular_buffer<T>::availableToRead() const volatile {

    if(_writeIndex==_readIndex)
      return _lastOpRead ? 0 : _size;
    else if(_writeIndex>_readIndex)
      return _writeIndex-_readIndex;
    else
      return _size-_readIndex+_writeIndex;
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

    for(pos=_readIndex;size!=0;size--) {

      *output++=_buffer[pos];

      if(++pos==_size)
        pos=0;
    }

    _readIndex=pos;
    _lastOpRead=true;
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

    for(pos=_writeIndex;size!=0;size--) {

      _buffer[pos]=*input++;

      if(++pos==_size)
        pos=0;
    }

    _writeIndex=pos;
    _lastOpRead=false;
  }
}
