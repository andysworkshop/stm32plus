/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {

  /**
   * DMA feature to enable ADC data to be written to memory
   * @tparam TAdc The type of the ADC peripheral (Adc1<...>, Adc2<...> ... )
   *
   * The default configuration is to run in circular buffer mode with half-word
   * transfers. These values can be customised by accessing the DMA_InitTypeDef structure
   * after the constructor has completed.
   */

  template<class TAdc,bool TRequestAfterLastTransfer=true,uint32_t TPriority=DMA_Priority_High,uint32_t TFifoMode=DMA_FIFOMode_Disable>
  class AdcDmaFeature : public DmaFeatureBase {

    public:
      AdcDmaFeature(Dma& dma);
      void beginRead(volatile void *dest,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<class TAdc,bool TRequestAfterLastTransfer,uint32_t TPriority,uint32_t TFifoMode>
  inline AdcDmaFeature<TAdc,TRequestAfterLastTransfer,TPriority,TFifoMode>::AdcDmaFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    ADC_TypeDef *adc;

    adc=(ADC_TypeDef *)TAdc::PERIPHERAL_BASE;

    _init.DMA_Channel=dma.getChannelNumber();                 // channel id
    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(adc->DR));
    _init.DMA_DIR=DMA_DIR_PeripheralToMemory;                 // 'peripheral' is source
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;        // 'peripheral' does not increment
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;                 // memory is incremented
    _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; // transferring 16-bits
    _init.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;         // transferring 16-bits
    _init.DMA_Mode=DMA_Mode_Circular;                         // is a circular buffer
    _init.DMA_Priority=TPriority;                             // user-configurable priority
    _init.DMA_MemoryBurst=DMA_MemoryBurst_Single;             // burst size
    _init.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;     // burst size
    _init.DMA_FIFOMode=TFifoMode;                             // FIFO mode

    if(TFifoMode==DMA_FIFOMode_Enable)
      _init.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;     // flush on half-full
  }


  /**
   * Start a transfer of data to the destination.
   *
   * @param[in] dest The destination of the transfer.
   * @param[in] count The number of bytes to transfer.
   */

  template<class TAdc,bool TRequestAfterLastTransfer,uint32_t TPriority,uint32_t TFifoMode>
  inline void AdcDmaFeature<TAdc,TRequestAfterLastTransfer,TPriority,TFifoMode>::beginRead(volatile void *dest,uint32_t count) {

    DMA_Stream_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_Memory0BaseAddr=reinterpret_cast<uint32_t>(dest);
    _init.DMA_BufferSize=count;

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

    // disable and then re-enable

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);

    ADC_DMARequestAfterLastTransferCmd((ADC_TypeDef *)TAdc::PERIPHERAL_BASE,TRequestAfterLastTransfer ? ENABLE : DISABLE);
    ADC_MultiModeDMARequestAfterLastTransferCmd(TRequestAfterLastTransfer ? ENABLE : DISABLE);

    ADC_DMACmd((ADC_TypeDef *)TAdc::PERIPHERAL_BASE,ENABLE);
  }
}
