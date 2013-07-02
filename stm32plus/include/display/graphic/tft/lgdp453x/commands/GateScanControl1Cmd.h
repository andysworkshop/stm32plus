/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35E60ACA_5A80_11E2_A32E002481184849
#define __35E60ACA_5A80_11E2_A32E002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace GateScanControl1Cmd
    {
      enum
      {
        Opcode = 0x60,

        GS = 1<<15
      };

      inline uint16_t SCN(uint16_t n) { return n; }
      inline uint16_t NL(uint16_t n) { return n<<8; }
    }
  }
}


#endif
