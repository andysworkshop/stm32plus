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
   * Dma 1, channel 3 interrupt feature
   */

  typedef DmaInterruptFeature<3> Dma1Channel3InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel3InterruptFeature Spi1TxDmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Usart1RxDmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature I2C1RxDmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer1Channel2DmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer2Channel2DmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer3Channel4DmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer3UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer6UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Dac1UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer16Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel3InterruptFeature Timer16UpdateDmaChannelInterruptFeature;
}
