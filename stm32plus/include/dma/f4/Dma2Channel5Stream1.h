/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * Feature collection for this DMA channel. Parameterise this class with the features
   * that you want to use on this channel.
   */

  template<class... Features>
  class Dma2Channel5Stream1 : public Dma,
                 	 	 	 	 	 	 	public Features... {

    public:

  		/**
			 * Constructor
			 */

			Dma2Channel5Stream1()
				: Dma(DMA2_Stream1,DMA_Channel_5,DMA_FLAG_TCIF1,DMA_FLAG_HTIF1,DMA_FLAG_TEIF1),
				  Features((Dma&)*this)... {

				ClockControl<PERIPHERAL_DMA2>::On();
			}
  };


  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Usart6RxDmaChannel=Dma2Channel5Stream1<Features...>;
}
