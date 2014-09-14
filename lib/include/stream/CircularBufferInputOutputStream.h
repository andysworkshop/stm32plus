/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Buffered IO stream operating as a circular buffer.
   *
   * The buffer is non-blocking and it will not overwrite data that has not been read. The
   * read and write functions will return errors if the buffer is full. Read functions will
   * set E_END_OF_STREAM when there is no data available. Write functions will return E_BUFFER_FULL
   * if there is not enough space for the write.
   *
   * This is designed to be safe for using with interrupts. e.g. an ISR writes to to the class
   * and the main code reads from it, or vice-versa. The write() and read() functions do not share
   * any internal state members and all state members are <= the native word size of the MCU.
   */

  class CircularBufferInputOutputStream : public BufferedInputOutputStream {

    protected:
      bool _wrappedWrite;

    public:

      /**
       * Constructor.
       * @param fixedSize The fixed size of the circular buffer. This parameter should be set to reflect how far 'ahead'
       *    writes can get from reads in the buffer.
       */

      CircularBufferInputOutputStream(uint32_t fixedSize) :
        BufferedInputOutputStream(fixedSize) {
        _wrappedWrite=false;
      }

      bool isFull() const;

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;

      /**
       * @copydoc InputStream::skip
       */

      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;

      virtual bool flush() override {
        return true;
      }

      /**
       * Cannot reset this stream
       * @return Always false and sets error code to E_OPERATION_NOT_SUPPORTED
       */

      virtual bool reset() override {
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_CIRCULAR_BUFFER_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
      }

      // common methods

      virtual bool close() override {
        return true;
      }
  };


  /**
   * Check if the buffer is full
   * @return true if it's full
   */

  inline bool CircularBufferInputOutputStream::isFull() const {
    return _readPtr==_writePtr && _wrappedWrite;
  }
}
