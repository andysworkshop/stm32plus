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
   * Dma 1, channel 6 interrupt feature
   */

  typedef DmaInterruptFeature<1,6> Dma1Channel6InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel6InterruptFeature Usart2RxDmaChannelInterruptFeature;
  typedef Dma1Channel6InterruptFeature Timer1Channel3DmaChannelInterruptFeature;
  typedef Dma1Channel6InterruptFeature Timer3Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel6InterruptFeature Timer3TriggerDmaChannelInterruptFeature;
  typedef Dma1Channel6InterruptFeature I2C1TxDmaChannelInterruptFeature;
}
