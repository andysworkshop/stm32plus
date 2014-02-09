/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Specialisation of the AF mapper for this peripheral.
   */

  /*
   * USART1 is on AF1 for PA and AF0 for PB
   */

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_USART1,GPIOA_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_1
    };
  };

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_USART1,GPIOB_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_0
    };
  };


  /*
   * USART2 is on AF1 for PA
   */

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_USART2,GPIOA_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_1
    };
  };
}
