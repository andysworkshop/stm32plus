/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {

    /**
     * Base template for SPI commands. TSpi is a template parameter instead of an instance
     * of "Spi" so that a wrapper can be used to emulate SPI over GPIO or defer to a DMA
     * channel that's connected to the SPI peripheral.
     */

    template<class TSpi>
    class Base {

      protected:
        TSpi *_spi;     // not an owned pointer and not a reference to avoid ugly virtual inheritance ctor requirements
    };
  }
}
