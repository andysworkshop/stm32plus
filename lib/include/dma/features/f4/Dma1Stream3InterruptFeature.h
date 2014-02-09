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

  typedef DmaInterruptFeature<1,3> Dma1Stream3InterruptFeature;

  /**
   * Typedefs for the peripherals that use this interrupt
   */

  typedef Dma1Stream3InterruptFeature I2C2RxStream3DmaChannelInterruptFeature;
  typedef Dma1Stream3InterruptFeature Timer5Channel4Stream3DmaChannelInterruptFeature;
  typedef Dma1Stream3InterruptFeature Timer5TriggerStream3DmaChannelInterruptFeature;
  typedef Dma1Stream3InterruptFeature Usart3TxDmaChannelInterruptFeature;
  typedef Dma1Stream3InterruptFeature I2S2ExtRxStream3DmaChannelInterruptFeature;
  typedef Dma1Stream3InterruptFeature Timer4Channel2DmaChannelInterruptFeature;
  typedef Dma1Stream3InterruptFeature Spi2RxDmaChannelInterruptFeature;
}
