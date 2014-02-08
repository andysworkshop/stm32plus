/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __279C787B_CD32_4C4C_8ABC1FFC4B57CD4A
#define __279C787B_CD32_4C4C_8ABC1FFC4B57CD4A


namespace stm32plus
{
  namespace ili9325
  {
    namespace PowerCtrl2Cmd
    {
      enum
      {
        Opcode = 0x11,

        VC_095 = 0,
        VC_090 = 1,
        VC_085 = 2,
        VC_080 = 3,
        VC_075 = 4,
        VC_070 = 5,
        VC_100 = 7,

        STEPUP_FREQ1_1 = 0,
        STEPUP_FREQ1_2 = 1<<4,
        STEPUP_FREQ1_4 = 2<<4,
        STEPUP_FREQ1_8 = 3<<4,
        STEPUP_FREQ1_16 = 4<<4,
        STEPUP_FREQ1_32 = 5<<4,
        STEPUP_FREQ1_64 = 6<<4,
        STEPUP_FREQ1_HALT = 7<<4,

        STEPUP_FREQ2_4 = 0,
        STEPUP_FREQ2_8 = 1<<8,
        STEPUP_FREQ2_16 = 2<<8,
        STEPUP_FREQ2_32 = 3<<8,
        STEPUP_FREQ2_64 = 4<<8,
        STEPUP_FREQ2_128 = 5<<8,
        STEPUP_FREQ2_256 = 6<<8,
        STEPUP_FREQ2_HALT = 7<<8
      };
    }
  }
}


#endif /* POWERCTRL2CMD_H_ */
