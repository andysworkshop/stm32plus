/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9327 {
    namespace SetAddressModeCmd {
      enum {
        Opcode=0x36,

        VERTICAL_FLIP=1,
        HORIZONTAL_FLIP=1 << 1,
        BGR=1 << 3,
        VERTICAL_ORDER=1 << 4,
        PAGECOL_SELECTION=1 << 5,
        COLUMN_ADDRESS_ORDER=1 << 6,
        PAGE_ADDRESS_ORDER=1 << 7
      };
    }
  }
}
