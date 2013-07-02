/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

namespace stm32plus {
  namespace lgdp453x {
    namespace DisplayCtrl1Cmd {
      enum {
        Opcode=7,

        COL   = 1<<3,
        DTE   = 1<<4,
        GON   = 1<<5,
        BASEE = 1<<8

//        DISPLAY_HALT=0,
//        DISPLAY_ON_INTERNAL_ONLY=1,
//        DISPLAY_ON=3,
//
//        CL_8BIT=1 << 3,
//
//        GATE_DRIVER_VGH=0,
//        GATE_DRIVER_VGL=1 << 5,
//        GATE_DRIVER_NORMAL=3 << 4,
//
//        BASE_IMAGE=1 << 8,
//
//        PARTIAL_IMAGE=3 << 12
      };
      inline uint16_t D(uint16_t n){return n;}
      inline uint16_t PTDE(uint16_t n){return n<<12;}
    }
  }
}

