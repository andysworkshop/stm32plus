/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


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
  typedef PeripheralTraits<PERIPHERAL_DAC1> Dac1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_GPIO> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz
    };
  };
  typedef PeripheralTraits<PERIPHERAL_GPIO> GpioPeripheralTraits;

#if defined(GPIO_AF_FSMC)
  template<>
  struct PeripheralTraits<PERIPHERAL_FSMC> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      GPIO_AF = GPIO_AF_FSMC
    };
  };
  typedef PeripheralTraits<PERIPHERAL_FSMC> FsmcPeripheralTraits;
#endif

#if defined(GPIO_AF_SDIO)
  template<>
  struct PeripheralTraits<PERIPHERAL_SDIO> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      GPIO_AF = GPIO_AF_SDIO
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SDIO> SdioPeripheralTraits;
#endif

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = I2C1_BASE,
      GPIO_AF = GPIO_AF_I2C1
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C1> I2C1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = I2C2_BASE,
      GPIO_AF = GPIO_AF_I2C2
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C2> I2C2PeripheralTraits;

#if defined(STM32PLUS_F4)

  template<>
  struct PeripheralTraits<PERIPHERAL_I2C3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = I2C3_BASE,
      GPIO_AF = GPIO_AF_I2C3
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2C3> I2C3PeripheralTraits;

#endif

  template<>
  struct PeripheralTraits<PERIPHERAL_I2S2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI2_BASE,
      GPIO_AF = GPIO_AF_SPI2
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2S2> I2S2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_I2S3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI3_BASE,
      GPIO_AF = GPIO_AF_SPI3
    };
  };
  typedef PeripheralTraits<PERIPHERAL_I2S3> I2S3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI1_BASE,
      GPIO_AF = GPIO_AF_SPI1
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI1> Spi1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI2_BASE,
      GPIO_AF = GPIO_AF_SPI2
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI2> Spi2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_SPI3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = SPI3_BASE,
      GPIO_AF = GPIO_AF_SPI3
    };
  };
  typedef PeripheralTraits<PERIPHERAL_SPI3> Spi3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM1_BASE,
      GPIO_AF = GPIO_AF_TIM1
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER1> Timer1PeripheralTraits;
  typedef PeripheralTraits<PERIPHERAL_TIMER1> Timer1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM2_BASE,
      GPIO_AF = GPIO_AF_TIM2
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER2> Timer2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM3_BASE,
      GPIO_AF = GPIO_AF_TIM3
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER3> Timer3PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER4> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM4_BASE,
      GPIO_AF = GPIO_AF_TIM4
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER4> Timer4PeripheralTraits;

#if defined(TIM5_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER5> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM5_BASE,
      GPIO_AF = GPIO_AF_TIM5
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER5> Timer5PeripheralTraits;
#endif

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER6> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM6_BASE,
      GPIO_AF = 0
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER6> Timer6PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER7> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM7_BASE,
      GPIO_AF = 0
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER7> Timer7PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER8> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM8_BASE,
      GPIO_AF = GPIO_AF_TIM8
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER8> Timer8PeripheralTraits;

#if defined(TIM9_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER9> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM9_BASE,
      GPIO_AF = GPIO_AF_TIM9
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER9> Timer9PeripheralTraits;
#endif

#if defined(TIM10_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER10> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM10_BASE,
      GPIO_AF = GPIO_AF_TIM10
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER10> Timer10PeripheralTraits;
#endif

#if defined(TIM11_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER11> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM11_BASE,
      GPIO_AF = GPIO_AF_TIM11
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER11> Timer11PeripheralTraits;
#endif

#if defined(TIM12_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER12> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM12_BASE,
      GPIO_AF = GPIO_AF_TIM12
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER12> Timer12PeripheralTraits;
#endif

#if defined(TIM13_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER13> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM13_BASE,
      GPIO_AF = GPIO_AF_TIM13
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER13> Timer13PeripheralTraits;
#endif

#if defined(TIM14_BASE)
  template<>
  struct PeripheralTraits<PERIPHERAL_TIMER14> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = TIM14_BASE,
      GPIO_AF = GPIO_AF_TIM14
    };
  };
  typedef PeripheralTraits<PERIPHERAL_TIMER14> Timer14PeripheralTraits;
#endif

  template<>
  struct PeripheralTraits<PERIPHERAL_USART1> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART1_BASE,
      GPIO_AF = GPIO_AF_USART1
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART1> Usart1PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART2> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART2_BASE,
      GPIO_AF = GPIO_AF_USART2
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART2> Usart2PeripheralTraits;

  template<>
  struct PeripheralTraits<PERIPHERAL_USART3> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART3_BASE,
      GPIO_AF = GPIO_AF_USART3
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART3> Usart3PeripheralTraits;

#if defined(GPIO_AF_UART4)
  template<>
  struct PeripheralTraits<PERIPHERAL_UART4> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = UART4_BASE,
      GPIO_AF = GPIO_AF_UART4
    };
  };
  typedef PeripheralTraits<PERIPHERAL_UART4> Uart4PeripheralTraits;
#endif

#if defined(GPIO_AF_UART5)
  template<>
  struct PeripheralTraits<PERIPHERAL_UART5> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = UART5_BASE,
      GPIO_AF = GPIO_AF_UART5
    };
  };
  typedef PeripheralTraits<PERIPHERAL_UART5> Uart5PeripheralTraits;
#endif

#if defined(STM32PLUS_F4) || defined(STM32PLUS_F1_CL)

  template<>
  struct PeripheralTraits<PERIPHERAL_MAC> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = ETH_BASE,
      GPIO_AF = GPIO_AF_ETH
    };
  };
  typedef PeripheralTraits<PERIPHERAL_MAC> MacPeripheralTraits;

  #endif

#if defined(STM32PLUS_F4)

  template<>
  struct PeripheralTraits<PERIPHERAL_USART6> {
    enum {
      GPIO_SPEED = GPIO_Speed_50MHz,
      PERIPHERAL_BASE = USART6_BASE,
      GPIO_AF = GPIO_AF_USART6
    };
  };
  typedef PeripheralTraits<PERIPHERAL_USART6> Usart6PeripheralTraits;

#endif

}
