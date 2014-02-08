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
   * Dma 1, stream 6 interrupt feature
   */

  typedef DmaInterruptFeature<1,6> Dma1Stream6InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma1Stream6InterruptFeature Dac2DmaChannelInterruptFeature;
  typedef Dma1Stream6InterruptFeature Timer5UpdateStream6DmaChannelInterruptFeature;
  typedef Dma1Stream6InterruptFeature Usart2TxDmaChannelInterruptFeature;
  typedef Dma1Stream6InterruptFeature Timer2Channel2DmaChannelInterruptFeature;
  typedef Dma1Stream6InterruptFeature Timer2Channel4DmaChannelInterruptFeature;
  typedef Dma1Stream6InterruptFeature Timer4UpdateDmaChannelInterruptFeature;
  typedef Dma1Stream6InterruptFeature I2C1TxDmaChannelInterruptFeature;
}
