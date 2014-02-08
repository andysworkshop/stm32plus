/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9327 {
    namespace SetScrollStartCmd {
      enum {
        Opcode=0x37

      };

      inline uint16_t VSP(uint16_t n) {
        return n;
      }
    }
  }
}
