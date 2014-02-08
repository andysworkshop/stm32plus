/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus {
  namespace hx8347 {
    namespace DisplayControlCmd {
      enum {
        OpcodeNBP=0x27,
        OpcodeNFP=0x28,
        OpcodePIB=0x29,
        OpcodePIF=0x2a,
        OpcodeIBP=0x2c,
        OpcodeIFP=0x2d,

        Opcode1=0x26,
        Opcode8=0x90,
        Opcode9=0x35,
        Opcode10=0x36,
        Opcode14=0x41
      };
    }
  }
}
