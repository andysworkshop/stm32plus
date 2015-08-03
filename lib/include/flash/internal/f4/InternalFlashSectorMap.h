/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * Size map declaration for the F40x and F41x devices. 12 sectors.
   */

  struct InternalFlashSectorMapF40xF41x {
    static const uint8_t SizeMap[12];
  };


  /**
   * Size map declaration for the F42x and F43x devices. 24 sectors.
   */

  struct InternalFlashSectorMapF42xF43x {
    static const uint8_t SizeMap[24];
  };
}
