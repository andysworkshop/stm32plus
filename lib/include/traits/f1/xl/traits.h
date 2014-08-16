/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

#if !defined(STM32PLUS_F1_XL)
#error This include file is not for your MCU
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
      PERIPHERAL_BASE = ADC1_BASE,
      V25 = 1430,         // from the datasheet, scaled by 1000
      AVG_SLOPE = 4300
    };
  };
  typedef PeripheralTraits<PERIPHERAL_ADC1> Adc1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_ADC2> {
    enum {
      PERIPHERAL_BASE = ADC2_BASE
    };
  };
  typedef PeripheralTraits<PERIPHERAL_ADC2> Adc2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_ADC3> {
    enum {
      PERIPHERAL_BASE = ADC3_BASE
    };
  };
  typedef PeripheralTraits<PERIPHERAL_ADC3> Adc3PeripheralTraits;
  template<>
  struct PeripheralTraits<PERIPHERAL_DAC1> {
    enum {
      CHANNEL_NUMBER = DAC_Channel_1
    };
  };
  typedef PeripheralTraits<PERIPHERAL_DAC1> Dac1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_DAC2> {
    enum {
      CHANNEL_NUMBER = DAC_Channel_2
    };
  };
  typedef PeripheralTraits<PERIPHERAL_DAC2> Dac2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_GPIO> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz
    };
  };
  typedef PeripheralTraits<PERIPHERAL_GPIO> GpioPeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_FSMC> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_FSMC> FsmcPeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SDIO> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SDIO> SdioPeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = I2C1_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C1> I2C1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = I2C2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C2> I2C2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2S2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2S2> I2S2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2S3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI3_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2S3> I2S3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI1_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI1> Spi1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI2> Spi2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI3_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI3> Spi3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM1_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER1> Timer1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM2_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER2> Timer2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM3_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER3> Timer3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER4> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM4_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER4> Timer4PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER5> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM5_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER5> Timer5PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER6> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM6_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER6> Timer6PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER7> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM7_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER7> Timer7PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER8> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM8_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER8> Timer8PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER9> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM9_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER9> Timer9PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER10> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM10_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER10> Timer10PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER11> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM11_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER11> Timer11PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER12> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM12_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER12> Timer12PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER13> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM13_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER13> Timer13PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER14> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM14_BASE,
      COUNTER_MAX = 0xFFFF
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER14> Timer14PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART1_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART1> Usart1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART2_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART2> Usart2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART3_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART3> Usart3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_UART4> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = UART4_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_UART4> Uart4PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_UART5> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = UART5_BASE,
    };
  };
  typedef PeripheralTraits<PERIPHERAL_UART5> Uart5PeripheralTraits;
}
