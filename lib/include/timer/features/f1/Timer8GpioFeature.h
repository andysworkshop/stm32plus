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
  struct TIM8_ETR {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_ETR() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_0,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_BKIN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_BKIN() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH1N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH1N() {

      static constexpr const uint32_t ports[4]={ GPIOA_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH2N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH2N() {

      static constexpr const uint32_t ports[4]={ GPIOB_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_0,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH3N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH3N() {

      static constexpr const uint32_t ports[4]={ GPIOB_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_1,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH1_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH1_IN() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH1_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH1_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_6,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH2_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH2_IN() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH2_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH2_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_7,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH3_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH3_IN() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_8,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH3_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH3_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_8,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH4_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH4_IN() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_9,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM8_CH4_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    TIM8_CH4_OUT() {

      static constexpr const uint32_t ports[4]={ GPIOC_BASE,0,0,0 };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_9,0,0,0 };

      GpioPinInitialiser::initialise((GPIO_TypeDef *)ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER8>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER8,ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer8GpioFeature<REMAP_NONE,TIM8_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class... Features>
  struct Timer8GpioFeature;


  template<template<TimerGpioRemapLevel> class... Features>
  struct Timer8GpioFeature<TIMER_REMAP_NONE,Features...> : TimerFeatureBase, Features<TIMER_REMAP_NONE>... {
    Timer8GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    }
  };
}
