/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35E0DCE4_5A80_11E2_B716002481184849
#define __35E0DCE4_5A80_11E2_B716002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
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
