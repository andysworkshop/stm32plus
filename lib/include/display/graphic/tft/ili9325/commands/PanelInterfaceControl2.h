/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __34573AE7_E124_432E_8BF7D924B8EA5A60
#define __34573AE7_E124_432E_8BF7D924B8EA5A60

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PanelInterfaceControl2
    {
      enum
      {
        Opcode = 0x92
      };

      inline uint16_t NOWI(uint16_t n) { return n<<8; }
    }
  }
}


#endif
