/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __36143814_5A80_11E2_891C002481184849
#define __36143814_5A80_11E2_891C002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace DisplayCtrl4Cmd
    {
      enum
      {
        Opcode = 0xa,

        FMARKOE = 1<<3
      };

      inline int FMI(uint16_t n) { return n; }
    }
  }
}


#endif
