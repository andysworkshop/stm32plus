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

  struct TIM2_PinPackage_Remap_None {
    typedef gpio::PA0 TIM2_ETR_Pin;
    typedef gpio::PA0 TIM2_CH1_Pin;
    typedef gpio::PA1 TIM2_CH2_Pin;
    typedef gpio::PA2 TIM2_CH3_Pin;
    typedef gpio::PA3 TIM2_CH4_Pin;
  };

  struct TIM2_PinPackage_Remap_Partial1 {
    typedef gpio::PA15 TIM2_ETR_Pin;
    typedef gpio::PA15 TIM2_CH1_Pin;
    typedef gpio::PB3 TIM2_CH2_Pin;
    typedef gpio::PA2 TIM2_CH3_Pin;
    typedef gpio::PA3 TIM2_CH4_Pin;
  };

  struct TIM2_PinPackage_Remap_Partial2 {
    typedef gpio::PA0 TIM2_ETR_Pin;
    typedef gpio::PA0 TIM2_CH1_Pin;
    typedef gpio::PA1 TIM2_CH2_Pin;
    typedef gpio::PB10 TIM2_CH3_Pin;
    typedef gpio::PB11 TIM2_CH4_Pin;
  };

  struct TIM2_PinPackage_Remap_Full {
    typedef gpio::PA15 TIM2_ETR_Pin;
    typedef gpio::PA15 TIM2_CH1_Pin;
    typedef gpio::PB3 TIM2_CH2_Pin;
    typedef gpio::PB10 TIM2_CH3_Pin;
    typedef gpio::PB11 TIM2_CH4_Pin;
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM2_ETR {

    TIM2_ETR() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM2_ETR_Pin::Port),
          TPinPackage::TIM2_ETR_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,
          TPinPackage::TIM2_ETR_Pin::Port,
          TPinPackage::TIM2_ETR_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM2_CH1_IN {

    TIM2_CH1_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM2_CH1_Pin::Port),
          TPinPackage::TIM2_CH1_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,
          TPinPackage::TIM2_CH1_Pin::Port,
          TPinPackage::TIM2_CH1_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM2_CH1_OUT=TIM2_CH1_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM2_CH2_IN {

    TIM2_CH2_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM2_CH2_Pin::Port),
          TPinPackage::TIM2_CH2_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,
          TPinPackage::TIM2_CH2_Pin::Port,
          TPinPackage::TIM2_CH2_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM2_CH2_OUT=TIM2_CH2_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM2_CH3_IN {

    TIM2_CH3_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM2_CH3_Pin::Port),
          TPinPackage::TIM2_CH3_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,
          TPinPackage::TIM2_CH3_Pin::Port,
          TPinPackage::TIM2_CH3_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM2_CH3_OUT=TIM2_CH3_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM2_CH4_IN {

    TIM2_CH4_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM2_CH4_Pin::Port),
          TPinPackage::TIM2_CH4_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,
          TPinPackage::TIM2_CH4_Pin::Port,
          TPinPackage::TIM2_CH4_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM2_CH4_OUT=TIM2_CH4_IN<TPinPackage>;

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer2GpioFeature<TIMER_REMAP_NONE,TIM2_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer2GpioFeature;

  template<template<typename> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase,Features<TIM2_PinPackage_Remap_None>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_PARTIAL1,Features...> : TimerFeatureBase,Features<TIM2_PinPackage_Remap_Partial1>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_PARTIAL2,Features...> : TimerFeatureBase,Features<TIM2_PinPackage_Remap_Partial2>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase,Features<TIM2_PinPackage_Remap_Full>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
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
  struct Timer2CustomGpioFeature : TimerFeatureBase,Features... {
    Timer2CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
