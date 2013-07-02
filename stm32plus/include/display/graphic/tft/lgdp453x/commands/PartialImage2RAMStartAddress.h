/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35F4292A_5A80_11E2_A16C002481184849
#define __35F4292A_5A80_11E2_A16C002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PartialImage2RAMStartAddress
    {
      enum
      {
        Opcode = 0x84
      };

      inline uint16_t STARTADDRESS(uint16_t n) { return n; }
    }
  }
}


#endif
