/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __01E6D5DF_8FBD_42C8_B84DEB9DA106D9AB
#define __01E6D5DF_8FBD_42C8_B84DEB9DA106D9AB

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PartialImage1RAMEndAddress
    {
      enum
      {
        Opcode = 0x82
      };

      inline uint16_t ENDADDRESS(uint16_t n) { return n; }
    }
  }
}


#endif
