/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __360A2A2C_5A80_11E2_A3F1002481184849
#define __360A2A2C_5A80_11E2_A3F1002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PowerCtrl1Cmd
    {
      enum
      {
        Opcode = 0x10,

        STB  = 1,
        SLP  = 1<<1,
        DSTB = 1<<2,
        DK   = 1<<3
      };
      inline uint16_t AP(uint16_t n) { return n<<4; }
      inline uint16_t BT(uint16_t n) { return n<<8; }
      inline uint16_t SAP(uint16_t n) { return n<<12; }
    }
  }
}


#endif
