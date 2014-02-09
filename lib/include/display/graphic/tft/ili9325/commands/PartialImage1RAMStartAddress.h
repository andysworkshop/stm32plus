/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __7340B2F2_5F0E_4D21_A11FEC357E30E897
#define __7340B2F2_5F0E_4D21_A11FEC357E30E897

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PartialImage1RAMStartAddress
    {
      enum
      {
        Opcode = 0x81
      };

      inline uint16_t STARTADDRESS(uint16_t n) { return n; }
    }
  }
}


#endif
