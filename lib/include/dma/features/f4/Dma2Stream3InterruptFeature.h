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
   * Dma 2, stream 3 interrupt feature
   */

  typedef DmaInterruptFeature<2,3> Dma2Stream3InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream3InterruptFeature Timer8Channel2Stream3DmaChannelInterruptFeature;
  typedef Dma2Stream3InterruptFeature Timer1Channel1Stream3DmaChannelInterruptFeature;
  typedef Dma2Stream3InterruptFeature SdioDmaChannelInterruptFeature;
  typedef Dma2Stream3InterruptFeature Spi1TxDmaChannelInterruptFeature;
  typedef Dma2Stream3InterruptFeature Adc2Stream3DmaChannelInterruptFeature;
}
