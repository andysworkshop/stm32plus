/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __8587A6A1_1802_4F1A_AE140E097C568635
#define __8587A6A1_1802_4F1A_AE140E097C568635

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PowerCtrl4Cmd
    {
      enum
      {
        Opcode = 0x13
      };

      inline uint16_t VDV(uint16_t n) { return n<<8; }
    }
  }
}


#endif /* POWERCTRL4CMD_H_ */
