/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __360D69EE_5A80_11E2_9957002481184849
#define __360D69EE_5A80_11E2_9957002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PowerCtrl7Cmd
    {
      enum
      {
        Opcode = 0x29
      };

      inline uint16_t VCOMH(uint16_t n) { return n; }
    }
  }
}


#endif /* POWERCTRL4CMD_H_ */
