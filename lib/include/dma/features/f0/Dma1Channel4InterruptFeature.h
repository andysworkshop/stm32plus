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
   * Dma 1, channel 4 interrupt feature
   */

  typedef DmaInterruptFeature<4> Dma1Channel4InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel4InterruptFeature Spi2RxDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Usart1TxRemapDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature I2C2TxDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Usart2TxDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer1Channel4DmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer1TriggerDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer1ComDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer2Channel4DmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer3Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer3TriggerDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer16Channel1RemapDmaChannelInterruptFeature;
  typedef Dma1Channel4InterruptFeature Timer16UpdateRemapDmaChannelInterruptFeature;
}
