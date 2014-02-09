/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {

  /**
   * @brief Implementation of an output stream for the serial USART.
   * Writing to this stream causes data to be transmitted to the USART.
   */

  class UsartPollingOutputStream : public OutputStream {

    protected:
      Usart& _usart;

    public:
      UsartPollingOutputStream(Usart& usart);
      virtual ~UsartPollingOutputStream() {}

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
