/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __361AC7EC_5A80_11E2_B024002481184849
#define __361AC7EC_5A80_11E2_B024002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PanelInterfaceControl3
    {
      enum
      {
        Opcode = 0x93
      };
      inline uint16_t MCPI(uint16_t n){return n;}
      inline uint16_t SEQI(uint16_t n){return n<<8;}
    }
  }
}


#endif
