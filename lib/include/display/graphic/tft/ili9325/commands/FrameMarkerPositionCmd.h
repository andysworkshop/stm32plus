/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __82E7E338_148C_4940_BB5163370C747CB9
#define __82E7E338_148C_4940_BB5163370C747CB9

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace FrameMarkerPositionCmd
    {
      enum
      {
        Opcode = 0xd
      };

      inline uint16_t FRAME_MARKER_POSITION(uint16_t n) { return n; }
    }
  }
}


#endif
