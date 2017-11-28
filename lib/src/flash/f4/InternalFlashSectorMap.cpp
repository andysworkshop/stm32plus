/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/flash/internal.h"


namespace stm32plus {

  /**
   * Size map declaration for the F40x and F41x devices. 12 sectors.
   */

  const uint8_t InternalFlashSectorMapF40xF41x::SizeMap[12]={
    16,16,16,16,64,128,128,128,128,128,128,128
  };


  /**
   * Size map declaration for the F42x and F43x devices. 24 sectors.
   */

  const uint8_t InternalFlashSectorMapF42xF43x::SizeMap[24]={
    16,16,16,16,64,128,128,128,128,128,128,128,16,16,16,16,64,128,128,128,128,128,128,128
  };
}


#endif
