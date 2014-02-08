/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Read status register command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_READ_STATUS_REGISTER>
    struct ReadStatusRegister : Command<TSpi> {

      /**
       * Provide a readStatusRegister() function for the derived class
       */

      bool readStatusRegister(uint8_t& sr) const {
        return this->readCommand(TOpCode,0,0,0,&sr,1);
      }
    };
  }
}
