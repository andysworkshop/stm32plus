/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once



namespace stm32plus {


  /**
   * Template class for a CRC output stream.
   */

  template<class TCrc>
  class CrcOutputStream : public OutputStream {

    protected:
      TCrc& _crc;

    public:

      CrcOutputStream(TCrc& crc);
      virtual ~CrcOutputStream() {}

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;
      virtual bool flush() override;
      virtual bool close() override;
  };


  /**
   * Constructor
   * @param crc The CRC peripheral type
   */

  template<class TCrc>
  inline CrcOutputStream<TCrc>::CrcOutputStream(TCrc& crc)
    : _crc(crc) {
  }


  /**
   * Write a byte
   * @param c The byte
   * @return always true
   */

  template<class TCrc>
  inline bool CrcOutputStream<TCrc>::write(uint8_t c) {
    _crc.addNewData(c);
    return true;
  }


  /**
   * Write a buffer of bytes
   * @param buffer the buffer
   * @param size The number of bytes
   * @return Always true
   */

  template<class TCrc>
  inline bool CrcOutputStream<TCrc>::write(const void *buffer,uint32_t size) {

    const uint8_t *ptr=reinterpret_cast<const uint8_t *>(buffer);

    while(size--)
      _crc.addNewData(*ptr++);

    return true;
  }


  /**
   * Always true.
   * @return always true
   */

  template<class TCrc>
  inline bool CrcOutputStream<TCrc>::flush() {
    return true;
  }


  /**
   * Always true. Calls finish() on the underlying CRC
   * @return always true
   */

  template<class TCrc>
  inline bool CrcOutputStream<TCrc>::close() {
    _crc.finish();
    return true;
  }
}
