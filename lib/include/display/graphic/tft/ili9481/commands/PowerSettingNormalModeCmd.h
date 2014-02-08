/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __348D0DB8_24D0_4BEE_BCCF0C46E3018672
#define __348D0DB8_24D0_4BEE_BCCF0C46E3018672


namespace stm32plus
{
  namespace ili9481
  {
    namespace PowerSettingNormalModeCmd
    {
      enum
      {
        Opcode = 0xD2
      };

      inline uint16_t AP(uint16_t n) { return n; }
      inline uint16_t DC(uint16_t n) { return n; }
      inline uint16_t DC10(uint16_t n) { return n<<4; }
    }
  }
}


#endif
