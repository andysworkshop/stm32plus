/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace hx8347 {
    namespace DisplayModeCmd {
      enum {
        Opcode=0x1
      };

      static const uint16_t IDMON=1 << 3;
      static const uint16_t INVON=1 << 2;
      static const uint16_t NORON=1 << 1;
      static const uint16_t PTLON=1;
    }
  }
}
