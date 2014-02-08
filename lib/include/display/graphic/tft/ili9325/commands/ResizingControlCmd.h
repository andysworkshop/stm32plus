/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __C69C2D13_81D5_4CDA_9E526AACBF24F3B9
#define __C69C2D13_81D5_4CDA_9E526AACBF24F3B9


namespace stm32plus
{
  namespace ili9325
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
