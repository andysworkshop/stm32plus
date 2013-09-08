/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once


namespace stm32plus {

  /**
   * Template implementation of a serial EEPROM. Inherits from InputStream
   * and OutputStream. Provides the functionality for maintaining the stream pointer.
   * Delegates the actual read and write operations to the TImpl class parameter
   * @tparam TImpl The device implementation (this is the CRTP template pattern)
   */

  template<class TImpl>
  class SerialEeprom : public InputStream,
                       public OutputStream {

    protected:
      TImpl& _impl;
      uint32_t _position;

    public:

      /**
       * Error codes
       */

      enum {
        E_INVALID_SEEK_POSITION = 1,    ///< can't seek past the end
        E_INVALID_SIZE = 2              ///< can't write past the end
      };

    public:
      SerialEeprom(TImpl& impl);

      bool seek(uint32_t position);

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;
      virtual bool close() override { return true; }
      virtual bool flush() override { return true; }

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;
  };


  /**
   * Constructor
   */

  template<class TImpl>
  inline SerialEeprom<TImpl>::SerialEeprom(TImpl& impl)
    : _impl(impl) {

    _position=0;
  }


  /**
   * "seek" to a new position on the EEPROM. This just stores the new position which the
   * implementation class must use the next time it gets asked to do a read or a write.
   * @param[in] position The new position to seek to.
   * @return false if the position is out of range for this device.
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::seek(uint32_t position) {

    if(position<TImpl::SIZE_IN_BYTES) {
      _position=position;
      return true;
    }

    // out of range

    return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SERIAL_EEPROM,E_INVALID_SEEK_POSITION);
  }


  /**
   * Write a single byte
   * @param c The byte to write
   * @return true if it worked
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::write(uint8_t c) {

    // can't write if at the end

    if(_position>=TImpl::SIZE_IN_BYTES)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SERIAL_EEPROM,OutputStream::E_END_OF_STREAM);

    // do the write

    return _impl.writeByte(c);
  }


  /**
   * Write many bytes
   * @param buffer the source of data
   * @param size the number of bytes to write
   * @return true if it worked
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::write(const void *buffer,uint32_t size) {

    // can't write past the end

    if(_position+size>TImpl::SIZE_IN_BYTES)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SERIAL_EEPROM,E_INVALID_SIZE);

    // do the write

    return _impl.writeBytes(reinterpret_cast<const uint8_t *>(buffer),size);
  }


  /**
   * Read a single byte
   * @return Either the byte read in the lower 8-bits of the return value or
   * E_END_OF_STREAM / E_STREAM_ERROR. In the case of E_STREAM_ERROR the error
   * provider will have the detailed reason.
   */

  template<class TImpl>
  inline int16_t SerialEeprom<TImpl>::read() {

    uint8_t c;

    // check for the end

    if(_position>=TImpl::SIZE_IN_BYTES)
      return InputStream::E_END_OF_STREAM;

    // try the read

    if(!_impl.readByte(c))
      return InputStream::E_STREAM_ERROR;

    return c;
  }


  /**
   * Read many bytes
   * @param buffer Where to read to
   * @param size The number of bytes to read
   * @param actuallyRead The number of bytes read, which can be less than the total requested
   * if the end of stream is hit. If the pointer is already at the end of stream then zero
   * is returned in actuallyRead.
   * @return false if it fails, and actuallyRead will be undefined
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    // cut down the size if there would be an overflow

    if(_position+size>TImpl::SIZE_IN_BYTES)
      size=TImpl::SIZE_IN_BYTES-_position;

    // do the read

    if(!_impl.readBytes(reinterpret_cast<uint8_t *>(buffer),size))
      return false;

    // update the bytes read

    actuallyRead=size;
    return true;
  }


  /**
   * Skip forward, if possible
   * @param howMuch How far to skip forward
   * @return true if it worked
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::skip(uint32_t howMuch) {

    // check for out of range

    if(_position+howMuch>=TImpl::SIZE_IN_BYTES)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SERIAL_EEPROM,E_INVALID_SEEK_POSITION);

    // update the position

    _position+=howMuch;
    return true;
  }


  /**
   * Check if there is at least one byte available
   * @return true if at least one byte is available
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::available() {
    return _position<TImpl::SIZE_IN_BYTES;
  }


  /**
   * Reset the stream pointer to position zero.
   * @return Always returns true
   */

  template<class TImpl>
  inline bool SerialEeprom<TImpl>::reset() {
    _position=0;
    return true;
  }
}
