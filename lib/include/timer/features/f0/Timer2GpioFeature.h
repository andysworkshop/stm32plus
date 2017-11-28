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
  struct TIM2_ETR {

    TIM2_ETR() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOA_BASE,GPIOA_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_0,GPIO_Pin_15,GPIO_Pin_0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH1_IN {

    TIM2_CH1_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOA_BASE,GPIOA_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_0,GPIO_Pin_15,GPIO_Pin_0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH1_OUT {

    TIM2_CH1_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOA_BASE,GPIOA_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_0,GPIO_Pin_15,GPIO_Pin_0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH2_IN {

    TIM2_CH2_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOB_BASE,GPIOA_BASE,GPIOB_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_1,GPIO_Pin_3,GPIO_Pin_1,GPIO_Pin_3 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH2_OUT {

    TIM2_CH2_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOB_BASE,GPIOA_BASE,GPIOB_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_1,GPIO_Pin_3,GPIO_Pin_1,GPIO_Pin_3 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH3_IN {

    TIM2_CH3_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOB_BASE,GPIOB_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_2,GPIO_Pin_2,GPIO_Pin_10,GPIO_Pin_10 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH3_OUT {

    TIM2_CH3_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOB_BASE,GPIOB_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_2,GPIO_Pin_2,GPIO_Pin_10,GPIO_Pin_10 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH4_IN {

    TIM2_CH4_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOB_BASE,GPIOB_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_3,GPIO_Pin_3,GPIO_Pin_11,GPIO_Pin_11 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };


  /**
   * Initialise GPIO pins for this timer GPIO mode
   * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
   */

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM2_CH4_OUT {

    TIM2_CH4_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,GPIOA_BASE,GPIOB_BASE,GPIOB_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_3,GPIO_Pin_3,GPIO_Pin_11,GPIO_Pin_11 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER2>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER2,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF);
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer2GpioFeature<REMAP_NONE,TIM2_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class... Features>
  struct Timer2GpioFeature;


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase, Features<TIMER_REMAP_NONE>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_PARTIAL1,Features...> : TimerFeatureBase, Features<TIMER_REMAP_PARTIAL1>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_PARTIAL2,Features...> : TimerFeatureBase, Features<TIMER_REMAP_PARTIAL2>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer2GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase, Features<TIMER_REMAP_FULL>... {
    Timer2GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
    }
  };
}
