/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35F17F7C_5A80_11E2_ACCE002481184849
#define __35F17F7C_5A80_11E2_ACCE002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PowerCtrl3Cmd
    {
      enum
      {
        Opcode = 0x12,

        PON = 1<<4
      };
      inline uint16_t VRH(uint16_t n) { return n; }
    }
  }
}


#endif /* POWERCTRL3CMD_H_ */
