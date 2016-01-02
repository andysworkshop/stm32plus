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
   * DMA feature to enable reading via DMA
   * @tparam TUsart The type of the usart peripheral (Usart1<...>, Usart2<...>)
   */

  template<class TUsart,uint32_t TPriority=DMA_Priority_High,uint32_t TMode=DMA_Mode_Normal>
  class UsartDmaWriterFeature  : public DmaFeatureBase {

    public:
      UsartDmaWriterFeature(Dma& dma);
      void beginWrite(const void *source,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<class TUsart,uint32_t TPriority,uint32_t TMode>
  inline UsartDmaWriterFeature<TUsart,TPriority,TMode>::UsartDmaWriterFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    USART_TypeDef *usart;

    usart=(USART_TypeDef *)TUsart::PERIPHERAL_BASE;

    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(usart->TDR));
    _init.DMA_DIR=DMA_DIR_PeripheralDST;                      // 'peripheral' is destination
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;        // 'peripheral' does not increment
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;                 // memory is incremented
    _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte; // transferring bytes
    _init.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;         // user defined word size
    _init.DMA_Mode=TMode;                                     // DMA_Mode_Normal/DMA_Mode_Circular
    _init.DMA_Priority=TPriority;                             // user-configurable priority
    _init.DMA_M2M=DMA_M2M_Disable;                            // memory->peripheral configuration

    USART_DMACmd(usart,USART_DMAReq_Tx,ENABLE);
  }


  /**
   * Start a transfer of data to the USART
   *
   * @param[in] source memory address of the source data bytes.
   * @param[in] count The number of bytes to transfer.
   */

  template<class TUsart,uint32_t TPriority,uint32_t TMode>
  inline void UsartDmaWriterFeature<TUsart,TPriority,TMode>::beginWrite(const void *source,uint32_t count) {

    DMA_Channel_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_MemoryBaseAddr=reinterpret_cast<uint32_t>(source);
    _init.DMA_BufferSize=count;

    // set the peripheral address from the overloaded operator

    peripheralAddress=_dma;

      // disable and then re-enable

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
