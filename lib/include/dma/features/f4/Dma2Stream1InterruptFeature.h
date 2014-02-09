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
   * Dma 2, stream 1 interrupt feature
   */

  typedef DmaInterruptFeature<2,1> Dma2Stream1InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream1InterruptFeature Timer8UpdateDmaChannelInterruptFeature;
  typedef Dma2Stream1InterruptFeature Timer1Channel1DmaChannelInterruptFeature;
  typedef Dma2Stream1InterruptFeature Usart6RxDmaChannelInterruptFeature;
  typedef Dma2Stream1InterruptFeature Adc3Stream1DmaChannelInterruptFeature;
  typedef Dma2Stream1InterruptFeature DcmiDmaChannelInterruptFeature;
}
