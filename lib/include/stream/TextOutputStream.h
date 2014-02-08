/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * This class wraps an existing output stream and provides additional functionality
   * to write out text values using the << operator. e.g. stream << 3 will write
   * the text string "3" to the stream instead of the binary integer 3.
   */

  class TextOutputStream : public OutputStream {

    protected:
      OutputStream& _stream;

    public:

      TextOutputStream(OutputStream& stream);

      // can't do these as a template with specialisation because you can't specialise
      // members in a template class that isn't also fully specialised

      TextOutputStream& operator<<(const char *str);
      TextOutputStream& operator<<(char c);
      TextOutputStream& operator<<(int32_t val);
      TextOutputStream& operator<<(uint32_t val);
      TextOutputStream& operator<<(int16_t val);
      TextOutputStream& operator<<(uint16_t val);
      TextOutputStream& operator<<(const DoublePrecision& val);
      TextOutputStream& operator<<(double val);

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;
      virtual bool close() override;
      virtual bool flush() override;
  };


  /**
   * Constructor
   * @param stream The stream that we are wrapping
   */

  inline TextOutputStream::TextOutputStream(OutputStream& stream)
    : _stream(stream) {

  }

  /**
   * character string operator
   * @param str The character string
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(const char *str) {
    write(str,strlen(str));
    return *this;
  }


  /**
   * Character writer
   * @param c The character
   * @return self-reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(char c) {
    write(c);
    return *this;
  }


  /**
   * integer writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(int32_t val) {

    char buf[15];
    StringUtil::itoa(val,buf,10);
    write(buf,strlen(buf));

    return *this;
  }


  /**
   * Unsigned int writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint32_t val) {

    char buf[15];
    StringUtil::modp_uitoa10(val,buf);
    write(buf,strlen(buf));

    return *this;
  }


  /**
   * Signed 16-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(int16_t val) {
    return operator<<((int32_t)val);
  }


  /**
   * Unsigned 16-bit writer
   * @param val The integer
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(uint16_t val) {
    return operator<<((uint32_t)val);
  }


  /**
   * Double writer
   * @param val The double to write
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(const DoublePrecision& val) {

    char str[25];

    StringUtil::modp_dtoa(val.Value,val.Precision,str);
    write(str,strlen(str));
    return *this;
  }


  /**
   * Double writer
   *  @param val The double to write
   * @return self reference
   */

  inline TextOutputStream& TextOutputStream::operator<<(double val) {
    operator<<(DoublePrecision(val,DoublePrecision::MAX_DOUBLE_FRACTION_DIGITS));
    return *this;
  }


  /**
   * Write a byte - call through to the underlying stream
   * @param buffer The buffer
   * @param size The size
   * @return the underlying stream success result
   */

  inline bool TextOutputStream::write(uint8_t c) {
    return _stream.write(c);
  }


  /**
   * Write a buffer - call through to the underlying stream
   * @param buffer The buffer
   * @param size The size
   * @return the underlying stream success result
   */

  inline bool TextOutputStream::write(const void *buffer,uint32_t size) {
    return _stream.write(buffer,size);
  }


  /**
   * Close the stream (do nothing)
   * @return true
   */

  inline bool TextOutputStream::close() {
    return true;
  }


  /**
   * Flush the stream
   * @return the underlying stream flush result
   */

  inline bool TextOutputStream::flush() {
    return _stream.flush();
  }
}

