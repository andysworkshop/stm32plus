/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Sector erase command implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_SECTOR_ERASE,uint8_t TAddressBytes=3>
    struct SectorErase : Command<TSpi> {

      /**
       * Provide a sectorErase() function for the derived class
       */

      bool sectorErase(uint32_t address) const {
        return this->writeCommand(TOpCode,address,TAddressBytes,0,nullptr,0);
      }
    };
  }
}
