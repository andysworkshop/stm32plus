/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __86F34465_A14D_48E5_AEB2568FB17BC8CE
#define __86F34465_A14D_48E5_AEB2568FB17BC8CE

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PowerCtrl3Cmd
    {
      enum
      {
        Opcode = 0x12,

        PON = 1<<4,
        VCIRE = 1<<7
      };

      inline uint16_t VRH(uint16_t n) { return n; }
    }
  }
}


#endif /* POWERCTRL3CMD_H_ */
