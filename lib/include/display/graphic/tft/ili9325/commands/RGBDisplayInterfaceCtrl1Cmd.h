/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __AA8E19FE_56AA_4496_869C882AF31D661C
#define __AA8E19FE_56AA_4496_869C882AF31D661C


namespace stm32plus
{
  namespace ili9325
  {
    namespace RGBDisplayInterfaceCtrl1Cmd
    {
      enum
      {
        Opcode = 0xc,

        RGB18 = 0,
        RGB16 = 1,
        RGB6 = 2,

        MODE_INTERNAL_CLOCK = 0,
        MODE_RGB_INTERFACE = 1<<4,
        MODE_VSYNC_INTERFACE = 2<<5,

        RGB_GRAM_INTERFACE = 1<<8,

        GRAM_RGB_WRITE_CYCLE_1 = 0,
        GRAM_RGB_WRITE_CYCLE_2 = 1<<12,
        GRAM_RGB_WRITE_CYCLE_3 = 2<<12,
        GRAM_RGB_WRITE_CYCLE_4 = 3<<12
      };
    }
  }
}


#endif
