/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __361E0B32_5A80_11E2_AE95002481184849
#define __361E0B32_5A80_11E2_AE95002481184849


namespace stm32plus
{
  namespace lgdp453x
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
