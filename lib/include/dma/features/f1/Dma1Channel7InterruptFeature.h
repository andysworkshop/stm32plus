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
   * Dma 1, channel 7 interrupt feature
   */

  typedef DmaInterruptFeature<1,7> Dma1Channel7InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel7InterruptFeature Usart2TxDmaChannelInterruptFeature;
  typedef Dma1Channel7InterruptFeature Timer2Channel2DmaChannelInterruptFeature;
  typedef Dma1Channel7InterruptFeature Timer2Channel4DmaChannelInterruptFeature;
  typedef Dma1Channel7InterruptFeature Timer4UpdateDmaChannelInterruptFeature;
  typedef Dma1Channel7InterruptFeature I2C1RxDmaChannelInterruptFeature;
}
