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

  struct TIM4_PinPackage_Remap_None {
    typedef gpio::PE0 TIM4_ETR_Pin;
    typedef gpio::PB6 TIM4_CH1_Pin;
    typedef gpio::PB7 TIM4_CH2_Pin;
    typedef gpio::PB8 TIM4_CH3_Pin;
    typedef gpio::PB9 TIM4_CH4_Pin;
  };

  struct TIM4_PinPackage_Remap_Full {
    typedef gpio::PD12 TIM4_CH1_Pin;
    typedef gpio::PD13 TIM4_CH2_Pin;
    typedef gpio::PD14 TIM4_CH3_Pin;
    typedef gpio::PD15 TIM4_CH4_Pin;
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM4_ETR {

    TIM4_ETR() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM4_ETR_Pin::Port),
          TPinPackage::TIM4_ETR_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,
          TPinPackage::TIM4_ETR_Pin::Port,
          TPinPackage::TIM4_ETR_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM4_CH1_IN {

    TIM4_CH1_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM4_CH1_Pin::Port),
          TPinPackage::TIM4_CH1_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,
          TPinPackage::TIM4_CH1_Pin::Port,
          TPinPackage::TIM4_CH1_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM4_CH1_OUT=TIM4_CH1_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM4_CH2_IN {

    TIM4_CH2_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM4_CH2_Pin::Port),
          TPinPackage::TIM4_CH2_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,
          TPinPackage::TIM4_CH2_Pin::Port,
          TPinPackage::TIM4_CH2_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM4_CH2_OUT=TIM4_CH2_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM4_CH3_IN {

    TIM4_CH3_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM4_CH3_Pin::Port),
          TPinPackage::TIM4_CH3_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,
          TPinPackage::TIM4_CH3_Pin::Port,
          TPinPackage::TIM4_CH3_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM4_CH3_OUT=TIM4_CH3_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM4_CH4_IN {

    TIM4_CH4_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM4_CH4_Pin::Port),
          TPinPackage::TIM4_CH4_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER4>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER4,
          TPinPackage::TIM4_CH4_Pin::Port,
          TPinPackage::TIM4_CH4_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM4_CH4_OUT=TIM4_CH4_IN<TPinPackage>;

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer4GpioFeature<TIMER_REMAP_NONE,TIM4_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer4GpioFeature;

  template<template<typename> class... Features>
  struct Timer4GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase,Features<TIM4_PinPackage_Remap_None>... {
    Timer4GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer4GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase,Features<TIM4_PinPackage_Remap_Full>... {
    Timer4GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
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
  struct Timer4CustomGpioFeature : TimerFeatureBase,Features... {
    Timer4CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
