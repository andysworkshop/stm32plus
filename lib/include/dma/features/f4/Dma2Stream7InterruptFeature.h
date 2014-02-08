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
   * Dma 2, stream 7 interrupt feature
   */

  typedef DmaInterruptFeature<2,7> Dma2Stream7InterruptFeature;

  /**
   * Types for the peripherals mapped to this channel
   */

  typedef Dma2Stream7InterruptFeature Timer8Channel4DmaChannelInterruptFeature;
  typedef Dma2Stream7InterruptFeature Timer8TriggerDmaChannelInterruptFeature;
  typedef Dma2Stream7InterruptFeature Timer8ComDmaChannelInterruptFeature;
  typedef Dma2Stream7InterruptFeature Usart6TxStream7DmaChannelInterruptFeature;
  typedef Dma2Stream7InterruptFeature Usart1TxDmaChannelInterruptFeature;
  typedef Dma2Stream7InterruptFeature HashInDmaChannelInterruptFeature;
  typedef Dma2Stream7InterruptFeature DcmiStream7DmaChannelInterruptFeature;
}
