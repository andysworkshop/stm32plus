/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __57077944_5B3B_11E2_8E69002481184849
#define __57077944_5B3B_11E2_8E69002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace RegulatorControlCmd
    {
      enum
      {
        Opcode = 0x15
      };
      inline uint16_t RCONT(uint16_t n){return n;}
      inline uint16_t RV(uint16_t n){return n<<4;}
      inline uint16_t RI(uint16_t n){return n<<8;}
      inline uint16_t RSET(uint16_t n){return n<<12;}
    }
  }
}


#endif
