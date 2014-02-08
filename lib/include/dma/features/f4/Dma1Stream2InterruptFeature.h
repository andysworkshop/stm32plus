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
   * Dma 1, stream 2 interrupt feature
   */

  typedef DmaInterruptFeature<1,2> Dma1Stream2InterruptFeature;

  /**
   * Typedefs for the peripherals that use this interrupt
   */

  typedef Dma1Stream2InterruptFeature I2C2RxDmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature Timer5Channel1DmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature Timer3Channel4DmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature Timer3UpdateDmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature Uart4RxDmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature I2C3RxDmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature I2S2ExtRxDmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature Timer7UpdateDmaChannelInterruptFeature;
  typedef Dma1Stream2InterruptFeature Spi3RxStream2DmaChannelInterruptFeature;
}
