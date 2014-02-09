/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Block erase command implementation. Sometimes this is described as a sector erase and
     * generally it's the command that erases the largest 'unit' of sectors on the device in
     * a single call.
     */

    template<class TSpi,uint8_t TOpCode=CMD_BLOCK_ERASE,uint8_t TAddressBytes=3>
    struct BlockErase : Command<TSpi> {

      /**
       * Provide a blockErase() function for the derived class
       */

      bool blockErase(uint32_t address) const {
        return this->writeCommand(TOpCode,address,TAddressBytes,0,nullptr,0);
      }
    };
  }
}
