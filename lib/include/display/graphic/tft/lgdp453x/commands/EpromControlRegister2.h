/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __388D579E_5E34_11E2_93E8002481184849
#define __388D579E_5E34_11E2_93E8002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace EpromControlRegister2
    {
      enum
      {
        Opcode = 0x41,

        AUTOWR = 1<<4
      };
      inline uint16_t VCMSEL(uint16_t n){return n;}
      inline uint16_t RA(uint16_t n){return n<<2;}
    }
  }
}


#endif
