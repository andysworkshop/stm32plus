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
   * Dma 1, channel 5 interrupt feature
   */

  typedef DmaInterruptFeature<1,5> Dma1Channel5InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel5InterruptFeature Usart1RxDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer1UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Spi2TxDmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer2Channel1DmaChannelInterruptFeature;
  typedef Dma1Channel5InterruptFeature Timer4Channel3DmaChannelInterruptFeature;
}
