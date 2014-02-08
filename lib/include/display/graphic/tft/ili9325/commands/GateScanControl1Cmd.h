/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __E7EA5C4D_5642_44EF_822EA8A62E8047FB
#define __E7EA5C4D_5642_44EF_822EA8A62E8047FB

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace GateScanControl1Cmd
    {
      enum
      {
        Opcode = 0x60,

        GS = 1<<15
      };

      inline uint16_t SCAN_GATE_LINE(uint16_t n) { return n; }
      inline uint16_t NUM_LINES(uint16_t n) { return n<<8; }
    }
  }
}


#endif
