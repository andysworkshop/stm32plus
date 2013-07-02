/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35E9C3FE_5A80_11E2_8876002481184849
#define __35E9C3FE_5A80_11E2_8876002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PartialImage1DisplayPosition
    {
      enum
      {
        Opcode = 0x80
      };

      inline uint16_t POSITION(uint16_t n) { return n; }
    }
  }
}


#endif
