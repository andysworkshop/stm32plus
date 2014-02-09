/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Specialisation of the AF mapper for this peripheral. The F4 has the same AF ID
   * regardless of the port so we specialise only on the peripheral
   */

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM1
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM2
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM3
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM4
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER5,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM5
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM8
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER9,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM9
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER10,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM10
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER11,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM11
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER12,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM12
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER13,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM13
    };
  };

  template<uint32_t TPort,uint16_t TPin>
  struct GpioAlternateFunctionMapper<PERIPHERAL_TIMER14,TPort,TPin> {
    enum {
      GPIO_AF = GPIO_AF_TIM14
    };
  };
}
