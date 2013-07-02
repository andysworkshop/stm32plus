/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __36054502_5A80_11E2_A2B5002481184849
#define __36054502_5A80_11E2_A2B5002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace GateScanControlScrollCmd
    {
      enum
      {
        Opcode = 0x6A
      };

      inline uint16_t SCROLL(uint16_t n) { return n; }
    }
  }
}


#endif
