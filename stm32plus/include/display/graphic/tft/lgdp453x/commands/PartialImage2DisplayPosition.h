/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35D56954_5A80_11E2_ABB5002481184849
#define __35D56954_5A80_11E2_ABB5002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PartialImage2DisplayPosition
    {
      enum
      {
        Opcode = 0x83
      };

      inline uint16_t POSITION(uint16_t n) { return n; }
    }
  }
}


#endif
