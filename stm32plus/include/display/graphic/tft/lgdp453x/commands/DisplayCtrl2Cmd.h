/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __36249362_5A80_11E2_A7B3002481184849
#define __36249362_5A80_11E2_A7B3002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace DisplayCtrl2Cmd
    {
      enum
      {
        Opcode = 0x08
      };

      inline uint16_t BP(uint16_t n) { return n; }
      inline uint16_t FP(uint16_t n) { return n<<8; }
    }
  }
}


#endif
