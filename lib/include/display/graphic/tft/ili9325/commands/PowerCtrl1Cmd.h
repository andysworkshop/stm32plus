/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __28182CA5_979A_4E9B_86BD843F996FDF9C
#define __28182CA5_979A_4E9B_86BD843F996FDF9C

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PowerCtrl1Cmd
    {
      enum
      {
        Opcode = 0x10,

        STANDBY = 1,
        SLEEP = 1<<1,
        DEEP_STANDBY = 1<<2,

        AP_HALT = 0,
        AP_1_1 = 1<<4,
        AP_1_075 = 2<<4,
        AP_1_050 = 3<<4,
        AP_075_1 = 4<<4,
        AP_075_075 = 5<<4,
        AP_075_050 = 6<<4,
        AP_050_050 = 7<<4,

        APE = 1<<7,

        SOURCE_DRIVER_ENABLE = 1<<12
      };

      inline uint16_t STEP_UP_FACTOR(uint16_t n) { return n<<8; }
    }
  }
}


#endif
