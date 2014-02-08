/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __D060ACDD_4F73_4A0F_A5E09B49371F5A17
#define __D060ACDD_4F73_4A0F_A5E09B49371F5A17


namespace stm32plus
{
  namespace ili9481
  {
    namespace FrameRateAndInversionControlCmd
    {
      enum
      {
        Opcode = 0xC5,

        FR_125 = 0,
        FR_100 = 1,
        FR_85  = 2,
        FR_72  = 3,
        FR_56  = 4,
        FR_50  = 5,
        FR_45  = 6,
        FR_42  = 7
      };
    }
  }
}


#endif /* GRAMSTARTWRITINGCMD_H_ */
