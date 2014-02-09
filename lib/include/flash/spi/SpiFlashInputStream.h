/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * @brief Implementation of an input stream for SPI flash devices. The SPI flash device is modelled as
     * a very large sequence of bytes on to which you can map an input stream of a defined length. This
     * class accepts an initial offset and a size parameter which together define the segment of the flash
     * device that you want to read from.
     * @tparam TSpiFlash An object that implements fastRead()
     */

    template<class TSpiFlash>
    class SpiFlashInputStream : public InputStream {

      public:

        /**
         * Error codes
         */

        enum {
          E_INVALID_SKIP_POSITION = 1       //!< attempt to skip() past the end
        };

      protected:
        const TSpiFlash& _spiFlash;
        uint32_t _initialOffset;
        uint32_t _offset;
        uint32_t _size;

      public:
        SpiFlashInputStream(const TSpiFlash& spiFlash,uint32_t initialOffset,uint32_t size);
        virtual ~SpiFlashInputStream() {}

        uint32_t remaining() const;

        // overrides from InputStream

        virtual int16_t read() override;
        virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
        virtual bool skip(uint32_t howMuch) override;
        virtual bool available() override;
        virtual bool close() override;
        virtual bool reset() override;
    };


    /**
     * Constructor
     */

    template<class TSpiFlash>
    inline SpiFlashInputStream<TSpiFlash>::SpiFlashInputStream(const TSpiFlash& spiFlash,uint32_t initialOffset,uint32_t size)
      : _spiFlash(spiFlash),
        _initialOffset(initialOffset),
        _offset(initialOffset),
        _size(size) {
    }


    /**
     * Doesn't do anything.
     * @return always true
     */

    template<class TSpiFlash>
    inline bool SpiFlashInputStream<TSpiFlash>::close() {
      return true;
    }


    /**
     * Reset the stream pointer to the beginning
     * @return true
     */

    template<class TSpiFlash>
    inline bool SpiFlashInputStream<TSpiFlash>::reset() {
      _offset=_initialOffset;
      return true;
    }


    /**
     * Read a single byte
     * @return The byte in the lower 8 bits or E_END_OF_STREAM / E_STREAM ERROR (negative numbers)
     */

    template<class TSpiFlash>
    inline int16_t SpiFlashInputStream<TSpiFlash>::read() {

      uint8_t c;
      uint32_t actuallyRead;

      // must have at least a byte available

      if(!available())
        return E_END_OF_STREAM;

      // try to read a byte

      if(!read(&c,1,actuallyRead))
        return E_STREAM_ERROR;

      // return the byte in the lower bits

      return c;
    }


    /**
     * Read a chunk of bytes up to the amount requested.
     * @param buffer Where to read out the bytes to
     * @param size The maximum number of bytes to read
     * @param actuallyRead The actual number of bytes read, which may be less than requested if the end of stream is hit.
     * @return false if there was an error. end of stream is not an error.
     */

    template<class TSpiFlash>
    inline bool SpiFlashInputStream<TSpiFlash>::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

      // trim the requested size if not enough remains

      actuallyRead=remaining() < size ? remaining() : size;

      // return now if we're at the EOF

      if(actuallyRead==0)
        return true;

      // attempt to read the bytes

      if(!_spiFlash.fastRead(_offset,buffer,actuallyRead))
        return false;

      // update the read pointer

      _offset+=actuallyRead;
      return true;
    }


    /**
     * Skip a number of bytes forward
     * @param howMuch Amount to skip - can go to EOF but not beyond
     * @return true if OK
     */

    template<class TSpiFlash>
    inline bool SpiFlashInputStream<TSpiFlash>::skip(uint32_t howMuch) {

      if(howMuch>remaining())
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SPI_FLASH_INPUT_STREAM,E_INVALID_SKIP_POSITION);

      _offset+=howMuch;
      return true;
    }


    /**
     * Return true if at least one byte can be read
     * @return true if reading is possible
     */

    template<class TSpiFlash>
    inline bool SpiFlashInputStream<TSpiFlash>::available() {
      return remaining()>0;
    }


    /**
     * Return the amount of bytes remaining to read
     * @return The number of bytes remaining
     */

    template<class TSpiFlash>
    inline uint32_t SpiFlashInputStream<TSpiFlash>::remaining() const {
      return _size-(_offset-_initialOffset);
    }
  }
}
