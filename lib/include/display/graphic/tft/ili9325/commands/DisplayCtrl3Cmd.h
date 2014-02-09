/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __619B90FE_F253_4CB6_9DCAC808AA7D7125
#define __619B90FE_F253_4CB6_9DCAC808AA7D7125

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace DisplayCtrl3Cmd
    {
      enum
      {
        Opcode = 9,

        NON_DISPLAY_SCAN_MODE_NORMAL = 0,
        NON_DISPLAY_SCAN_MODE_INTERVAL = 2<<4,

        NON_DISPLAY_SOURCE_OUT_V63_V0_REG = 0,
        NON_DISPLAY_SOURCE_OUT_GND_GND_REG = 2<<8,
        NON_DISPLAY_SOURCE_OUT_HIZ_HIZ_REG = 3<<8,
        NON_DISPLAY_SOURCE_OUT_V63_V0_FREQ = 4<<8,
        NON_DISPLAY_SOURCE_OUT_GND_GND_FREQ = 6<<8,
        NON_DISPLAY_SOURCE_OUT_HIZ_HIZ_FREQ = 7<<8
      };

      inline uint16_t NON_DISPLAY_SCAN_CYCLE_CODE(uint16_t n) { return n; }
    }
  }
}

#endif /* DISPLAYCTRL3CMD_H_ */
