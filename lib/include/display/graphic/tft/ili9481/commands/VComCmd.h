/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __C65AA765_93BF_4A34_BC3A98BBAE03A442
#define __C65AA765_93BF_4A34_BC3A98BBAE03A442


namespace stm32plus
{
  namespace ili9481
  {
    namespace VComCmd
    {
      enum
      {
        Opcode = 0xD1,

        SELVCM = 1
      };

      inline uint16_t VCM(uint16_t n) { return n; }
      inline uint16_t VDV(uint16_t n) { return n; }
    }
  }
}


#endif
