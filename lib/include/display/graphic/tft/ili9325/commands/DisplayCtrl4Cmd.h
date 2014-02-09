/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __475F237E_99BB_4DCF_8876FE3EB7715519
#define __475F237E_99BB_4DCF_8876FE3EB7715519


namespace stm32plus
{
  namespace ili9325
  {
    namespace DisplayCtrl4Cmd
    {
      enum
      {
        Opcode = 0xa,

        FMARK_OUTPUT_ENABLE = 1<<3
      };

      inline int FMARK_OUTPUT_INTERVAL(uint16_t n) { return n; }
    }
  }
}


#endif
