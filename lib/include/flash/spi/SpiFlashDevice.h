/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * This template contains generic operations
     */

    template<class TImpl>
    class SpiFlashDevice {

      public:

        /**
         * Error codes
         */

        enum {
          E_TIMED_OUT = 1     //!< Timed out while performing an operation
        };

      public:
        bool waitForIdle(uint32_t timeoutMillis=0) const;
    };


    /**
     * Wait for the status register to clear the BUSY bit
     * @param timeoutMillis Maximum time to wait, or zero to wait forever
     * @return true if it worked
     */

    template<class TImpl>
    inline bool SpiFlashDevice<TImpl>::waitForIdle(uint32_t timeoutMillis) const {

      uint8_t sr;
      uint32_t start;

      if(timeoutMillis)
        start=MillisecondTimer::millis();

      for(;;) {

        if(!static_cast<const TImpl *>(this)->readStatusRegister(sr))
          return false;

        if((sr & TImpl::STATUS_BUSY_BIT_MASK)==0)
          return true;

        if(timeoutMillis && MillisecondTimer::hasTimedOut(start,timeoutMillis))
          return errorProvider.set(ErrorProvider::ERROR_PROVIDER_SPI_FLASH,E_TIMED_OUT);
      }
    }
  }
}

