/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of an input stream for the SPI peripheral.
   * The stream methods can be used to read data from the SPI in polling mode.
   */

  class SpiPollingInputStream : public InputStream {

    protected:
      Spi& _spi;

    public:
      SpiPollingInputStream(Spi& usart);
      virtual ~SpiPollingInputStream() {}

      // overrides from InputStream

      virtual int16_t read() override;
      virtual bool read(void *buffer,uint32_t size,uint32_t& actuallyRead) override;
      virtual bool skip(uint32_t howMuch) override;
      virtual bool available() override;

      /**
       * Doesn't do anything.
       * @return always true
       */

      virtual bool close() override {
        return true;
      }

      /**
       * Not supported.
       * @return always false and E_OPERATION_NOT_SUPPORTED
       */

      virtual bool reset() override {
        return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SPI_INPUT_STREAM,E_OPERATION_NOT_SUPPORTED);
      }
  };
}
