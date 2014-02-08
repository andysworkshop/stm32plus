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
       * Commands specific to the Winbond w25q16dw
       */

      enum {
        CMD_ERASE_SUSPEND = 0x75,//!< CMD_ERASE_SUSPEND
        CMD_ERASE_RESUME = 0x7a  //!< CMD_ERASE_RESUME
      };
    }
  }
}
