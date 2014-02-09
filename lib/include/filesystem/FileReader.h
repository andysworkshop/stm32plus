/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {


  /**
   * Extension of FileInputStream to provide the facility to read lines
   * from a file. This is unbuffered, hence can be slow.
   */

  class FileReader : public Reader {

    protected:
      FileInputStream _fis;

    public:
      FileReader(File& file);
      virtual ~FileReader() {}

      // overrides from Reader

      virtual bool readLine(char *buffer,uint32_t maxLength) override;

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool close() override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;
  };


  /**
   * Constructor
   * @param file The file to read from
   */

  inline FileReader::FileReader(File& file)
    : _fis(file) {
  }


  /**
   * Read a line from the file
   * @param buffer Where to store the characters
   * @param maxLength The most to read, including a null
   * @return true if it worked, false for failure
   */

  inline bool FileReader::readLine(char *buffer,uint32_t maxLength) {

    char *ptr;
    int16_t c;

    // set up pointer and decrease maxlength for the \0 terminator

    ptr=buffer;
    maxLength--;

    for(;;) {

      // get and check for error

      if((c=_fis.read())==E_STREAM_ERROR)
        return false;

      // check for EOF or EOL

      if(c==E_END_OF_STREAM || c=='\n')
        break;

      // store character if not a CR and there is space

      if(c!='\r' && maxLength) {

        *ptr++=c;
        maxLength--;
      }
    }

    // terminate the line

    *ptr='\0';
    return true;
  }


  /*
   * InputStream overrides deferred to the contained class
   */

  inline int16_t FileReader::read() {
    return _fis.read();
  }

  inline bool FileReader::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {
    return _fis.read(buffer,size,actuallyRead);
  }

  inline bool FileReader::close() {
    return _fis.close();
  }

  inline bool FileReader::skip(uint32_t howMuch) {
    return _fis.skip(howMuch);
  }

  inline bool FileReader::available() {
    return _fis.available();
  }

  inline bool FileReader::reset() {
    return _fis.reset();
  }
}

