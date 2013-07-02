/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __361C693A_5A80_11E2_A88B002481184849
#define __361C693A_5A80_11E2_A88B002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace DriverOutputControlCmd
    {
      enum
      {
        Opcode = 1,

        SS = 1<<8,
        SM = 1<<10
      };
    }
  }
}


#endif /* STARTOSCILLATIONCOMMAND_H_ */
