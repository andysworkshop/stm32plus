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
   * Dma 2, stream 6 interrupt feature
   */

  typedef DmaInterruptFeature<2,6> Dma2Stream6InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream6InterruptFeature Timer1Channel3Stream6DmaChannelInterruptFeature;
  typedef Dma2Stream6InterruptFeature Usart6TxDmaChannelInterruptFeature;
  typedef Dma2Stream6InterruptFeature SdioStream6DmaChannelInterruptFeature;
  typedef Dma2Stream6InterruptFeature CrypInDmaChannelInterruptFeature;
  typedef Dma2Stream6InterruptFeature Timer1Channel1Stream6DmaChannelInterruptFeature;
  typedef Dma2Stream6InterruptFeature Timer1Channel2Stream6DmaChannelInterruptFeature;
  typedef Dma2Stream6InterruptFeature Timer1Channel3DmaChannelInterruptFeature;
}
