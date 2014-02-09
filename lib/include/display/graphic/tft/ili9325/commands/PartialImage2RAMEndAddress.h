/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __A558A544_31BC_4C9E_87FF1644A0B69B13
#define __A558A544_31BC_4C9E_87FF1644A0B69B13

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PartialImage2RAMEndAddress
    {
      enum
      {
        Opcode = 0x85
      };

      inline uint16_t ENDADDRESS(uint16_t n) { return n; }
    }
  }
}


#endif
