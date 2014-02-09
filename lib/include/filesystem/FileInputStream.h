/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of an input stream for files
   */

  class FileInputStream : public InputStream {

    protected:
      File& _file;

    public:
      FileInputStream(File& file);
      virtual ~FileInputStream() {}

    public:

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool close() override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;
  };


  /**
   * Constructor with file
   * @param[in] file The file to read the data from. Caller supplied, must not go out of scope.
   */

  inline FileInputStream::FileInputStream(File& file) :
    _file(file) {
  }


  /**
   * Read data from the file
   * @param buffer where to read out to
   * @param size how much to try to read
   * @param[out] actuallyRead how much was read (may be zero for EOF)
   * @return false on error.
   */

  inline bool FileInputStream::read(void *buffer,uint32_t size,uint32_t& actuallyRead) {
    return _file.read(buffer,size,actuallyRead);
  }


  /**
   * no-op close
   * @return always true
   */

  inline bool FileInputStream::close() {
    return true;
  }


  /*
   * Skip some distance. Seeking past the end is an error.
   * @param howMuch How far to seek forward.
   * @return true if it worked
   */

  inline bool FileInputStream::skip(uint32_t howMuch) {
    return _file.seek(howMuch,File::SeekCurrent);
  }


  /**
   * Check if anything available
   * @return true if there is at least a byte available
   */

  inline bool FileInputStream::available() {
    return _file.getOffset() < _file.getLength();
  }


  /**
   * Read the next byte from the stream (0..255, or E_END_OF_STREAM for EOF, or E_STREAM_ERROR for error)
   * @return The next byte, or an error code
   */

  inline int16_t FileInputStream::read() {

    uint8_t value;
    uint32_t actuallyRead;

    if(!_file.read(&value,1,actuallyRead))
      return E_STREAM_ERROR;

    return actuallyRead == 0 ? static_cast<int16_t>(E_END_OF_STREAM) : value;
  }


  /**
   * Reset to start of stream
   * @return true if it worked
   */

  inline bool FileInputStream::reset() {
    return _file.seek(0,File::SeekStart);
  }
}
