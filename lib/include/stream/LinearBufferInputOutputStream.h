/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Class to support a linear I/O buffer.
   *
   * The buffer has a start and an end and
   * does not wrap around like the circular buffer. The reset() call can be used to reset
   * both the input pointer and the resetOutput() call can be used to reset the output pointer.
   */

  class LinearBufferInputOutputStream : public BufferedInputOutputStream {
    public:

      // constructors

      LinearBufferInputOutputStream(void *buffer,uint32_t size);
      LinearBufferInputOutputStream(uint32_t initialSize);

      void resetOutput();

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;
      virtual bool reset() override;

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;

      virtual bool flush() override {
        return true;
      }

      // overrides from both

      virtual bool close() override {
        return true;
      }
  };
}
