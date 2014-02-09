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
  class Dma1Channel2 : public Dma,
                       public Features... {

    public:
      typedef Dma1Channel2<Features...> DmaType;

      /**
       * Constructor
       */

      Dma1Channel2()
        : Dma(DMA1_Channel2,DMA1_FLAG_TC2,DMA1_FLAG_HT2,DMA1_FLAG_TE2),
          Features(static_cast<Dma&>(*this))... {

        ClockControl<PERIPHERAL_DMA1>::On();
      }
  };

  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Adc1RemapDmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Spi1RxDmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Usart1TxDmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using I2C1TxDmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Timer1Channel1DmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Timer2UpdateDmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Timer3Channel3DmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Timer17Channel1RemapDmaChannel=Dma1Channel2<Features...>;
  template<class... Features> using Timer17UpdateRemapDmaChannel=Dma1Channel2<Features...>;
}
