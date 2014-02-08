/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * Dma1, channel1. The template parameters can be used to specify a list
   * of features that you want to use.
   */

  template<class... Features>
  class Dma2Channel5 : public Dma,
                       public Features... {

    public:
      typedef Dma2Channel5<Features...> DmaType;

      /**
       * Constructor
       */

      Dma2Channel5()
        : Dma(DMA2_Channel5,DMA2_FLAG_TC5,DMA2_FLAG_HT5,DMA2_FLAG_TE5),
          Features(static_cast<Dma&>(*this))... {

        ClockControl<PERIPHERAL_DMA2>::On();
      }
  };

  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Adc3DmaChannel=Dma2Channel5<Features...>;
  template<class... Features> using Timer8Channel2DmaChannel=Dma2Channel5<Features...>;
  template<class... Features> using Timer5Channel1DmaChannel=Dma2Channel5<Features...>;
  template<class... Features> using Uart4TxDmaChannel=Dma2Channel5<Features...>;
}
