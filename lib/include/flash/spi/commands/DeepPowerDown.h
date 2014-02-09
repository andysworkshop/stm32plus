/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Deep power down command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_DEEP_POWER_DOWN>
    struct DeepPowerDown : Command<TSpi> {

      /**
       * Provide a deepPowerDown() function for the derived class
       */

      bool deepPowerDown() const {
        return this->writeCommand(TOpCode);
      }
    };
  }
}
