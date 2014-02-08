/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace hx8347 {
    namespace MemoryAccessCtrlCmd {
      enum {
        Opcode=0x16
      };

      static const uint16_t MY=1 << 7;
      static const uint16_t MX=1 << 6;
      static const uint16_t MV=1 << 5;
      static const uint16_t ML=1 << 4;
      static const uint16_t BGR=1 << 3;
    }
  }
}
