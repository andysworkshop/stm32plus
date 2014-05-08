/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 *
 * THIS IS AN AUTOMATICALLY GENERATED FILE - DO NOT EDIT!
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif

namespace stm32plus {


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_ETR {

    TIM4_ETR() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOE,NULL,NULL,NULL };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_0,0,0,0 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH1_IN {

    TIM4_CH1_IN() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,GPIO_Pin_12 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH1_OUT {

    TIM4_CH1_OUT() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,GPIO_Pin_12 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH2_IN {

    TIM4_CH2_IN() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,0,GPIO_Pin_13 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH2_OUT {

    TIM4_CH2_OUT() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,0,GPIO_Pin_13 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH3_IN {

    TIM4_CH3_IN() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_8,0,0,GPIO_Pin_14 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH3_OUT {

    TIM4_CH3_OUT() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_8,0,0,GPIO_Pin_14 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH4_IN {

    TIM4_CH4_IN() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_9,0,0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM4_CH4_OUT {

    TIM4_CH4_OUT() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,NULL,NULL,GPIOD };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_9,0,0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer4GpioFeature<REMAP_NONE,TIM4_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class... Features>
  struct Timer4GpioFeature;


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer4GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase, Features<TIMER_REMAP_NONE>... {
    Timer4GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer4GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase, Features<TIMER_REMAP_FULL>... {
    Timer4GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
