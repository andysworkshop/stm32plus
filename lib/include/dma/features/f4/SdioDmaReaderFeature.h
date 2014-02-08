/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {

  /**
   * DMA feature to enable reading via DMA
   */

  template<uint32_t TPriority=DMA_Priority_High,uint32_t TFifoMode=DMA_FIFOMode_Enable>
  class SdioDmaReaderFeature : public DmaFeatureBase {

    public:
      SdioDmaReaderFeature(Dma& dma);
      void beginRead(void *dest,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<uint32_t TPriority,uint32_t TFifoMode>
  inline SdioDmaReaderFeature<TPriority,TFifoMode>::SdioDmaReaderFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    // enable what we can

    _init.DMA_Channel=dma.getChannelNumber();                 // channel id
    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&SDIO->FIFO);
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;
    _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
    _init.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
    _init.DMA_Mode=DMA_Mode_Normal;
    _init.DMA_Priority=TPriority;
    _init.DMA_MemoryBurst=DMA_MemoryBurst_INC4;               // burst size
    _init.DMA_PeripheralBurst=DMA_PeripheralBurst_INC4;       // burst size
    _init.DMA_FIFOMode=TFifoMode;                             // FIFO mode

    if(TFifoMode==DMA_FIFOMode_Enable)
      _init.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;         // flush on half-full
  }


  /**
   * Start a transfer of data to the destination
   *
   * @param[in] dest The destination of the transfer.
   * @param[in] count The number of bytes to transfer. Must be a block multiple
   */

  template<uint32_t TPriority,uint32_t TFifoMode>
  inline void SdioDmaReaderFeature<TPriority,TFifoMode>::beginRead(void *dest,uint32_t) {

    DMA_Stream_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_Memory0BaseAddr=reinterpret_cast<uint32_t>(dest);
    _init.DMA_BufferSize=0;                                 // hardware flow control instead
    _init.DMA_DIR=DMA_DIR_PeripheralToMemory;               // 'peripheral' is source

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

    // disable and then re-enable

    SDIO_DMACmd(ENABLE);

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_FlowControllerConfig(peripheralAddress,DMA_FlowCtrl_Peripheral);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
