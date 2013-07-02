/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __3603A53A_5A80_11E2_9F4D002481184849
#define __3603A53A_5A80_11E2_9F4D002481184849


namespace stm32plus
{
  namespace lgdp453x
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
