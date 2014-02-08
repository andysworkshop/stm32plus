/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Dma 2, stream 5 interrupt feature
   */

  typedef DmaInterruptFeature<2,5> Dma2Stream5InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream5InterruptFeature Timer1UpdateDmaChannelInterruptFeature;
  typedef Dma2Stream5InterruptFeature Usart1RxStream5DmaChannelInterruptFeature;
  typedef Dma2Stream5InterruptFeature Spi1TxStream5DmaChannelInterruptFeature;
  typedef Dma2Stream5InterruptFeature CrypOutDmaChannelInterruptFeature;
}
