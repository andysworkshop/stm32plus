/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __1F7B085C_240E_4125_AF4BAFBEE5D1CAA5
#define __1F7B085C_240E_4125_AF4BAFBEE5D1CAA5


namespace stm32plus
{
  namespace ili9481
  {
    namespace SetScrollStartCmd
    {
      enum
      {
        Opcode = 0x37

      };

      inline uint16_t VSP(uint16_t n) { return n; }
    }
  }
}


#endif /* GRAMSTARTWRITINGCMD_H_ */
