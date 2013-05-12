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
  class Dma2Channel0Stream2 : public Dma,
                 	 	 	 	 	 	 	public Features... {

    public:

  		/**
			 * Constructor
			 */

			Dma2Channel0Stream2()
				: Dma(DMA2_Stream2,DMA_Channel_0,DMA_FLAG_TCIF2,DMA_FLAG_HTIF2,DMA_FLAG_TEIF2),
				  Features((Dma&)*this)... {

				ClockControl<PERIPHERAL_DMA2>::On();
			}
  };


  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Timer8Channel1DmaChannel=Dma2Channel0Stream2<Features...>;
  template<class... Features> using Timer8Channel2DmaChannel=Dma2Channel0Stream2<Features...>;
  template<class... Features> using Timer8Channel3DmaChannel=Dma2Channel0Stream2<Features...>;
}
