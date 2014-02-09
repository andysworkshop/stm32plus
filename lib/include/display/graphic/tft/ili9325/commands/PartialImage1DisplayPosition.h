/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __D5070117_D11F_4042_8212F157C798714F
#define __D5070117_D11F_4042_8212F157C798714F

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PartialImage1DisplayPosition
    {
      enum
      {
        Opcode = 0x80
      };

      inline uint16_t POSITION(uint16_t n) { return n; }
    }
  }
}


#endif
