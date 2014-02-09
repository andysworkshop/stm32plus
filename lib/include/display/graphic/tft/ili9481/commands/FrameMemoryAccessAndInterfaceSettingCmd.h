/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9481 {
    namespace FrameMemoryAccessAndInterfaceSettingCmd {
      enum {
        Opcode=0xB3,

        WEMODE=1 << 1,
        DFM=1
      };

      inline uint16_t TEI(uint16_t n) {
        return n;
      }
      inline uint16_t DENC(uint16_t n) {
        return n;
      }
      inline uint16_t EPF(uint16_t n) {
        return n << 4;
      }
    }
  }
}
