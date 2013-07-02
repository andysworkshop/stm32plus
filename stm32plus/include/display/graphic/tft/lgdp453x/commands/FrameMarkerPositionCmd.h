/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35F6CC52_5A80_11E2_87C5002481184849
#define __35F6CC52_5A80_11E2_87C5002481184849

#include <stdint.h>


namespace stm32plus
{
  namespace lgdp453x
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
