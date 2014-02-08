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
   * Dma 1, stream 1 interrupt feature
   */

  typedef DmaInterruptFeature<1,1> Dma1Stream1InterruptFeature;

  /**
   * Typedefs for the peripherals that use this interrupt
   */

  typedef Dma1Stream1InterruptFeature Timer6UpdateDmaChannelInterruptFeature;
  typedef Dma1Stream1InterruptFeature Timer5Channel4DmaChannelInterruptFeature;
  typedef Dma1Stream1InterruptFeature Timer5TriggerDmaChannelInterruptFeature;
  typedef Dma1Stream1InterruptFeature Usart3RxDmaChannelInterruptFeature;
  typedef Dma1Stream1InterruptFeature Timer2UpdateDmaChannelInterruptFeature;
  typedef Dma1Stream1InterruptFeature Timer2Channel3DmaChannelInterruptFeature;
}
