/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __35F975C4_5A80_11E2_9568002481184849
#define __35F975C4_5A80_11E2_9568002481184849


namespace stm32plus
{
  namespace lgdp453x
  {
    namespace PowerCtrl2Cmd
    {
      enum
      {
        Opcode = 0x11
      };
      inline uint16_t VC(uint16_t n){return n;}
      inline uint16_t DC0(uint16_t n){return n<<4;}
      inline uint16_t DC1(uint16_t n){return n<<8;}
    }
  }
}


#endif /* POWERCTRL2CMD_H_ */
