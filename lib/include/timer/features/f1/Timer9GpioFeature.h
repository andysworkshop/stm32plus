/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 *
 * THIS IS AN AUTOMATICALLY GENERATED FILE - DO NOT EDIT!
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif

namespace stm32plus {

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM9_CH1_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM9_CH1_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,GPIOE_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_2,0,0,GPIO_Pin_5 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER9>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM9_CH1_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM9_CH1_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,GPIOE_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_2,0,0,GPIO_Pin_5 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER9>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER9,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM9_CH2_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM9_CH2_IN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,GPIOE_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_3,0,0,GPIO_Pin_6 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER9>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM9_CH2_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM9_CH2_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,GPIOE_BASE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_3,0,0,GPIO_Pin_6 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER9>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER9,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer9GpioFeature<REMAP_NONE,TIM9_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class... Features>
  struct Timer9GpioFeature;


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer9GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase, Features<TIMER_REMAP_NONE>... {
    Timer9GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    }
  };


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer9GpioFeature<TIMER_REMAP_FULL,Features...> : TimerFeatureBase, Features<TIMER_REMAP_FULL>... {
    Timer9GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_TIM9,ENABLE);
    }
  };
}
