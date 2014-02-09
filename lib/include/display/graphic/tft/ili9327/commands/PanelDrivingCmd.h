/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace ili9327 {
    namespace PanelDrivingCmd {
      enum {
        Opcode=0xC0, SS=1, BGR=1 << 1,

        REV=1 << 4, SM=1 << 3, GS=1 << 2,

        PTG=1 << 4
      };

      inline uint16_t NL(uint16_t n) {
        return n;
      }
      inline uint16_t SCN(uint16_t n) {
        return n;
      }
      inline uint16_t PTS(uint16_t n) {
        return n;
      }
      inline uint16_t ISC(uint16_t n) {
        return n;
      }
      inline uint16_t DIVE(uint16_t n) {
        return n;
      }
    }
  }
}
