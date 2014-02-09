/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __B06A96CA_4EE7_41E6_BE71D45663B22FBE
#define __B06A96CA_4EE7_41E6_BE71D45663B22FBE


namespace stm32plus
{
  namespace ili9481
  {
    namespace InterfaceControlCmd
    {
      enum
      {
        Opcode = 0xc6,

        DPL = 1,
        EPL = 1<<1,
        HSPL = 1 << 3,
        VSPL = 1 << 4,
        SDA_EN = 1 << 7
      };
    }
  }
}


#endif
