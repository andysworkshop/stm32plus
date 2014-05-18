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

  struct TIM3_PinPackage_Remap_None {
    typedef gpio::PD2 TIM3_ETR_Pin;
    typedef gpio::PA6 TIM3_CH1_Pin;
    typedef gpio::PA7 TIM3_CH2_Pin;
    typedef gpio::PB0 TIM3_CH3_Pin;
    typedef gpio::PB1 TIM3_CH4_Pin;
  };

  struct TIM3_PinPackage_Remap_Partial2 {
    typedef gpio::PB4 TIM3_CH1_Pin;
    typedef gpio::PB5 TIM3_CH2_Pin;
    typedef gpio::PB0 TIM3_CH3_Pin;
    typedef gpio::PB1 TIM3_CH4_Pin;
  };

  struct TIM3_PinPackage_Remap_Full {
    typedef gpio::PC6 TIM3_CH1_Pin;
    typedef gpio::PC7 TIM3_CH2_Pin;
    typedef gpio::PC8 TIM3_CH3_Pin;
    typedef gpio::PC9 TIM3_CH4_Pin;
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM3_ETR {

    TIM3_ETR() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM3_ETR_Pin::Port),
          TPinPackage::TIM3_ETR_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER3>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,
          TPinPackage::TIM3_ETR_Pin::Port,
          TPinPackage::TIM3_ETR_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM3_CH1_IN {

    TIM3_CH1_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM3_CH1_Pin::Port),
          TPinPackage::TIM3_CH1_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER3>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,
          TPinPackage::TIM3_CH1_Pin::Port,
          TPinPackage::TIM3_CH1_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM3_CH1_OUT=TIM3_CH1_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM3_CH2_IN {

    TIM3_CH2_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM3_CH2_Pin::Port),
          TPinPackage::TIM3_CH2_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER3>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,
          TPinPackage::TIM3_CH2_Pin::Port,
          TPinPackage::TIM3_CH2_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM3_CH2_OUT=TIM3_CH2_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM3_CH3_IN {

    TIM3_CH3_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM3_CH3_Pin::Port),
          TPinPackage::TIM3_CH3_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER3>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,
          TPinPackage::TIM3_CH3_Pin::Port,
          TPinPackage::TIM3_CH3_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM3_CH3_OUT=TIM3_CH3_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM3_CH4_IN {

    TIM3_CH4_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM3_CH4_Pin::Port),
          TPinPackage::TIM3_CH4_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER3>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER3,
          TPinPackage::TIM3_CH4_Pin::Port,
          TPinPackage::TIM3_CH4_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM3_CH4_OUT=TIM3_CH4_IN<TPinPackage>;

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer3GpioFeature<TIMER_REMAP_NONE,TIM3_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer3GpioFeature;

  template<template<typename> class... Features>
  struct Timer3GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase,Features<TIM3_PinPackage_Remap_None>... {
    Timer3GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer3GpioFeature<TIMER_REMAP_PARTIAL2,Features...> : TimerFeatureBase,Features<TIM3_PinPackage_Remap_Partial2>... {
    Timer3GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer3GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase,Features<TIM3_PinPackage_Remap_Full>... {
    Timer3GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
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
  struct Timer3CustomGpioFeature : TimerFeatureBase,Features... {
    Timer3CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
