/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9325 {
    namespace DisplayCtrl1Cmd {
      enum {
        Opcode=7,

        DISPLAY_HALT=0, DISPLAY_ON_INTERNAL_ONLY=1, DISPLAY_ON=3,

        CL_8BIT=1 << 3,

        GATE_DRIVER_VGH=0, GATE_DRIVER_VGL=1 << 5, GATE_DRIVER_NORMAL=3 << 4,

        BASE_IMAGE=1 << 8,

        PARTIAL_IMAGE=3 << 12
      };
    }
  }
}

