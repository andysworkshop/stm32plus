/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {
    namespace w25q16dw {


      /**
       * Erase suspend command implementation
       */

      template<class TSpi>
      struct EraseResume : Command<TSpi> {

        /**
         * Provide a eraseSuspend() function for the derived class
         */

        bool eraseResume() const {
          return this->writeCommand(CMD_ERASE_RESUME);
        }
      };
    }
  }
}
