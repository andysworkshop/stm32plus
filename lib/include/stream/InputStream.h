/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Abstract class defining how an input stream behaves.
   *
   * Input streams give you the ability to read data sequentially. The base
   * class supplies an implementation of the popular stream operators
   * << and >>. These should be used with care as error handling is
   * not easily implemented.
   */

  class InputStream : public StreamBase {

    protected:
      InputStream& opRead(void *buffer,uint32_t bufferSize);

    public:
      virtual ~InputStream() {}

      // some convenience operators when you don't care about errors or endian-ness

      InputStream& operator>>(int8_t& c);
      InputStream& operator>>(int16_t& c);
      InputStream& operator>>(int32_t& c);
      InputStream& operator>>(uint8_t& c);
      InputStream& operator>>(uint16_t& c);
      InputStream& operator>>(uint32_t& c);

      /**
       * Read a byte from the stream, potentially blocking until data
       * is available.
       *
       * @return If the read succeeds then the byte
       * is returned in the lower 8 bytes, hence the return value is in the
       * range 0..255. If the read fails then the return value is E_STREAM_ERROR.
       * If the end of the stream has been reached then the return value is
       * E_END_OF_STREAM.
       */

      virtual int16_t read()=0;

      /**
       * Read a sequence of bytes from the stream, potentially blocking
       * if data is not available.
       *
       * @param[in,out] buffer The location to store the data.
       * @param[in] size The number of bytes to try to read.
       * @param[in] actuallyRead The number of bytes actually read, which may be less than the size
       *  requested if the end of the file is reached.
       * @return false if the read fails.
       */

      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead)=0;

      /**
       * Seek forward by the given number of bytes.
       * @param[in] howMuch The amount to try to seek.
       * @return false if it fails.
       */

      virtual bool skip(uint32_t howMuch)=0;

      /**
       * Peek into a blocking stream and return true if data is available
       * to read. Allows the caller to prevent blocking by checking if there
       * is data available.
       *
       * @return true if there is data available, false if not.
       */

      virtual bool available()=0;


      /**
       * Seek the stream back to the beginning.
       * @return false if the stream does not support being reset or an error occurred.
       */

      virtual bool reset()=0;
  };
}
