/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/filesystem.h"


namespace stm32plus {

  /**
   * Constructor with file.
   * @param[in] file_ The file to write to. Caller supplied parameter, must not go out of scope.
   */

  FileOutputStream::FileOutputStream(File& file_) :
    _file(file_) {
  }

  /*
   * Write data to the file
   */

  bool FileOutputStream::write(const void *buffer,uint32_t size_) {
    return _file.write(buffer,size_);
  }

  /*
   * no-op close
   */

  bool FileOutputStream::close() {
    return true;
  }

  /*
   * Write byte to file
   */

  bool FileOutputStream::write(uint8_t c_) {
    return _file.write(&c_,sizeof(c_));
  }
}
