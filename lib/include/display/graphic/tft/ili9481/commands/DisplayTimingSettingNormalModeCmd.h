/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __9E4F960A_6817_4C08_8D3BE4641389EFFE
#define __9E4F960A_6817_4C08_8D3BE4641389EFFE


namespace stm32plus
{
  namespace ili9481
  {
    namespace DisplayTimingSettingNormalModeCmd
    {
      enum
      {
        Opcode = 0xC1,

        BC = 1<<4,

        DIV0_1 = 0,
        DIV0_2 = 1,
        DIV0_4 = 2,
        DIV0_8 = 3
      };

      inline uint16_t RTN(uint16_t n) { return n; }
      inline uint16_t FP(uint16_t n) { return n<<4; }
      inline uint16_t BP(uint16_t n) { return n; }
    }
  }
}


#endif
