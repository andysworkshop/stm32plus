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

  struct TIM11_CH1_IN {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOB,nullptr,nullptr,GPIOF };
      static const uint16_t pins[4]={ GPIO_Pin_9,0,0,GPIO_Pin_7 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::INPUT,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER11>::GPIO_SPEED,Gpio::PUPD_NONE); 
    }
  };

  struct TIM11_CH1_OUT {

    /**
     * Initialise GPIO pins for this timer GPIO mode
     * @param remap The remap level (none, partial1, partial2, full)
     */
 
    static void initialise(TimerGpioRemapLevel remap) {

      static GPIO_TypeDef *const ports[4]={ GPIOB,nullptr,nullptr,GPIOF };
      static const uint16_t pins[4]={ GPIO_Pin_9,0,0,GPIO_Pin_7 };

      GpioPinInitialiser::initialise(ports[remap],pins[remap],Gpio::ALTERNATE_FUNCTION,(GPIOSpeed_TypeDef)PeripheralTraits<PERIPHERAL_TIMER11>::GPIO_SPEED,Gpio::PUPD_NONE,Gpio::PUSH_PULL,PeripheralTraits<PERIPHERAL_TIMER11>::GPIO_AF); 
    }
  };

  /**
   * Timer feature to enable any number of the GPIO alternate function outputs.
   * All remap levels are supported. An example declaration could be:
   *
   * Timer11GpioFeature<REMAP_NONE,TIM11_CH1_OUT>
   */

  template<TimerGpioRemapLevel TRemapLevel,class TF0=NullTimerGpio>
  struct Timer11GpioFeature;


  template<class TF0>
  struct Timer11GpioFeature<TIMER_REMAP_NONE,TF0> : public TimerFeatureBase {
    Timer11GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      TF0::initialise(TIMER_REMAP_NONE);
    }
  };


  template<class TF0>
  struct Timer11GpioFeature<TIMER_REMAP_FULL,TF0> : public TimerFeatureBase {
    Timer11GpioFeature(Timer& timer) : TimerFeatureBase(timer) {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_TIM11,ENABLE);
      TF0::initialise(TIMER_REMAP_FULL);
    }
  };
}
