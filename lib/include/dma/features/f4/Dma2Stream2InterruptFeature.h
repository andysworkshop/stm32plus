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
   * Dma 2, stream 2 interrupt feature
   */

  typedef DmaInterruptFeature<2,2> Dma2Stream2InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream2InterruptFeature Timer8Channel1Stream2DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Timer1Channel2DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Usart6RxStream2DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Usart1RxDmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Spi1RxStream2DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Adc2DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Timer8Channel3DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Timer8Channel2DmaChannelInterruptFeature;
  typedef Dma2Stream2InterruptFeature Timer8Channel1DmaChannelInterruptFeature;
}
