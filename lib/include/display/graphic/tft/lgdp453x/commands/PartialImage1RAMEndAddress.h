/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __36016374_5A80_11E2_987C002481184849
#define __36016374_5A80_11E2_987C002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PartialImage1RAMEndAddress
    {
      enum
      {
        Opcode = 0x82
      };

      inline uint16_t ENDADDRESS(uint16_t n) { return n; }
    }
  }
}


#endif
