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
   * Dma 1, stream 5 interrupt feature
   */

  typedef DmaInterruptFeature<1,5> Dma1Stream5InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma1Stream5InterruptFeature Dac1DmaChannelInterruptFeature;
  typedef Dma1Stream5InterruptFeature Timer3Channel2DmaChannelInterruptFeature;
  typedef Dma1Stream5InterruptFeature Usart2RxDmaChannelInterruptFeature;
  typedef Dma1Stream5InterruptFeature Timer2Channel1DmaChannelInterruptFeature;
  typedef Dma1Stream5InterruptFeature I2S3ExtTxDmaChannelInterruptFeature;
  typedef Dma1Stream5InterruptFeature I2C1RxStream5DmaChannelInterruptFeature;
  typedef Dma1Stream5InterruptFeature Spi3TxDmaChannelInterruptFeature;
}
