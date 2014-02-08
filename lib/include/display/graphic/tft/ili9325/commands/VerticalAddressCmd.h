/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __EC2B8DF7_DAA1_4C13_857C4B71FB2C7948
#define __EC2B8DF7_DAA1_4C13_857C4B71FB2C7948

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace VerticalAddressCmd
    {
      enum
      {
        Opcode = 0x21
      };

      inline uint16_t ADDR(uint16_t n) { return n; }
    }
  }
}


#endif /* VERTICALADDRESSCMD_H_ */
