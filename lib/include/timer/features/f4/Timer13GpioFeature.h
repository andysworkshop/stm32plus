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
  struct TIM13_CH1_IN {

    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,NULL,NULL,GPIOF };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,GPIO_Pin_8 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER13>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER13,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM13_CH1_OUT {

    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,NULL,NULL,GPIOF };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,GPIO_Pin_8 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER13>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER13,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer13GpioFeature<REMAP_NONE,TIM13_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class TF0=NullTimerGpio>
  struct Timer13GpioFeature;


  template<template<TimerGpioRemapLevel> class TF0>
  struct Timer13GpioFeature<TIMER_REMAP_NONE,TF0> : public TimerFeatureBase {
    Timer13GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      TF0<TIMER_REMAP_NONE>::initialise();
    }
  };


  template<template<TimerGpioRemapLevel> class TF0>
  struct Timer13GpioFeature<TIMER_REMAP_FULL,TF0> : public TimerFeatureBase {
    Timer13GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      TF0<TIMER_REMAP_FULL>::initialise();
    }
  };
}
