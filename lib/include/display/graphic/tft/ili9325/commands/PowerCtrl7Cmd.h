/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __6D9FE80E_E069_4F33_BA45AF7D9E6E7D43
#define __6D9FE80E_E069_4F33_BA45AF7D9E6E7D43

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
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
