/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Base class for a stream that maintains a memory buffer of bytes.
   */

  class BufferedInputOutputStream : public InputStream, public OutputStream {

    protected:
      uint8_t *_buffer;
      uint32_t _bufferSize;
      volatile uint8_t *_readPtr;
      volatile uint8_t *_writePtr;
      bool _needToFree;

    public:

      /**
       * Error codes
       */

      enum {
        /// invalid seek size, you probably tried to seek forward beyond the available data
        E_INVALID_SEEK_SIZE=1,
        /// There is not enough space in the buffer for this operation
        E_BUFFER_FULL
      };

    public:

      /**
       * Constructor. Allocates memory for the initial size of the buffer.
       *
       * @param[in] initialSize The size to allocate.
       */

      BufferedInputOutputStream(uint32_t initialSize) {
        _readPtr=_writePtr=_buffer=static_cast<uint8_t *> (malloc(initialSize));
        _bufferSize=initialSize;
        _needToFree=true;
      }


      /**
       * Constructor. Buffer is provided by caller and will not be freed here.
       * @param[in] buffer The caller-allocated buffer.
       * @param[in] size The size of the caller supplied buffer
       */

      BufferedInputOutputStream(void *buffer,uint32_t size) {
        _readPtr=_writePtr=_buffer=static_cast<uint8_t*>(buffer);
        _bufferSize=size;
        _needToFree=false;
      }

      /**
       * Destructor, frees up the internal buffer.
       */

      virtual ~BufferedInputOutputStream() {
        if(_needToFree)
          free(_buffer);
      }

  };
}
