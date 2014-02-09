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
   * Dma1, channel1. The template parameters can be used to specify a
   * list of features that you want to use.
   */

  template<class... Features>
  class Dma2Channel1 : public Dma,
                       public Features... {

    public:
      typedef Dma2Channel1<Features...> DmaType;

      /**
       * Constructor
       */

      Dma2Channel1()
        : Dma(DMA2_Channel1,DMA2_FLAG_TC1,DMA2_FLAG_HT1,DMA2_FLAG_TE1),
          Features(static_cast<Dma&>(*this))... {

        ClockControl<PERIPHERAL_DMA2>::On();
      }
  };

  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Timer5Channel4DmaChannel=Dma2Channel1<Features...>;
  template<class... Features> using Timer5TriggerDmaChannel=Dma2Channel1<Features...>;
  template<class... Features> using Timer8Channel3DmaChannel=Dma2Channel1<Features...>;
  template<class... Features> using Timer8UpdateDmaChannel=Dma2Channel1<Features...>;
  template<class... Features> using Spi3RxDmaChannel=Dma2Channel1<Features...>;
  template<class... Features> using I2S3RxDmaChannel=Dma2Channel1<Features...>;
}
