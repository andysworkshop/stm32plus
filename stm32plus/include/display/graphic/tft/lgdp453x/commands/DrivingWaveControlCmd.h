/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35FEBD9A_5A80_11E2_9EC6002481184849
#define __35FEBD9A_5A80_11E2_9EC6002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace DrivingWaveControlCmd
    {
      enum
      {
        Opcode = 2,

        EOR = 1<<8,
        BC0 = 1<<9
      };
      inline uint16_t NW(uint16_t n){return n ;}
    }
  }
}


#endif /* STARTOSCILLATIONCOMMAND_H_ */
