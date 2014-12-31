/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


#if !defined(STM32PLUS_F0)
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * The traits templates give us a type as a container for the various constants
   * that we need to operate a peripheral. See config/custom_traits.h for the
   * mechanism that you can use to override these defaults, for example to change
   * the GPIO speed to something other than the default.
   */

  template<PeripheralName TPeripheralName>
  struct PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_ADC1> {
    enum {
      PERIPHERAL_BASE = ADC1_BASE
    };
  };
  typedef PeripheralTraits<PERIPHERAL_ADC1> Adc1PeripheralTraits;

#if defined(STM32PLUS_F0_51)

  template<>
  struct PeripheralTraits<PERIPHERAL_DAC1> {
    enum {
      CHANNEL_NUMBER = DAC_Channel_1
    };
  };
  typedef PeripheralTraits<PERIPHERAL_DAC1> Dac1PeripheralTraits;

#endif
 
 template<>
  struct PeripheralTraits<PERIPHERAL_GPIO> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3
    };
  };
  typedef PeripheralTraits<PERIPHERAL_GPIO> GpioPeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_FSMC> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_FSMC> FsmcPeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SDIO> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SDIO> SdioPeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C1> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = I2C1_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C1> I2C1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C2> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = I2C2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C2> I2C2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2S2> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = SPI2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2S2> I2S2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI1> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = SPI1_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI1> Spi1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI2> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = SPI2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI2> Spi2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER1> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM1_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER1> Timer1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER2> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM2_BASE,
      COUNTER_MAX = 0xFFFFFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER2> Timer2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER3> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM3_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER3> Timer3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER6> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM6_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER6> Timer6PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER14> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM14_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER14> Timer14PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER15> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM15_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER15> Timer15PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER16> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM16_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER16> Timer16PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER17> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = TIM17_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER17> Timer17PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART1> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = USART1_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART1> Usart1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART2> {
    enum {
      GPIO_SPEED = GPIO_Speed_Level_3,
      PERIPHERAL_BASE = USART2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART2> Usart2PeripheralTraits;
}
