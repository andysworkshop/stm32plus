/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of an output stream for files
   */

  class FileOutputStream : public OutputStream {
    protected:
      File& _file;

    public:
      FileOutputStream(File& f);

    public:

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;
      virtual bool close() override;

      virtual bool flush() override;
  };


  /**
   * Constructor with file.
   * @param[in] file The file to write to. Caller supplied parameter, must not go out of scope.
   */

  inline FileOutputStream::FileOutputStream(File& file)
    : _file(file) {
  }


  /**
   * Write data to the file
   * @param buffer source of your data
   * @param size How much to write
   */

  inline bool FileOutputStream::write(const void *buffer,uint32_t size) {
    return _file.write(buffer,size);
  }


  /**
   * no-op close
   * @return true
   */

  inline bool FileOutputStream::close() {
    return true;
  }


  /**
   * Write byte to file
   * @param c The byte to write
   * @return true if it worked
   */

  inline bool FileOutputStream::write(uint8_t c) {
    return _file.write(&c,sizeof(c));
  }


  /**
   * No-op flush
   * @return true always
   */

  inline bool FileOutputStream::flush() {
    return true;
  }
}
