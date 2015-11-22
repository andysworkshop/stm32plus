/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * ADC2
   */

  template<>
  struct ClockControl<PERIPHERAL_ADC2> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,DISABLE);
    }
  };


  /**
   * ADC3
   */

  template<>
  struct ClockControl<PERIPHERAL_ADC3> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,DISABLE);
    }
  };


  /*
   * CAN1
   */

  template<>
  struct ClockControl<PERIPHERAL_CAN1> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,DISABLE);
    }
  };


  /*
   * CAN2
   */

  template<>
  struct ClockControl<PERIPHERAL_CAN2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,DISABLE);
    }
  };


  /*
   * DAC2
   */

  template<>
  struct ClockControl<PERIPHERAL_DAC2> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,DISABLE);
    }
  };


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

#if !defined(STM32PLUS_F1_CL) && !defined(STM32PLUS_F1_MD_VL)

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
   * SPI3
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI3> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,DISABLE);
    }
  };

  /*
   * TIM4
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER4> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,DISABLE);
    }
  };

  /*
   * TIM5
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER5> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE);
    }
  };

  /*
   * TIM7
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER7> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,DISABLE);
    }
  };

  /*
   * TIM8
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER8> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,DISABLE);
    }
  };

  /*
   * TIM9
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER9> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,DISABLE);
    }
  };

  /*
   * TIM10
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER10> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,DISABLE);
    }
  };

  /*
   * TIM11
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER11> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,DISABLE);
    }
  };

  /*
   * TIM12
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER12> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,DISABLE);
    }
  };

  /*
   * TIM13
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER13> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,DISABLE);
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

#if !defined(STM32PLUS_F1_CL) && !defined(STM32PLUS_F1_MD_VL)

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
