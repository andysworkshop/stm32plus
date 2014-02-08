/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9327 {
    namespace DisplayTimingSettingNormalModeCmd {
      enum {
        Opcode=0xC1,

        BC=1 << 4,

        DIV0_1=0, DIV0_2=1, DIV0_4=2, DIV0_8=3
      };

      inline uint16_t RTN(uint16_t n) {
        return n;
      }
      inline uint16_t FP(uint16_t n) {
        return n;
      }
      inline uint16_t BP(uint16_t n) {
        return n;
      }
    }
  }
}
