/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __D38DC809_3069_42DA_BBC7E758D601E30A
#define __D38DC809_3069_42DA_BBC7E758D601E30A


namespace stm32plus
{
  namespace ili9481
  {
    namespace PowerSettingCmd
    {
      enum
      {
        Opcode = 0xD0,

        VC_095 = 0,
        VC_090 = 1,
        VC_085 = 2,
        VC_080 = 3,
        VC_075 = 4,
        VC_070 = 5,
        VC_DISABLE = 6,
        VC_100 = 7,

        BT_6_5 = 0,
        BT_6_4 = 1,
        BT_6_3 = 2,
        BT_5_5 = 3,
        BT_5_4 = 4,
        BT_5_3 = 5,
        BT_4_4 = 6,
        BT_4_3 = 7,

        PON = 1<<6,

        VCIRE = 1<<4
      };

      inline uint16_t VRH(uint16_t n) { return n; }
    }
  }
}


#endif /* GRAMSTARTWRITINGCMD_H_ */
