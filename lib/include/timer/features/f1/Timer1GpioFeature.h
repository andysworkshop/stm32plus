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
  struct TIM1_ETR {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_12,GPIO_Pin_12,0,GPIO_Pin_7 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH1_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_8,GPIO_Pin_8,0,GPIO_Pin_9 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH1_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_8,GPIO_Pin_8,0,GPIO_Pin_9 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH2_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_9,GPIO_Pin_9,0,GPIO_Pin_11 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH2_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_9,GPIO_Pin_9,0,GPIO_Pin_11 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH3_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_10,GPIO_Pin_10,0,GPIO_Pin_13 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH3_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_10,GPIO_Pin_10,0,GPIO_Pin_13 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH4_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_11,GPIO_Pin_11,0,GPIO_Pin_14 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH4_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_11,GPIO_Pin_11,0,GPIO_Pin_14 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_BKIN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_12,GPIO_Pin_6,0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH1N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,GPIOA,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_13,GPIO_Pin_7,0,GPIO_Pin_8 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH2N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,GPIOB,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_14,GPIO_Pin_0,0,GPIO_Pin_10 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  template<TimerGpioRemapLevel TRemapLevel>
  struct TIM1_CH3N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @tparam TRemapLevel The remap level (none, partial1, partial2, full)
     */
 
    static void initialise() {

      static constexpr GPIO_TypeDef *const ports[4]={ GPIOB,GPIOB,nullptr,GPIOE };
      static constexpr const uint16_t pins[4]={ GPIO_Pin_15,GPIO_Pin_1,0,GPIO_Pin_12 };

      GpioPinInitialiser::initialise(ports[TRemapLevel],pins[TRemapLevel],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,GpioAlternateFunctionMapper<PERIPHERAL_TIMER1,(uint32_t)ports[TRemapLevel],pins[TRemapLevel]>::GPIO_AF); 
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer1GpioFeature<REMAP_NONE,TIM1_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,template<TimerGpioRemapLevel> class TF0=NullTimerGpio,template<TimerGpioRemapLevel> class TF1=NullTimerGpio,template<TimerGpioRemapLevel> class TF2=NullTimerGpio,template<TimerGpioRemapLevel> class TF3=NullTimerGpio,template<TimerGpioRemapLevel> class TF4=NullTimerGpio,template<TimerGpioRemapLevel> class TF5=NullTimerGpio,template<TimerGpioRemapLevel> class TF6=NullTimerGpio,template<TimerGpioRemapLevel> class TF7=NullTimerGpio,template<TimerGpioRemapLevel> class TF8=NullTimerGpio>
  struct Timer1GpioFeature;


  template<template<TimerGpioRemapLevel> class TF0,template<TimerGpioRemapLevel> class TF1,template<TimerGpioRemapLevel> class TF2,template<TimerGpioRemapLevel> class TF3,template<TimerGpioRemapLevel> class TF4,template<TimerGpioRemapLevel> class TF5,template<TimerGpioRemapLevel> class TF6,template<TimerGpioRemapLevel> class TF7,template<TimerGpioRemapLevel> class TF8>
  struct Timer1GpioFeature<TIMER_REMAP_NONE,TF0,TF1,TF2,TF3,TF4,TF5,TF6,TF7,TF8> : public TimerFeatureBase {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      TF0<TIMER_REMAP_NONE>::initialise();
      TF1<TIMER_REMAP_NONE>::initialise();
      TF2<TIMER_REMAP_NONE>::initialise();
      TF3<TIMER_REMAP_NONE>::initialise();
      TF4<TIMER_REMAP_NONE>::initialise();
      TF5<TIMER_REMAP_NONE>::initialise();
      TF6<TIMER_REMAP_NONE>::initialise();
      TF7<TIMER_REMAP_NONE>::initialise();
      TF8<TIMER_REMAP_NONE>::initialise();
    }
  };


  template<template<TimerGpioRemapLevel> class TF0,template<TimerGpioRemapLevel> class TF1,template<TimerGpioRemapLevel> class TF2,template<TimerGpioRemapLevel> class TF3,template<TimerGpioRemapLevel> class TF4,template<TimerGpioRemapLevel> class TF5,template<TimerGpioRemapLevel> class TF6,template<TimerGpioRemapLevel> class TF7,template<TimerGpioRemapLevel> class TF8>
  struct Timer1GpioFeature<TIMER_REMAP_PARTIAL1,TF0,TF1,TF2,TF3,TF4,TF5,TF6,TF7,TF8> : public TimerFeatureBase {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);
      TF0<TIMER_REMAP_PARTIAL1>::initialise();
      TF1<TIMER_REMAP_PARTIAL1>::initialise();
      TF2<TIMER_REMAP_PARTIAL1>::initialise();
      TF3<TIMER_REMAP_PARTIAL1>::initialise();
      TF4<TIMER_REMAP_PARTIAL1>::initialise();
      TF5<TIMER_REMAP_PARTIAL1>::initialise();
      TF6<TIMER_REMAP_PARTIAL1>::initialise();
      TF7<TIMER_REMAP_PARTIAL1>::initialise();
      TF8<TIMER_REMAP_PARTIAL1>::initialise();
    }
  };


  template<template<TimerGpioRemapLevel> class TF0,template<TimerGpioRemapLevel> class TF1,template<TimerGpioRemapLevel> class TF2,template<TimerGpioRemapLevel> class TF3,template<TimerGpioRemapLevel> class TF4,template<TimerGpioRemapLevel> class TF5,template<TimerGpioRemapLevel> class TF6,template<TimerGpioRemapLevel> class TF7,template<TimerGpioRemapLevel> class TF8>
  struct Timer1GpioFeature<TIMER_REMAP_FULL,TF0,TF1,TF2,TF3,TF4,TF5,TF6,TF7,TF8> : public TimerFeatureBase {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);
      TF0<TIMER_REMAP_FULL>::initialise();
      TF1<TIMER_REMAP_FULL>::initialise();
      TF2<TIMER_REMAP_FULL>::initialise();
      TF3<TIMER_REMAP_FULL>::initialise();
      TF4<TIMER_REMAP_FULL>::initialise();
      TF5<TIMER_REMAP_FULL>::initialise();
      TF6<TIMER_REMAP_FULL>::initialise();
      TF7<TIMER_REMAP_FULL>::initialise();
      TF8<TIMER_REMAP_FULL>::initialise();
    }
  };
}
