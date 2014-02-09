/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Write disable command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_WRITE_DISABLE>
    struct WriteDisable : Command<TSpi> {

      /**
       * Provide a writeDisable() function for the derived class
       */

      bool writeDisable() const {
        return this->writeCommand(TOpCode);
      }
    };
  }
}
