/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __36192658_5A80_11E2_A767002481184849
#define __36192658_5A80_11E2_A767002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace HorizontalAddressCmd
    {
      enum
      {
        Opcode = 0x20
      };

      inline uint16_t ADDR(uint16_t n) { return n; }
    }
  }
}


#endif /* HORIZONTALADDRESSCMD_H_ */
