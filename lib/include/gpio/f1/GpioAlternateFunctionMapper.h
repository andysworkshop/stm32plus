/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */


#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * This is the dummy implementation for the F1.
   */

  template<PeripheralName TPeripheralName,uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper {
    enum {
      GPIO_AF = 0       ///< never used
    };
  };
}
