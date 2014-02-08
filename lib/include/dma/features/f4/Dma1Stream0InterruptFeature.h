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
   * Dma 1, stream 0 interrupt feature
   */

  typedef DmaInterruptFeature<1,0> Dma1Stream0InterruptFeature;

  /**
   * Typedefs for the peripherals that use this interrupt
   */

  typedef Dma1Stream0InterruptFeature Spi3RxDmaChannelInterruptFeature;
  typedef Dma1Stream0InterruptFeature Timer5Channel3DmaChannelInterruptFeature;
  typedef Dma1Stream0InterruptFeature Timer5UpdateDmaChannelInterruptFeature;
  typedef Dma1Stream0InterruptFeature Uart5RxDmaChannelInterruptFeature;
  typedef Dma1Stream0InterruptFeature I2S3ExtRxDmaChannelInterruptFeature;
  typedef Dma1Stream0InterruptFeature Timer4Channel1DmaChannelInterruptFeature;
  typedef Dma1Stream0InterruptFeature I2C1RxDmaChannelInterruptFeature;
}
