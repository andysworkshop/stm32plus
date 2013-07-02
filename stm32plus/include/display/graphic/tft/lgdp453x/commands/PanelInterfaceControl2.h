/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35D3CA2C_5A80_11E2_ABD1002481184849
#define __35D3CA2C_5A80_11E2_ABD1002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
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
