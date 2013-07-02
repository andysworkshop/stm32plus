/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __4C8F4BA5_B21B_4018_9EDD7AFDA3D55773
#define __4C8F4BA5_B21B_4018_9EDD7AFDA3D55773


namespace stm32plus
{
  namespace ili9481
  {
    namespace DisplayTimingSettingIdleModeCmd
    {
      enum
      {
        Opcode = 0xC3,

        BC2 = 1<<4,

        DIV2_1 = 0,
        DIV2_2 = 1,
        DIV2_4 = 2,
        DIV2_8 = 3
      };

      inline uint16_t RTN2(uint16_t n) { return n; }
      inline uint16_t FP2(uint16_t n) { return n<<4; }
      inline uint16_t BP2(uint16_t n) { return n; }
    }
  }
}


#endif
