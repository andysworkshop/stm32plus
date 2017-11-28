/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 *
 * THIS IS AN AUTOMATICALLY GENERATED FILE - DO NOT EDIT!
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif

namespace stm32plus {


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM14_CH1_IN {

    TIM14_CH1_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,GPIOB_BASE,GPIOA_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,GPIO_Pin_1,GPIO_Pin_4 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER14>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER14,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM14_CH1_OUT {

    TIM14_CH1_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,GPIOB_BASE,GPIOA_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,GPIO_Pin_1,GPIO_Pin_4 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER14>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER14,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer14GpioFeature<REMAP_NONE,TIM14_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class... Features>
  struct Timer14GpioFeature;


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer14GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase, Features<TIMER_REMAP_NONE>... {
    Timer14GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer14GpioFeature<TIMER_REMAP_PARTIAL2,Features...> : TimerFeatureBase, Features<TIMER_REMAP_PARTIAL2>... {
    Timer14GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer14GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase, Features<TIMER_REMAP_FULL>... {
    Timer14GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
