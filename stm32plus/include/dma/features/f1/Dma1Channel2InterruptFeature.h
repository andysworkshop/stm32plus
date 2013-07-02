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
   * Dma 1, channel 2 interrupt feature
   */

  typedef DmaInterruptFeature<1,2> Dma1Channel2InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel2InterruptFeature Usart3TxDmaChannelInterruptFeature;
  typedef Dma1Channel2InterruptFeature Timer1Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel2InterruptFeature Timer2UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel2InterruptFeature Timer3Channel3DmaChannelInterruptFeature;
  typedef Dma1Channel2InterruptFeature Spi1RxDmaChannelInterruptFeature;
}
