/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {


    /**
     * Write status and write status/configuration implementation
     */

    template<class TSpi,uint8_t TOpCode=CMD_WRITE_REGISTERS>
    struct WriteStatusRegister : Command<TSpi> {

      /**
       * Provide writeStatusRegister()
       */

      bool writeStatusRegister(uint8_t status) const {
        return this->writeCommand(TOpCode,0,0,0,&status,1);
      }


      /**
       * Provide writeStatusAndConfigurationRegisters()
       */

      bool writeStatusAndConfigurationRegisters(uint8_t status,uint8_t configuration) const {

        uint8_t data[2];

        data[0]=status;
        data[1]=configuration;

        return this->writeCommand(TOpCode,0,0,0,data,2);
      }
    };
  }
}
