/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __6D4EA5DB_69A2_4D91_9B4A28A38C273415
#define __6D4EA5DB_69A2_4D91_9B4A28A38C273415

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace HorizontalAddressCmd
    {
      enum
      {
        Opcode = 0x20
      };

      inline uint16_t ADDR(uint16_t n) { return n; }
    }
  }
}


#endif /* HORIZONTALADDRESSCMD_H_ */
