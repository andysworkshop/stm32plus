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
   * Dma 2, channel 2 interrupt feature
   */

  typedef DmaInterruptFeature<2,2> Dma2Channel2InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma2Channel2InterruptFeature Timer8Channel4DmaChannelInterruptFeature;
  typedef Dma2Channel2InterruptFeature Timer8TriggerDmaChannelInterruptFeature;
  typedef Dma2Channel2InterruptFeature Timer8ComDmaChannelInterruptFeature;
  typedef Dma2Channel2InterruptFeature Timer5Channel3DmaChannelInterruptFeature;
  typedef Dma2Channel2InterruptFeature Timer5UpdateDmaChannelInterruptFeature;
  typedef Dma2Channel2InterruptFeature Spi3TxDmaChannelInterruptFeature;
}
