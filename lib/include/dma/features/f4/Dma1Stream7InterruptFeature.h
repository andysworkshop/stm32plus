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
   * Dma 1, stream 7 interrupt feature
   */

  typedef DmaInterruptFeature<1,7> Dma1Stream7InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma1Stream7InterruptFeature I2C2TxDmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature Timer3Channel3DmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature Uart5TxDmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature Timer2UpdateStream7DmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature Timer2Channel4Stream7DmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature Timer4Channel3DmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature I2C1TxStream7DmaChannelInterruptFeature;
  typedef Dma1Stream7InterruptFeature Spi3TxStream7DmaChannelInterruptFeature;
}
