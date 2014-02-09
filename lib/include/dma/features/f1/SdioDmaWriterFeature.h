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
   * DMA feature to enable reading via DMA
   */

  template<uint32_t TPriority=DMA_Priority_High>
  class SdioDmaWriterFeature : public DmaFeatureBase {

    public:
      SdioDmaWriterFeature(Dma& dma);
      void beginWrite(const void *src,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<uint32_t TPriority>
  inline SdioDmaWriterFeature<TPriority>::SdioDmaWriterFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    // enable what we can

    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&SDIO->FIFO);
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;
    _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
    _init.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
    _init.DMA_Mode=DMA_Mode_Normal;
    _init.DMA_Priority=TPriority;
    _init.DMA_M2M=DMA_M2M_Disable;
  }


  /**
   * Start a transfer of data to the peripheral
   *
   * @param[in] src The source of the transfer.
   * @param[in] count The number of bytes to transfer. Must be a block multiple
   */

  template<uint32_t TPriority>
  inline void SdioDmaWriterFeature<TPriority>::beginWrite(const void *src,uint32_t count) {

    DMA_Channel_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_MemoryBaseAddr=reinterpret_cast<uint32_t>(src);
    _init.DMA_BufferSize=count/4;
    _init.DMA_DIR=DMA_DIR_PeripheralDST;

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

    // disable and then re-enable

    SDIO_DMACmd(ENABLE);

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
