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
   * DMA feature to enable writing via DMA
   * @tparam TI2S The type of the I2S peripheral (I2S1<...>, I2S2<...> ... )
   */

  template<class TI2SPeripheralTraits,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal,uint32_t TFifoMode=DMA_FIFOMode_Disable>
  class I2SDmaWriterFeature : public DmaFeatureBase {

    public:
      I2SDmaWriterFeature(Dma& dma);
      void beginWrite(const void *source,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<class TI2SPeripheralTraits,uint32_t TPriority,uint32_t TDmaMode,uint32_t TFifoMode>
  inline I2SDmaWriterFeature<TI2SPeripheralTraits,TPriority,TDmaMode,TFifoMode>::I2SDmaWriterFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    SPI_TypeDef *i2s;

    i2s=(SPI_TypeDef *)TI2SPeripheralTraits::PERIPHERAL_BASE;

    _init.DMA_Channel=dma.getChannelNumber();                     // channel id
    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(i2s->DR));
    _init.DMA_DIR=DMA_DIR_MemoryToPeripheral;                     // 'peripheral' is destination
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;            // 'peripheral' does not increment
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;                     // memory is incremented
    _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; // 16-bits
    _init.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;         // 16-bits
    _init.DMA_Mode=TDmaMode;                                      // not a circular buffer by default
    _init.DMA_Priority=TPriority;                                 // user-configurable priority
    _init.DMA_MemoryBurst=DMA_MemoryBurst_Single;                 // burst size
    _init.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;         // burst size
    _init.DMA_FIFOMode=TFifoMode;

    if(TFifoMode==DMA_FIFOMode_Enable)
      _init.DMA_FIFOThreshold=DMA_FIFOThreshold_Full;             // flush on full

    SPI_I2S_DMACmd(i2s,SPI_I2S_DMAReq_Tx,ENABLE);
  }


  /**
   * Start a transfer of data to the I2S
   *
   * @param[in] source memory address of the source data bytes.
   * @param[in] count The number of half-words to transfer.
   */

  template<class TI2SPeripheralTraits,uint32_t TPriority,uint32_t TDmaMode,uint32_t TFifoMode>
  inline void I2SDmaWriterFeature<TI2SPeripheralTraits,TPriority,TDmaMode,TFifoMode>::beginWrite(const void *source,uint32_t count) {

    DMA_Stream_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_Memory0BaseAddr=reinterpret_cast<uint32_t>(source);
    _init.DMA_BufferSize=count;

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

      // disable and then re-enable

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
