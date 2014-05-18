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

  struct TIM1_PinPackage_Remap_None {
    typedef gpio::PA12 TIM1_ETR_Pin;
    typedef gpio::PA8 TIM1_CH1_Pin;
    typedef gpio::PA9 TIM1_CH2_Pin;
    typedef gpio::PA10 TIM1_CH3_Pin;
    typedef gpio::PA11 TIM1_CH4_Pin;
    typedef gpio::PB12 TIM1_BKIN_Pin;
    typedef gpio::PB13 TIM1_CH1N_Pin;
    typedef gpio::PB14 TIM1_CH2N_Pin;
    typedef gpio::PB15 TIM1_CH3N_Pin;
  };

  struct TIM1_PinPackage_Remap_Partial1 {
    typedef gpio::PA12 TIM1_ETR_Pin;
    typedef gpio::PA8 TIM1_CH1_Pin;
    typedef gpio::PA9 TIM1_CH2_Pin;
    typedef gpio::PA10 TIM1_CH3_Pin;
    typedef gpio::PA11 TIM1_CH4_Pin;
    typedef gpio::PA6 TIM1_BKIN_Pin;
    typedef gpio::PA7 TIM1_CH1N_Pin;
    typedef gpio::PB0 TIM1_CH2N_Pin;
    typedef gpio::PB1 TIM1_CH3N_Pin;
  };

  struct TIM1_PinPackage_Remap_Full {
    typedef gpio::PE7 TIM1_ETR_Pin;
    typedef gpio::PE9 TIM1_CH1_Pin;
    typedef gpio::PE11 TIM1_CH2_Pin;
    typedef gpio::PE13 TIM1_CH3_Pin;
    typedef gpio::PE14 TIM1_CH4_Pin;
    typedef gpio::PE15 TIM1_BKIN_Pin;
    typedef gpio::PE8 TIM1_CH1N_Pin;
    typedef gpio::PE10 TIM1_CH2N_Pin;
    typedef gpio::PE12 TIM1_CH3N_Pin;
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_ETR {

    TIM1_ETR() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_ETR_Pin::Port),
          TPinPackage::TIM1_ETR_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_ETR_Pin::Port,
          TPinPackage::TIM1_ETR_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH1_IN {

    TIM1_CH1_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH1_Pin::Port),
          TPinPackage::TIM1_CH1_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH1_Pin::Port,
          TPinPackage::TIM1_CH1_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM1_CH1_OUT=TIM1_CH1_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH2_IN {

    TIM1_CH2_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH2_Pin::Port),
          TPinPackage::TIM1_CH2_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH2_Pin::Port,
          TPinPackage::TIM1_CH2_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM1_CH2_OUT=TIM1_CH2_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH3_IN {

    TIM1_CH3_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH3_Pin::Port),
          TPinPackage::TIM1_CH3_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH3_Pin::Port,
          TPinPackage::TIM1_CH3_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM1_CH3_OUT=TIM1_CH3_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH4_IN {

    TIM1_CH4_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH4_Pin::Port),
          TPinPackage::TIM1_CH4_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH4_Pin::Port,
          TPinPackage::TIM1_CH4_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM1_CH4_OUT=TIM1_CH4_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_BKIN {

    TIM1_BKIN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_BKIN_Pin::Port),
          TPinPackage::TIM1_BKIN_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_BKIN_Pin::Port,
          TPinPackage::TIM1_BKIN_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH1N {

    TIM1_CH1N() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH1N_Pin::Port),
          TPinPackage::TIM1_CH1N_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH1N_Pin::Port,
          TPinPackage::TIM1_CH1N_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH2N {

    TIM1_CH2N() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH2N_Pin::Port),
          TPinPackage::TIM1_CH2N_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH2N_Pin::Port,
          TPinPackage::TIM1_CH2N_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM1_CH3N {

    TIM1_CH3N() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM1_CH3N_Pin::Port),
          TPinPackage::TIM1_CH3N_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,
          TPinPackage::TIM1_CH3N_Pin::Port,
          TPinPackage::TIM1_CH3N_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer1GpioFeature<TIMER_REMAP_NONE,TIM1_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer1GpioFeature;

  template<template<typename> class... Features>
  struct Timer1GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase,Features<TIM1_PinPackage_Remap_None>... {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer1GpioFeature<TIMER_REMAP_PARTIAL1,Features...> : TimerFeatureBase,Features<TIM1_PinPackage_Remap_Partial1>... {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };

  template<template<typename> class... Features>
  struct Timer1GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase,Features<TIM1_PinPackage_Remap_Full>... {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
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
  struct Timer1CustomGpioFeature : TimerFeatureBase,Features... {
    Timer1CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
