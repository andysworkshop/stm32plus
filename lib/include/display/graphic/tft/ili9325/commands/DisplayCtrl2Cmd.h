/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __7A786737_93F3_48A0_86A2059575AC142D
#define __7A786737_93F3_48A0_86A2059575AC142D

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace DisplayCtrl2Cmd
    {
      enum
      {
        Opcode = 8
      };

      inline uint16_t BACK_PORCH(uint16_t n) { return n; }
      inline uint16_t FRONT_PORCH(uint16_t n) { return n<<8; }
    }
  }
}


#endif
