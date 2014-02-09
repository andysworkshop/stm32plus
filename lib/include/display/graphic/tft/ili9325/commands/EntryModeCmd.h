/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __912F3BDC_13EB_4BD5_80E7BB0A35624E9A
#define __912F3BDC_13EB_4BD5_80E7BB0A35624E9A


namespace stm32plus
{
  namespace ili9325
  {
    namespace EntryModeCmd
    {
      enum
      {
        Opcode = 3,

        AM_HORIZONTAL = 0,
        AM_VERTICAL = 1<<3,

        ID_HDEC_VDEC = 0,
        ID_HINC_VDEC = 1<<4,
        ID_HDEC_VINC = 2<<4,
        ID_HINC_VINC = 3<<4,

        ORG = 1<<7,
        HWM = 1<<9,
        BGR = 1<<12,
        DFM = 1<<14,
        TRI = 1<<15
      };
    }
  }
}


#endif /* STARTOSCILLATIONCOMMAND_H_ */
