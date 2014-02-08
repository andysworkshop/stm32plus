/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace hx8347 {
    namespace GateScanControlCmd {
      enum {
        Opcode=0x18,

        SCROLL_ON=1 << 1, SM=1
      };
    }
  }
}
