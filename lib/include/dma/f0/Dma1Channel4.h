/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {

  /**
   * Dma1, channel1. The template parameters can be used to specify a
   * list of features that you want to use.
   */

  template<class... Features>
  class Dma1Channel4 : public Dma,
                       public Features... {

    public:
      typedef Dma1Channel4<Features...> DmaType;

      /**
       * Constructor
       */

      Dma1Channel4()
        : Dma(DMA1_Channel4,DMA1_FLAG_TC4,DMA1_FLAG_HT4,DMA1_FLAG_TE4),
          Features(static_cast<Dma&>(*this))... {

        ClockControl<PERIPHERAL_DMA1>::On();
      }
  };


  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Spi2RxDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Usart1TxRemapDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using I2C2TxDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Usart2TxDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer1Channel4DmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer1TriggerDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer1ComDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer2Channel4DmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer3Channel1DmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer3TriggerDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer16Channel1RemapDmaChannel=Dma1Channel4<Features...>;
  template<class... Features> using Timer16UpdateRemapDmaChannel=Dma1Channel4<Features...>;
}
