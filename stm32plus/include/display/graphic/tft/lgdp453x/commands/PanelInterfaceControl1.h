/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35DBFFE4_5A80_11E2_AC93002481184849
#define __35DBFFE4_5A80_11E2_AC93002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PanelInterfaceControl1
    {
      enum
      {
        Opcode = 0x90
      };

      inline uint16_t RTNI(uint16_t n) { return n<<1; }
      inline uint16_t DIVI(uint16_t n) { return n<<8; }
    }
  }
}


#endif
