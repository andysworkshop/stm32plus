/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of an output stream for the SPI peripheral.
   * Writing to this stream causes data to be transmitted over SPI.
   */

  class SpiPollingOutputStream : public OutputStream {

    protected:
      Spi& _spi;

    public:
      SpiPollingOutputStream(Spi& usart);
      virtual ~SpiPollingOutputStream() {}

      // overrides from OutputStream

      virtual bool write(uint8_t c) override;
      virtual bool write(const void *buffer,uint32_t size) override;

      /**
       * Always true.
       * @return always true
       */

      virtual bool flush() override {
        return true;
      }

      /**
       * Always true.
       * @return always true
       */

      virtual bool close() override {
        return true;
      }
  };
}
