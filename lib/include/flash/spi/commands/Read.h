/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Read command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_READ,uint8_t TAddressBytes=3>
    struct Read : Command<TSpi> {

      /**
       * Provide a read() function for the derived class
       */

      bool read(uint32_t address,void *data,uint32_t dataSize) const {
        return this->readCommand(TOpCode,address,TAddressBytes,0,data,dataSize);
      }
    };
  }
}
