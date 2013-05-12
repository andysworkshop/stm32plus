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
  class Dma2Channel7Stream5 : public Dma,
                 	 	 	 	 	 	 	public Features... {

    public:

  		/**
			 * Constructor
			 */

			Dma2Channel7Stream5()
				: Dma(DMA2_Stream5,DMA_Channel_7,DMA_FLAG_TCIF5,DMA_FLAG_HTIF5,DMA_FLAG_TEIF5),
				  Features((Dma&)*this)... {

				ClockControl<PERIPHERAL_DMA2>::On();
			}
  };


}
