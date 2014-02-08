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
   * Dma 1, channel 4 interrupt feature
   */

  typedef DmaInterruptFeature<1,4> Dma1Channel4InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel4InterruptFeature Usart1TxDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer1Channel4DmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer1TriggerDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer1ComDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer4Channel2DmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Spi2RxDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature I2C2TxDmaChannelInterruptFeature;
}
