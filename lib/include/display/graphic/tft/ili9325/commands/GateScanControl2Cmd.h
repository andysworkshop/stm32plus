/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __571D7EE2_3342_4F08_BF8459893F30C9ED
#define __571D7EE2_3342_4F08_BF8459893F30C9ED


namespace stm32plus
{
  namespace ili9325
  {
    namespace GateScanControl2Cmd
    {
      enum
      {
        Opcode = 0x61,

        REV = 1,
        VLE = 1<<1,
        NDL = 1<<2
      };
    }
  }
}


#endif
