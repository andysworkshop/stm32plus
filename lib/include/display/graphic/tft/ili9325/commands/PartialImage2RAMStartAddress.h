/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __92B43290_3AAB_45B0_AE8E7E9004C11B08
#define __92B43290_3AAB_45B0_AE8E7E9004C11B08

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PartialImage2RAMStartAddress
    {
      enum
      {
        Opcode = 0x84
      };

      inline uint16_t STARTADDRESS(uint16_t n) { return n; }
    }
  }
}


#endif
