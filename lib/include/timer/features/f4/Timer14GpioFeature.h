/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011 to 2014 Andy Brown <www.andybrown.me.uk>
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

  struct TIM14_PinPackage_Remap_None {
    typedef gpio::PA7 TIM14_CH1_Pin;
  };

  struct TIM14_PinPackage_Remap_Full {
    typedef gpio::PF9 TIM14_CH1_Pin;
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM14_CH1_IN {

    TIM14_CH1_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM14_CH1_Pin::Port),
          TPinPackage::TIM14_CH1_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER14>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER14,
          TPinPackage::TIM14_CH1_Pin::Port,
          TPinPackage::TIM14_CH1_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM14_CH1_OUT=TIM14_CH1_IN<TPinPackage>;

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer14GpioFeature<TIMER_REMAP_NONE,TIM14_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer14GpioFeature;

  template<template<typename> class... Features>
  struct Timer14GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase,Features<TIM14_PinPackage_Remap_None>... {
    Timer14GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer14GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase,Features<TIM14_PinPackage_Remap_Full>... {
    Timer14GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  /**
   * Custom structure to allow any pin mapping.
   *
   * e.g:
   *    Timer14CustomGpioFeature<TIM14_CH1_OUT<Myclass>>
   * and in "MyClass" you would do a public declaration:
   *    typedef gpio::PF9 TIM14_CH1_Pin;
   */

  template<class... Features>
  struct Timer14CustomGpioFeature : TimerFeatureBase,Features... {
    Timer14CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
