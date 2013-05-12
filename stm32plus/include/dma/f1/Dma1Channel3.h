/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * Dma1, channel1. The template parameters can be used to specify a
   * list of features that you want to use.
   */

  template<class... Features>
  class Dma1Channel3 : public Dma,
                 	 	 	 public Features... {

    public:
			typedef Dma1Channel3<Features...> DmaType;

			/**
			 * Constructor
			 */

			Dma1Channel3()
				: Dma(DMA1_Channel3,DMA1_FLAG_TC3,DMA1_FLAG_HT3,DMA1_FLAG_TE3),
				  Features((Dma&)*this)... {

				ClockControl<PERIPHERAL_DMA1>::On();
			}
  };

  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Usart3RxDmaChannel=Dma1Channel3<Features...>;
  template<class... Features> using Timer1Channel2DmaChannel=Dma1Channel3<Features...>;
  template<class... Features> using Timer3Channel4DmaChannel=Dma1Channel3<Features...>;
  template<class... Features> using Timer3UpdateDmaChannel=Dma1Channel3<Features...>;
  template<class... Features> using Spi1TxDmaChannel=Dma1Channel3<Features...>;
}
