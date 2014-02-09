/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Fast Read command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_FAST_READ,uint8_t TAddressBytes=3,uint8_t TDummyBytes=1>
    struct FastRead : Command<TSpi> {

      /**
       * Provide a fastRead() function for the derived class
       */

      bool fastRead(uint32_t address,void *data,uint32_t dataSize) const {
        return this->readCommand(TOpCode,address,TAddressBytes,TDummyBytes,data,dataSize);
      }
    };
  }
}
