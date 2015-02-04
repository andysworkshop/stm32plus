/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


namespace stm32plus {


  /**
   * Helper class to read from an STL string using the InputStream interface
   */

  class StlStringInputStream : public InputStream {

    public:
      enum {
        E_INVALID_SEEK_POSITION = 1
      };

    protected:
      const std::string *_string;
      bool _delete;
      std::string::size_type _pos;

    public:
      StlStringInputStream(const std::string *str,bool takeOwnership);
      virtual ~StlStringInputStream();

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
   * @param str The string pointer
   * @param takeOwnership true if we must delete on destruction
   */

  inline StlStringInputStream::StlStringInputStream(const std::string *str,bool takeOwnership)
    : _string(str),
      _delete(takeOwnership),
      _pos(0) {
  }


  /**
   * Destructor
   */

  inline StlStringInputStream::~StlStringInputStream() {
    if(_delete)
      delete _string;
  }


  /**
   * Read a single byte
   * @return the byte or E_END_OF_STREAM
   */

  inline int16_t StlStringInputStream::read() {

    if(_pos==_string->length())
      return E_END_OF_STREAM;

    return (*_string)[_pos++];
  }


  /**
   * Read many characters
   * @param buffer Where to read out to
   * @param size Number requested
   * @param actuallyRead Number actually read out
   * @return true, always. zero actually read indicates EOF
   */

  inline bool StlStringInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {

    actuallyRead=std::min(size,static_cast<uint32_t>(_string->length()-_pos));

    if(actuallyRead) {
      memcpy(buffer,_string->c_str()+_pos,actuallyRead);
      _pos+=actuallyRead;
    }

    return true;
  }


  /**
   * Seek forward in the stream
   * @param howMuch amount to seek
   * @return true if OK, false if tried to seek past end
   */

  inline bool StlStringInputStream::skip(uint32_t howMuch) {

    // check validity of how much

    if(howMuch>_string->length()-_pos)
      return errorProvider.set(ErrorProvider::ERROR_PROVIDER_STL_STRING_INPUT_STREAM,E_INVALID_SEEK_POSITION);

    // move forward

    _pos+=howMuch;
    return true;
  }


  /**
   * Return true if some data is available for reading
   * @return true if data is available
   */

  inline bool StlStringInputStream::available() {
    return _string->length()!=_pos;
  }


  /**
   * Reset to the start of the stream
   * @return Always true
   */

  inline bool StlStringInputStream::reset() {
    _pos=0;
    return true;
  }


  /**
   * Do nothing close
   * @return true always
   */

  inline bool StlStringInputStream::close() {
    return true;
  }
}
