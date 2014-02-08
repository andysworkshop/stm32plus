/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __DE56515A_2BA3_4E82_86808AA1BA8944B1
#define __DE56515A_2BA3_4E82_86808AA1BA8944B1

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
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
