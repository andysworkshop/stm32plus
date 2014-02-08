/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9327 {
    namespace FrameRateAndInversionControlCmd {
      enum {
        Opcode=0xC5,

        FR_96=0, FR_88=1, FR_82=2, FR_76=3, FR_72=4, FR_67=5, FR_64=6, FR_60=7
      };
    }
  }
}
