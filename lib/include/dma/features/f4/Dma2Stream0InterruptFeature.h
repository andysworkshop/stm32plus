/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Dma 2, stream 0 interrupt feature
   */

  typedef DmaInterruptFeature<2,0> Dma2Stream0InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream0InterruptFeature Timer1TriggerDmaChannelInterruptFeature;
  typedef Dma2Stream0InterruptFeature Spi1RxDmaChannelInterruptFeature;
  typedef Dma2Stream0InterruptFeature Adc3DmaChannelInterruptFeature;
  typedef Dma2Stream0InterruptFeature Adc1DmaChannelInterruptFeature;
}
