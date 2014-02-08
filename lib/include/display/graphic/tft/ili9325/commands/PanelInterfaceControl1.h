/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __983642D4_8F3C_4F7C_94A869034FE2EF72
#define __983642D4_8F3C_4F7C_94A869034FE2EF72

#include <stdint.h>


namespace stm32plus
{
  namespace ili9325
  {
    namespace PanelInterfaceControl1
    {
      enum
      {
        Opcode = 0x90
      };

      inline uint16_t RTNI(uint16_t n) { return n; }
      inline uint16_t DIVI(uint16_t n) { return n<<8; }
    }
  }
}


#endif
