/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace spiflash {
    namespace s25fl208k {


      /**
       * Commands for the S25FL208K that are either different to, or extend
       * the standard set.
       */

      enum {
        CMD_BLOCK_ERASE = 0xd8        // different to the usual command id
      };
    }
  }
}
