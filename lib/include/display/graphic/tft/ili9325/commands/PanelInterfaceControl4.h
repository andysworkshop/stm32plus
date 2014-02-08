/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __AC83A5D4_93CE_4F93_861DFCDA4DC75D13
#define __AC83A5D4_93CE_4F93_861DFCDA4DC75D13

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PanelInterfaceControl4
    {
      enum
      {
        Opcode = 0x95
      };

      inline uint16_t RTNE(uint16_t n) { return n; }
      inline uint16_t DIVE(uint16_t n) { return n<<8; }
    }
  }
}


#endif
