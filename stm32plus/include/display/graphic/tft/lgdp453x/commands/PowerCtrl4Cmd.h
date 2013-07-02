/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __3615D854_5A80_11E2_AA58002481184849
#define __3615D854_5A80_11E2_AA58002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PowerCtrl4Cmd
    {
      enum
      {
        Opcode = 0x13,

        VCOMG  = 1<<13
      };
      inline uint16_t VCM(uint16_t n) { return n; }
      inline uint16_t VDV(uint16_t n) { return n<<8; }
    }
  }
}


#endif /* POWERCTRL4CMD_H_ */
