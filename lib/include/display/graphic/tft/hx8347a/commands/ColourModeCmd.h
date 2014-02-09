/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace hx8347 {
    namespace ColourModeCmd {
      enum {
        Opcode=0x17,

        COLOURS_64K = 0x55,
        COLOURS_262K = 0x66
      };
    }
  }
}

