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
   * Dma 2, stream 4 interrupt feature
   */

  typedef DmaInterruptFeature<2,4> Dma2Stream4InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream4InterruptFeature Timer8Channel3Stream4DmaChannelInterruptFeature;
  typedef Dma2Stream4InterruptFeature Timer1Channel4DmaChannelInterruptFeature;
  typedef Dma2Stream4InterruptFeature Timer1TriggerStream4DmaChannelInterruptFeature;
  typedef Dma2Stream4InterruptFeature Timer1ComDmaChannelInterruptFeature;
  typedef Dma2Stream4InterruptFeature Adc1Stream4DmaChannelInterruptFeature;
}
