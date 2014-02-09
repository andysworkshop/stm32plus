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
   * Dma 2, channel 3 interrupt feature
   */

  typedef DmaInterruptFeature<2,3> Dma2Channel3InterruptFeature;

  // typedefs for the peripherals that use this channel

  typedef Dma2Channel3InterruptFeature Timer8Channel1DmaChannelInterruptFeature;
  typedef Dma2Channel3InterruptFeature Uart4RxDmaChannelInterruptFeature;
  typedef Dma2Channel3InterruptFeature Timer6UpdateDmaChannelInterruptFeature;
  typedef Dma2Channel3InterruptFeature DacChannel1DmaChannelInterruptFeature;
}
