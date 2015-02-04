/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


namespace stm32plus {


  /**
   * Helper class to read from a byte array using the InputStream interface. This class does
   * not take ownership of the byte array. It is expected that the user will keep it in scope
   * while this class is in use.
   */

  class ByteArrayInputStream : public InputStream {

    public:
      enum {
        E_INVALID_SEEK_POSITION = 1
      };

    protected:
      const uint8_t *_data;
      uint32_t _size;
      uint32_t _pos;

    public:
      ByteArrayInputStream(const void *data,uint32_t size);
      virtual ~ByteArrayInputStream() {}

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;

      // overrides from StreamBase

      virtual bool close() override;
  };


  /**
   * Constructor
   * @param data The string pointer
   * @param size The data size in bytes
   */

  inline ByteArrayInputStream::ByteArrayInputStream(const void *data,uint32_t size)
    : _data(reinterpret_cast<const uint8_t *>(data)),
      _size(size),
      _pos(0) {
  }


  /**
   * Read a single byte
   * @return the byte or E_END_OF_STREAM
   */

  inline int16_t ByteArrayInputStream::read() {

    if(_pos==_size)
      return E_END_OF_STREAM;

    return _data[_pos++];
  }


  /**
   * Read many characters
   * @param buffer Where to read out to
   * @param size Number requested
   * @param actuallyRead Number actually read out
   * @return true, always. zero actually read indicates EOF
   */

  inline bool ByteArrayInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    actuallyRead=std::min(size,static_cast<uint32_t>(_size-_pos));

    if(actuallyRead) {
      memcpy(buffer,_data+_pos,actuallyRead);
      _pos+=actuallyRead;
    }

    return true;
  }


  /**
   * Seek forward in the stream
   * @param howMuch amount to seek
   * @return true if OK, false if tried to seek past end
   */

  inline bool ByteArrayInputStream::skip(uint32_t howMuch) {

    // check validity of how much

    if(howMuch>_size-_pos)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_BYTE_ARRAY_INPUT_STREAM,E_INVALID_SEEK_POSITION);

    // move forward

    _pos+=howMuch;
    return true;
  }


  /**
   * Return true if some data is available for reading
   * @return true if data is available
   */

  inline bool ByteArrayInputStream::available() {
    return _size!=_pos;
  }


  /**
   * Reset to the start of the stream
   * @return Always true
   */

  inline bool ByteArrayInputStream::reset() {
    _pos=0;
    return true;
  }


  /**
   * Do nothing close
   * @return true always
   */

  inline bool ByteArrayInputStream::close() {
    return true;
  }
}
