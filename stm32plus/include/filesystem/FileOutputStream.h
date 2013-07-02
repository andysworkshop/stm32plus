/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
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

      virtual bool flush() override {
        return true;
      }
  };
}
