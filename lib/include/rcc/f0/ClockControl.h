/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

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
   * POWER
   */

  template<>
  struct ClockControl<PERIPHERAL_POWER> {

    static void On() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
    }

    static void Off() {
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,DISABLE);
    }
  };


  /**
   * SPI1
   */

  template<>
  struct ClockControl<PERIPHERAL_SPI1> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,DISABLE);
    }
  };


  /*
   * TIM15
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER15> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,DISABLE);
    }
  };


  /*
   * TIM16
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER16> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,DISABLE);
    }
  };


  /*
   * TIM17
   */

  template<>
  struct ClockControl<PERIPHERAL_TIMER17> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,ENABLE);
    }

    static void Off() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17,DISABLE);
    }
  };


  /**
   * USART1
   */

  template<>
  struct ClockControl<PERIPHERAL_USART1> {

    static void On() {
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
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
}
