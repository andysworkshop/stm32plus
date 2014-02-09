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
   * Dma 1, stream 4 interrupt feature
   */

  typedef DmaInterruptFeature<1,4> Dma1Stream4InterruptFeature;

  /**
   * Typedefs for the peripherals that use this interrupt
   */

  typedef Dma1Stream4InterruptFeature Usart3TxStream4DmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature Timer5Channel2DmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature Timer3Channel1DmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature Timer3TriggerDmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature Uart4TxDmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature I2C3TxDmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature I2S2ExtTxDmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature Timer7UpdateStream4DmaChannelInterruptFeature;
  typedef Dma1Stream4InterruptFeature Spi2TxDmaChannelInterruptFeature;
}
