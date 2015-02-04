/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


namespace stm32plus {


  /**
   * Helper stream to provide read-ahead capabilities for the stream it wraps. This is useful
   * for when it is expensive to read small amounts from the underlying stream
   */

  class ReadAheadInputStream : public InputStream {

    protected:
      InputStream& _is;
      uint8_t *_buffer;
      uint32_t _pos;
      uint32_t _bufferSize;
      uint32_t _maxBufferSize;

    protected:
      bool refillBuffer();

    public:
      ReadAheadInputStream(InputStream& is,uint32_t bufferSize);
      virtual ~ReadAheadInputStream() {}

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
   * @param is The wrapped stream
   * @param bufferSize The internal buffer size
   */

  inline ReadAheadInputStream::ReadAheadInputStream(InputStream& is,uint32_t bufferSize)
    : _is(is),
      _buffer(reinterpret_cast<uint8_t *>(malloc(bufferSize))),
      _pos(bufferSize),
      _bufferSize(bufferSize),
      _maxBufferSize(bufferSize) {
  }


  /**
   * Read a single byte
   * @return the byte or E_END_OF_STREAM
   */

  inline int16_t ReadAheadInputStream::read() {

    if(_pos==_bufferSize) {

      // get some more bytes

      if(!refillBuffer())
        return E_STREAM_ERROR;

      // none means end of stream

      if(_pos==_bufferSize)
        return E_END_OF_STREAM;
    }

    return _buffer[_pos++];
  }


  /**
   * Read many characters
   * @param buffer Where to read out to
   * @param size Number requested
   * @param actuallyRead Number actually read out
   * @return true, always. zero actually read indicates EOF
   */

  inline bool ReadAheadInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    uint8_t *ptr;
    uint32_t count;

    actuallyRead=0;
    ptr=reinterpret_cast<uint8_t *>(buffer);

    while(size) {

      // ensure there's data

      if(_pos==_bufferSize) {

        if(!refillBuffer())
          return false;

        // check for end of stream

        if(_pos==_bufferSize)
          return true;
      }

      count=std::min(_bufferSize-_pos,size);
      memcpy(ptr,_buffer+_pos,count);
      _pos+=count;

      ptr+=count;
      size-=count;
      actuallyRead+=count;
    }

    return true;
  }


  /**
   * Refill the internal buffer from the stream
   * @return true if no error
   */

  inline bool ReadAheadInputStream::refillBuffer() {

    uint32_t actuallyRead;

    // try to read max from underlying stream

    if(!_is.read(_buffer,_maxBufferSize,actuallyRead))
      return false;

    // EOF check

    if(actuallyRead==0)
      return true;

    // adjust pointers

    _pos=0;
    _bufferSize=actuallyRead;
    return true;
  }


  /**
   * Seek forward in the stream
   * @param howMuch amount to seek
   * @return unsupported operation
   */

  inline bool ReadAheadInputStream::skip(uint32_t /* howMuch */) {
    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_READ_AHEAD_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
  }


  /**
   * Return true if some data is available for reading
   * @return true if data is available
   */

  inline bool ReadAheadInputStream::available() {
    return _pos!=_bufferSize || _is.available();
  }


  /**
   * Reset to the start of the stream
   * @return Always true
   */

  inline bool ReadAheadInputStream::reset() {
    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_READ_AHEAD_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
  }


  /**
   * Do nothing close
   * @return true always
   */

  inline bool ReadAheadInputStream::close() {
    return true;
  }
}
