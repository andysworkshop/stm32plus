/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35EED826_5A80_11E2_8C72002481184849
#define __35EED826_5A80_11E2_8C72002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace FrameRateAndColorControlCmd
    {
      enum
      {
        Opcode = 0x2b,

        FR_40 = 0,
        FR_43 = 1,
        FR_45 = 2,
        FR_48 = 3,
        FR_51 = 4,
        FR_55 = 5,
        FR_59 = 6,
        FR_64 = 7,
        FR_70 = 8,
        FR_77 = 9,
        FR_85 = 10,
        FR_96_DEFAULT = 11,
        FR_110 = 12,
        FR_128 = 13
      };
    }
  }
}


#endif /* FRAMERATEANDCOLORCONTROLCMD_H_ */
