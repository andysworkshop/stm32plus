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
   * Dma 1, channel 1 interrupt feature
   */

  typedef DmaInterruptFeature<1,1> Dma1Channel1InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma1Channel1InterruptFeature Adc1DmaChannelInterruptFeature;
  typedef Dma1Channel1InterruptFeature Timer2Channel3DmaChannelInterruptFeature;
  typedef Dma1Channel1InterruptFeature Timer4Channel1DmaChannelInterruptFeature;
}
