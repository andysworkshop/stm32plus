/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __535BE1CA_C0D1_4CDD_B1DE327C9DFEBB00
#define __535BE1CA_C0D1_4CDD_B1DE327C9DFEBB00


namespace stm32plus
{
  namespace ili9325
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
