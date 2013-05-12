/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012 Andy Brown <www.andybrown.me.uk>
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

  struct TIM1_ETR {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_12,GPIO_Pin_12,0,GPIO_Pin_7 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH1_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_8,GPIO_Pin_8,0,GPIO_Pin_9 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH1_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_8,GPIO_Pin_8,0,GPIO_Pin_9 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH2_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_9,GPIO_Pin_9,0,GPIO_Pin_11 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH2_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_9,GPIO_Pin_9,0,GPIO_Pin_11 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH3_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_10,GPIO_Pin_10,0,GPIO_Pin_13 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH3_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_10,GPIO_Pin_10,0,GPIO_Pin_13 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH4_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_11,GPIO_Pin_11,0,GPIO_Pin_14 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH4_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_11,GPIO_Pin_11,0,GPIO_Pin_14 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_BKIN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOB,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_12,GPIO_Pin_6,0,GPIO_Pin_15 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH1N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOB,GPIOA,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_13,GPIO_Pin_7,0,GPIO_Pin_8 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH2N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOB,GPIOB,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_14,GPIO_Pin_0,0,GPIO_Pin_10 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  struct TIM1_CH3N {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */

    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOB,GPIOB,nullptr,GPIOE };
      static const uint16_t pins[4]={ GPIO_Pin_15,GPIO_Pin_1,0,GPIO_Pin_12 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER1>::GPIO_AF);
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer1GpioFeature<REMAP_NONE,TIM1_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,class TF0=NullTimerGpio,class TF1=NullTimerGpio,class TF2=NullTimerGpio,class TF3=NullTimerGpio,class TF4=NullTimerGpio,class TF5=NullTimerGpio,class TF6=NullTimerGpio,class TF7=NullTimerGpio,class TF8=NullTimerGpio>
  struct Timer1GpioFeature;


  template<class TF0,class TF1,class TF2,class TF3,class TF4,class TF5,class TF6,class TF7,class TF8>
  struct Timer1GpioFeature<TIMER_REMAP_NONE,TF0,TF1,TF2,TF3,TF4,TF5,TF6,TF7,TF8> : public TimerFeatureBase {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      TF0::initialise(TIMER_REMAP_NONE);
      TF1::initialise(TIMER_REMAP_NONE);
      TF2::initialise(TIMER_REMAP_NONE);
      TF3::initialise(TIMER_REMAP_NONE);
      TF4::initialise(TIMER_REMAP_NONE);
      TF5::initialise(TIMER_REMAP_NONE);
      TF6::initialise(TIMER_REMAP_NONE);
      TF7::initialise(TIMER_REMAP_NONE);
      TF8::initialise(TIMER_REMAP_NONE);
    }
  };


  template<class TF0,class TF1,class TF2,class TF3,class TF4,class TF5,class TF6,class TF7,class TF8>
  struct Timer1GpioFeature<TIMER_REMAP_PARTIAL1,TF0,TF1,TF2,TF3,TF4,TF5,TF6,TF7,TF8> : public TimerFeatureBase {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      TF0::initialise(TIMER_REMAP_PARTIAL1);
      TF1::initialise(TIMER_REMAP_PARTIAL1);
      TF2::initialise(TIMER_REMAP_PARTIAL1);
      TF3::initialise(TIMER_REMAP_PARTIAL1);
      TF4::initialise(TIMER_REMAP_PARTIAL1);
      TF5::initialise(TIMER_REMAP_PARTIAL1);
      TF6::initialise(TIMER_REMAP_PARTIAL1);
      TF7::initialise(TIMER_REMAP_PARTIAL1);
      TF8::initialise(TIMER_REMAP_PARTIAL1);
    }
  };


  template<class TF0,class TF1,class TF2,class TF3,class TF4,class TF5,class TF6,class TF7,class TF8>
  struct Timer1GpioFeature<TIMER_REMAP_FULL,TF0,TF1,TF2,TF3,TF4,TF5,TF6,TF7,TF8> : public TimerFeatureBase {
    Timer1GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      TF0::initialise(TIMER_REMAP_FULL);
      TF1::initialise(TIMER_REMAP_FULL);
      TF2::initialise(TIMER_REMAP_FULL);
      TF3::initialise(TIMER_REMAP_FULL);
      TF4::initialise(TIMER_REMAP_FULL);
      TF5::initialise(TIMER_REMAP_FULL);
      TF6::initialise(TIMER_REMAP_FULL);
      TF7::initialise(TIMER_REMAP_FULL);
      TF8::initialise(TIMER_REMAP_FULL);
    }
  };
}
