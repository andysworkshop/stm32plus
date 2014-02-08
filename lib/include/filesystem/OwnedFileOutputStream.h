/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Subclass of a file output stream that owns the file pointer
   * and will delete it when the stream is deleted
   */

  struct OwnedFileOutputStream : FileOutputStream {

    /**
     * Constructor
     * @param file The file pointer
     */

    OwnedFileOutputStream(File *file) :
      FileOutputStream(*file) {
    }


    /**
     * Destructor, delete the file pointer
     */

    virtual ~OwnedFileOutputStream() {
      delete &_file;
    }
  };
}
