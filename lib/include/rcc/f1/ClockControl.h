/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * BACKUP
   */

  template<>
  struct ClockControl<PERIPHERAL_BACKUP> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,DISABLE);
    }
  };

  /**
   * CRC
   */

  template<>
  struct ClockControl<PERIPHERAL_CRC> {

    static void On() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC,DISABLE);
    }
  };

  /**
   * DMA1
   */

  template<>
  struct ClockControl<PERIPHERAL_DMA1> {

    static void On() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,DISABLE);
    }
  };

  /**
   * DMA2
   */

  template<>
  struct ClockControl<PERIPHERAL_DMA2> {

    static void On() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,DISABLE);
    }
  };

#if !defined(STM32PLUS_F1_CL)     // the 105/107 do not have the FSMC

  /**
   * FSMC
   */

  template<>
  struct ClockControl<PERIPHERAL_FSMC> {

    static void On() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);
      RCC_AHBPeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,DISABLE);
    }
  };

#endif

  /**
   * I2S2
   */

  template<>
  struct ClockControl<PERIPHERAL_I2S2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,DISABLE);
    }
  };

  /**
   * I2S3
   */

  template<>
  struct ClockControl<PERIPHERAL_I2S3> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
    }
  };

  /**
   * POWER
   */

  template<>
  struct ClockControl<PERIPHERAL_POWER> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,DISABLE);
    }
  };

  /**
   * SPI1
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI1> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
    }
  };

  /**
   * SPI1_REMAP
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI1_REMAP> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
    }
  };

  /**
   * SPI3_REMAP
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI3_REMAP> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
    }
  };

  /**
   * USART1
   */

  template<>
  struct ClockControl<PERIPHERAL_USART1> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);
    }
  };

  /**
   * USART1
   */

  template<>
  struct ClockControl<PERIPHERAL_USART1_REMAP> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,DISABLE);
    }
  };

  /**
   * USART2
   */

  template<>
  struct ClockControl<PERIPHERAL_USART2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,DISABLE);
    }
  };

  /**
   * USART2
   */

  template<>
  struct ClockControl<PERIPHERAL_USART2_REMAP> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,DISABLE);
    }
  };

  /**
   * USART3
   */

  template<>
  struct ClockControl<PERIPHERAL_USART3> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);
    }
  };

  /**
   * USART3 full remap
   */

  template<>
  struct ClockControl<PERIPHERAL_USART3_FULL_REMAP> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
      GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);
    }
  };

  /**
   * USART3 partial remap
   */

  template<>
  struct ClockControl<PERIPHERAL_USART3_PARTIAL_REMAP> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
      GPIO_PinRemapConfig(GPIO_PartialRemap_USART3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,DISABLE);
    }
  };

#if !defined(STM32PLUS_F1_CL)     // the 105/107 do not have SDIO

  /**
   * SDIO
   */

  template<>
  struct ClockControl<PERIPHERAL_SDIO> {

    static void On() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO,ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO,DISABLE);
    }
  };

#endif


  /**
   * UART4
   */

  template<>
  struct ClockControl<PERIPHERAL_UART4> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,DISABLE);
    }
  };

  /**
   * UART5
   */

  template<>
  struct ClockControl<PERIPHERAL_UART5> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,DISABLE);
    }
  };


#if defined(STM32PLUS_F1_CL_E)

  /*
   * Ethernet MAC
   */

  template<>
  struct ClockControl<PERIPHERAL_MAC> {

    // configure Tx, Rx

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx | RCC_AHBPeriph_ETH_MAC_Rx,ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx | RCC_AHBPeriph_ETH_MAC_Rx,DISABLE);
    }
  };

  template<>
  struct ClockControl<PERIPHERAL_MAC_REMAP> {

    // configure Tx, Rx

    static void On() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx | RCC_AHBPeriph_ETH_MAC_Rx,ENABLE);
      GPIO_PinRemapConfig(GPIO_Remap_ETH, ENABLE);
    }

    static void Off() {
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC | RCC_AHBPeriph_ETH_MAC_Tx | RCC_AHBPeriph_ETH_MAC_Rx,DISABLE);
    }
  };

#endif

}
