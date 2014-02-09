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
   * Dma 2, channel 1 interrupt feature
   */

  typedef DmaInterruptFeature<2,1> Dma2Channel1InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma2Channel1InterruptFeature Timer5Channel4DmaChannelInterruptFeature;
  typedef Dma2Channel1InterruptFeature Timer5TriggerDmaChannelInterruptFeature;
  typedef Dma2Channel1InterruptFeature Timer8Channel3DmaChannelInterruptFeature;
  typedef Dma2Channel1InterruptFeature Timer8UpdateDmaChannelInterruptFeature;
  typedef Dma2Channel1InterruptFeature Spi3RxDmaChannelInterruptFeature;
}
