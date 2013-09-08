/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35D8BFA0_5A80_11E2_B76A002481184849
#define __35D8BFA0_5A80_11E2_B76A002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PanelInterfaceControl4
    {
      enum
      {
        Opcode = 0x95
      };

      inline uint16_t RTNE(uint16_t n) { return n; }
      inline uint16_t DIVE(uint16_t n) { return n<<8; }
    }
  }
}


#endif
