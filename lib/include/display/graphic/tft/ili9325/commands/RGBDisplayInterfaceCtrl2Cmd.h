/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __5D8C0476_D9A4_4CE7_A31284931A4578BA
#define __5D8C0476_D9A4_4CE7_A31284931A4578BA


namespace stm32plus
{
  namespace ili9325
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
