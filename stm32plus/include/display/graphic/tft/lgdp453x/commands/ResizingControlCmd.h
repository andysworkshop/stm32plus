/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __3617789E_5A80_11E2_B4B7002481184849
#define __3617789E_5A80_11E2_B4B7002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace ResizingControlCmd
    {
      enum
      {
        Opcode = 4,

        NO_RESIZING = 0,

        FACTOR_HALF = 1,
        FACTOR_QUARTER = 3,

        HORIZONTAL_REMAINDER_1 = 1<<4,
        HORIZONTAL_REMAINDER_2 = 2<<4,
        HORIZONTAL_REMAINDER_3 = 3<<4,

        VERTICAL_REMAINDER_1 = 1<<8,
        VERTICAL_REMAINDER_2 = 2<<8,
        VERTICAL_REMAINDER_3 = 3<<8
      };
    }
  }
}


#endif
