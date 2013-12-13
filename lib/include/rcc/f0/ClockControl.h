/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /*
   * TIM15
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER15> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,DISABLE);
    }
  };


  /*
   * TIM16
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER16> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,DISABLE);
    }
  };


  /*
   * TIM17
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER17> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,DISABLE);
    }
  };
}
