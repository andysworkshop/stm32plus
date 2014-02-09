/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __5B764C8D_62C0_4B30_A94E4D6EEFE89B79
#define __5B764C8D_62C0_4B30_A94E4D6EEFE89B79

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace GateScanControlScrollCmd
    {
      enum
      {
        Opcode = 0x6A
      };

      inline uint16_t SCROLL(uint16_t n) { return n; }
    }
  }
}


#endif
