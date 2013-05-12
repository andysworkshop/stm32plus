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

  struct TIM5_CH1_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_0,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  struct TIM5_CH1_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_0,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_AF); 
    }
  };

  struct TIM5_CH2_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_1,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  struct TIM5_CH2_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_1,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_AF); 
    }
  };

  struct TIM5_CH3_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_2,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  struct TIM5_CH3_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_2,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_AF); 
    }
  };

  struct TIM5_CH4_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_3,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  struct TIM5_CH4_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOA,nullptr,nullptr,nullptr };
      static const uint16_t pins[4]={ GPIO_Pin_3,0,0,0 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER5>::GPIO_AF); 
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer5GpioFeature<REMAP_NONE,TIM5_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,class TF0=NullTimerGpio,class TF1=NullTimerGpio,class TF2=NullTimerGpio,class TF3=NullTimerGpio>
  struct Timer5GpioFeature;


  template<class TF0,class TF1,class TF2,class TF3>
  struct Timer5GpioFeature<TIMER_REMAP_NONE,TF0,TF1,TF2,TF3> : public TimerFeatureBase {
    Timer5GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      TF0::initialise(TIMER_REMAP_NONE);
      TF1::initialise(TIMER_REMAP_NONE);
      TF2::initialise(TIMER_REMAP_NONE);
      TF3::initialise(TIMER_REMAP_NONE);
    }
  };
}
