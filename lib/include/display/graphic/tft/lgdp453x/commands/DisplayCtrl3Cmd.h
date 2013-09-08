/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __361FAF14_5A80_11E2_B5EA002481184849
#define __361FAF14_5A80_11E2_B5EA002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace DisplayCtrl3Cmd
    {
      enum
      {
        Opcode = 0x09
      };

      inline uint16_t ISC(uint16_t n) { return n; }
      inline uint16_t PTG(uint16_t n) { return n<<4; }
      inline uint16_t PTS(uint16_t n) { return n<<8; }
    }
  }
}

#endif /* DISPLAYCTRL3CMD_H_ */
