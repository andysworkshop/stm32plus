/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __84E5C76E_5E36_11E2_A293002481184849
#define __84E5C76E_5E36_11E2_A293002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace TestRegister4
    {
      enum
      {
        Opcode = 0xA3
      };
      inline uint16_t WRPW(uint16_t n){return n;}
      inline uint16_t RDSM(uint16_t n){return n<<4;}
    }
  }
}


#endif
