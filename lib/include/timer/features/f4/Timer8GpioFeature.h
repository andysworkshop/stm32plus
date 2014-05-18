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

  struct TIM8_PinPackage_Remap_None {
    typedef gpio::PA0 TIM8_ETR_Pin;
    typedef gpio::PA6 TIM8_BKIN_Pin;
    typedef gpio::PA7 TIM8_CH1N_Pin;
    typedef gpio::PB0 TIM8_CH2N_Pin;
    typedef gpio::PB1 TIM8_CH3N_Pin;
    typedef gpio::PC6 TIM8_CH1_Pin;
    typedef gpio::PC7 TIM8_CH2_Pin;
    typedef gpio::PC8 TIM8_CH3_Pin;
    typedef gpio::PC9 TIM8_CH4_Pin;
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_ETR {

    TIM8_ETR() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_ETR_Pin::Port),
          TPinPackage::TIM8_ETR_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_ETR_Pin::Port,
          TPinPackage::TIM8_ETR_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_BKIN {

    TIM8_BKIN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_BKIN_Pin::Port),
          TPinPackage::TIM8_BKIN_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_BKIN_Pin::Port,
          TPinPackage::TIM8_BKIN_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH1N {

    TIM8_CH1N() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH1N_Pin::Port),
          TPinPackage::TIM8_CH1N_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH1N_Pin::Port,
          TPinPackage::TIM8_CH1N_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH2N {

    TIM8_CH2N() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH2N_Pin::Port),
          TPinPackage::TIM8_CH2N_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH2N_Pin::Port,
          TPinPackage::TIM8_CH2N_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH3N {

    TIM8_CH3N() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH3N_Pin::Port),
          TPinPackage::TIM8_CH3N_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH3N_Pin::Port,
          TPinPackage::TIM8_CH3N_Pin::Pin>::GPIO_AF);
      }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH1_IN {

    TIM8_CH1_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH1_Pin::Port),
          TPinPackage::TIM8_CH1_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH1_Pin::Port,
          TPinPackage::TIM8_CH1_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM8_CH1_OUT=TIM8_CH1_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH2_IN {

    TIM8_CH2_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH2_Pin::Port),
          TPinPackage::TIM8_CH2_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH2_Pin::Port,
          TPinPackage::TIM8_CH2_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM8_CH2_OUT=TIM8_CH2_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH3_IN {

    TIM8_CH3_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH3_Pin::Port),
          TPinPackage::TIM8_CH3_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH3_Pin::Port,
          TPinPackage::TIM8_CH3_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM8_CH3_OUT=TIM8_CH3_IN<TPinPackage>;

  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TPinPackage A type containing pin definitions for this timer feature
   */

  template<typename TPinPackage>
  struct TIM8_CH4_IN {

    TIM8_CH4_IN() {
      GpioPinInitialiser::initialise(
          reinterpret_cast<GPIO_TypeDef *>(TPinPackage::TIM8_CH4_Pin::Port),
          TPinPackage::TIM8_CH4_Pin::Pin,
          Gpio::ALTERNATE_FUNCTION,
          (GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,
          Gpio::PUPD_NONE,
          Gpio::PUSH_PULL,
          GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,
          TPinPackage::TIM8_CH4_Pin::Port,
          TPinPackage::TIM8_CH4_Pin::Pin>::GPIO_AF);
      }
  };

  template<typename TPinPackage> using TIM8_CH4_OUT=TIM8_CH4_IN<TPinPackage>;

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer8GpioFeature<TIMER_REMAP_NONE,TIM8_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<typename> class... Features>
  struct Timer8GpioFeature;

  template<template<typename> class... Features>
  struct Timer8GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase,Features<TIM8_PinPackage_Remap_None>... {
    Timer8GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
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
  struct Timer8CustomGpioFeature : TimerFeatureBase,Features... {
    Timer8CustomGpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
