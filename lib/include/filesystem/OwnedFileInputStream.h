/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Subclass of a file input stream that owns the file pointer
   * and will delete it when the stream is deleted
   */

  struct OwnedFileInputStream : FileInputStream {

    /**
     * Constructor
     * @param file The file pointer
     */

    OwnedFileInputStream(File *file) :
      FileInputStream(*file) {
    }


    /**
     * Destructor, delete the file pointer
     */

    virtual ~OwnedFileInputStream() {
      delete &_file;
    }
  };
}
