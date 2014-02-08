/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#ifndef __6D47AC7A_80FE_4105_8E5DD2195694A3ED
#define __6D47AC7A_80FE_4105_8E5DD2195694A3ED


namespace stm32plus
{
  namespace ili9325
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
