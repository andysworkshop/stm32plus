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
   * Dma 1, channel 5 interrupt feature
   */

  typedef DmaInterruptFeature<5> Dma1Channel5InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel5InterruptFeature Spi2TxDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Usart1RxRemapDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature I2C2RxDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Usart2RxDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer1Channel3DmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer1UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer2Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer15Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer15UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer15TriggerDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer15ComDmaChannelInterruptFeature;
}
