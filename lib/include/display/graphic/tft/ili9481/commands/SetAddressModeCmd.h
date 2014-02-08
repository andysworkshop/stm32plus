/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __57DDC7CF_D413_4D62_81075B60E7FAD24C
#define __57DDC7CF_D413_4D62_81075B60E7FAD24C


namespace stm32plus
{
  namespace ili9481
  {
    namespace SetAddressModeCmd
    {
      enum
      {
        Opcode = 0x36,

        VERTICAL_FLIP = 1,
        HORIZONTAL_FLIP = 1<<1,
        BGR = 1<<3,
        VERTICAL_ORDER = 1<<4,
        PAGECOL_SELECTION = 1<<5,
        COLUMN_ADDRESS_ORDER = 1<<6,
        PAGE_ADDRESS_ORDER = 1<<7
      };
    }
  }
}


#endif
