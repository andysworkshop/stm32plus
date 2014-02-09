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
  class Dma1Channel7 : public Dma,
                       public Features... {

    public:
      typedef Dma1Channel7<Features...> DmaType;

      /**
       * Constructor
       */

      Dma1Channel7()
        : Dma(DMA1_Channel7,DMA1_FLAG_TC7,DMA1_FLAG_HT7,DMA1_FLAG_TE7),
          Features(static_cast<Dma&>(*this))... {

        ClockControl<PERIPHERAL_DMA1>::On();
      }
  };

  /**
   * Types for the peripherals mapped to this channel
   */

  template<class... Features> using Usart2TxDmaChannel=Dma1Channel7<Features...>;
  template<class... Features> using Timer2Channel2DmaChannel=Dma1Channel7<Features...>;
  template<class... Features> using Timer2Channel4DmaChannel=Dma1Channel7<Features...>;
  template<class... Features> using Timer4UpdateDmaChannel=Dma1Channel7<Features...>;
  template<class... Features> using I2C1RxDmaChannel=Dma1Channel7<Features...>;
}
