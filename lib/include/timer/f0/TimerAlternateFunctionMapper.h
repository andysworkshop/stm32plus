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
   * Specialisation of the AF mapper for this peripheral. The timer AF mappings for the F0
   * are not always the same
   */


  /*
   * TIM1 is always AF2
   */

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_2
    };
  };


  /*
   * TIM2 is always AF2
   */

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_2
    };
  };


  /*
   * TIM3 is always AF1
   */

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_1
    };
  };


  /*
   * TIM14 is on AF4 for port A and AF0 for port B
   */

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER14,GPIOA_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_4
    };
  };

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER14,GPIOB_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_0
    };
  };


  /*
   * TIM15 is on AF0 for port A and AF1 for port B
   */

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER15,GPIOA_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_0
    };
  };

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER15,GPIOB_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_1
    };
  };


  /*
   * TIM16 is on AF5 for port A and AF2 for port B
   */

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER16,GPIOA_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_5
    };
  };

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER16,GPIOB_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_2
    };
  };


  /*
   * TIM17 is on AF5 for PA7, AF0 for PA10 and AF2 for port B
   */

  template<>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER17,GPIOA_BASE,GPIO_Pin_7> {
    enum {
      GPIO_AF = GPIO_AF_5
    };
  };

  template<>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER17,GPIOA_BASE,GPIO_Pin_10> {
    enum {
      GPIO_AF = GPIO_AF_0
    };
  };

  template<uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER17,GPIOB_BASE,TPin> {
    enum {
      GPIO_AF = GPIO_AF_2
    };
  };
}
