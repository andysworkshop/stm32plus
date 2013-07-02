/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35DA5DE2_5A80_11E2_A5F8002481184849
#define __35DA5DE2_5A80_11E2_A5F8002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace RGBDisplayInterfaceCtrl2Cmd
    {
      enum
      {
        Opcode = 0xf,

        DPL_RISING = 0,
        DPL_FALLING = 1,

        EPL_LOW = 0,
        EPL_HIGH = 1<<1,

        HSPL_LOW = 0,
        HSPL_HIGH = 1<<3,

        VSPL_LOW = 0,
        VSPL_HIGH = 1<<4
      };
    }
  }
}


#endif
