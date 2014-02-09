/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace hx8347 {
    namespace CycleControlCmd {
      enum {
        Opcode1=0x23, Opcode2=0x24, Opcode3=0x25,

        Opcode4=0x3a, Opcode5=0x3b, Opcode6=0x3c, Opcode7=0x3d,

        CycleControl5Opcode=0x3e, CycleControl6Opcode=0x40
      };

      inline uint16_t N_RTN(uint16_t value_) {
        return value_ << 4;
      }
      inline uint16_t PI_R(uint16_t value_) {
        return value_ << 4;
      }
      inline uint16_t I_RTN(uint16_t value_) {
        return value_ << 4;
      }
    }
  }
}
