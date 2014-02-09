/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Read JEDEC id command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_READ_JEDEC_ID>
    struct ReadJedecId : Command<TSpi> {

      /**
       * Provide a readJedecId() function for the derived class
       */

      bool readJedecId(uint8_t *data,uint32_t dataSize) const {
        return this->readCommand(TOpCode,0,0,0,data,dataSize);
      }
    };
  }
}
