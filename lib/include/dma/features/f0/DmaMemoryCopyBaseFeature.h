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
   * Dma feature to enable copying from memory to memory. Example declaration:
   *
   * @tparam TSourceInc DMA_PeripheralInc_Enable/DMA_PeripheralInc_Disable
   *
   * Dma1Channel1<DmaMemoryCopyBaseFeature<> > dmaMemoryCopier;
   */

  template<uint32_t TSourceInc,uint32_t TDataSize>
  class DmaMemoryCopyBaseFeature : public DmaFeatureBase {

    public:
      DmaMemoryCopyBaseFeature(Dma& dma);
      void beginCopyMemory(void *dest,void *source,uint32_t count,uint32_t priority);
  };


  /**
   * Constructor
   */

  template<uint32_t TSourceInc,uint32_t TDataSize>
  inline DmaMemoryCopyBaseFeature<TSourceInc,TDataSize>::DmaMemoryCopyBaseFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    uint32_t periphDataSize;

    DMA_StructInit(&_init);

    if(TDataSize==DMA_MemoryDataSize_Byte)
      periphDataSize=DMA_PeripheralDataSize_Byte;
    else if(TDataSize==DMA_MemoryDataSize_HalfWord)
      periphDataSize=DMA_PeripheralDataSize_HalfWord;
    else
      periphDataSize=DMA_PeripheralDataSize_Word;

    _init.DMA_DIR=DMA_DIR_PeripheralSRC;              // 'peripheral' is source
    _init.DMA_PeripheralInc=TSourceInc;               // 'peripheral' gets incremented for copy, not for fill.
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;         // memory is incremented
    _init.DMA_PeripheralDataSize=periphDataSize;      // user defined word size
    _init.DMA_MemoryDataSize=TDataSize;               // user defined word size
    _init.DMA_Mode=DMA_Mode_Normal;                   // not a circular buffer
    _init.DMA_M2M=DMA_M2M_Enable;                     // memory->memory configuration
  }


  /**
   * Start a transfer of data from source to the dest. The count is in units
   * of the word size.
   *
   * @param[in] dest The destination of the transfer.
   * @param[in] source The source of the transfer.
   * @param[in] count The number of words to transfer.
   */

  template<uint32_t TSourceInc,uint32_t TDataSize>
  inline void DmaMemoryCopyBaseFeature<TSourceInc,TDataSize>::beginCopyMemory(void *dest,void *source,uint32_t count,uint32_t priority) {

    DMA_Channel_TypeDef *peripheralAddress;

    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(source);
    _init.DMA_MemoryBaseAddr=reinterpret_cast<uint32_t>(dest);
    _init.DMA_BufferSize=count;
    _init.DMA_Priority=priority;                              // user-configurable priority

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
