/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


namespace stm32plus
{
  namespace ili9481
  {
    namespace SetPixelFormatCmd
    {
      enum
      {
        Opcode = 0x3A,

        COLOURS_8 = 0x11,
        COLOURS_64K = 0x55,
        COLOURS_262K = 0x66
      };
    }
  }
}
