/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {

  /**
   * Constructor with file
   * @param[in] file_ The file to read the data from. Caller supplied, must not go out of scope.
   */

  FileInputStream::FileInputStream(File& file_) :
    _file(file_) {
  }

  /*
   * Read data from the file
   */

  bool FileInputStream::read(void *buffer,uint32_t size_,uint32_t& actuallyRead_) {
    return _file.read(buffer,size_,actuallyRead_);
  }

  /*
   * no-op close
   */

  bool FileInputStream::close() {
    return true;
  }

  /*
   * Skip some distance. Seeking past the end is an error.
   */

  bool FileInputStream::skip(uint32_t howMuch_) {
    return _file.seek(howMuch_,File::SeekCurrent);
  }

  /*
   * Check if anything available
   */

  bool FileInputStream::available() {
    return _file.getOffset() < _file.getLength();
  }

  /*
   * Read the next byte from the stream (0..255, or -1 for EOF, or -2 for error)
   */

  int16_t FileInputStream::read() {

    uint8_t value;
    uint32_t actuallyRead;

    if(!_file.read(&value,1,actuallyRead))
      return E_STREAM_ERROR;

    return actuallyRead == 0 ? static_cast<int16_t>(E_END_OF_STREAM) : value;
  }


  /*
   * Reset to start of stream
   */

  bool FileInputStream::reset() {
    return _file.seek(0,File::SeekStart);
  }
}
