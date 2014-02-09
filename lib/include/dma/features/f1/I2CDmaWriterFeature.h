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
   * @tparam TI2C The I2C peripheral traits class
   * @tparam TPriority The DMA priority level
   */

  template<class TI2C,uint32_t TPriority=DMA_Priority_High>
  class I2CDmaWriterFeature : public DmaFeatureBase {

    public:
      I2CDmaWriterFeature(Dma& dma);
      void beginWrite(const void *source,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<class TI2C,uint32_t TPriority>
  inline I2CDmaWriterFeature<TI2C,TPriority>::I2CDmaWriterFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    I2C_TypeDef *i2c;

    i2c=(I2C_TypeDef *)TI2C::PERIPHERAL_BASE;

    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(i2c->DR));
    _init.DMA_DIR=DMA_DIR_PeripheralDST;                      // 'peripheral' is destination
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;        // 'peripheral' does not increment
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;                 // memory is incremented
    _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte; // transferring bytes
    _init.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;         // user defined word size
    _init.DMA_Mode=DMA_Mode_Normal;                           // not a circular buffer
    _init.DMA_Priority=TPriority;                             // user-configurable priority
    _init.DMA_M2M=DMA_M2M_Disable;                            // memory->peripheral configuration

    I2C_DMACmd(i2c,ENABLE);
  }


  /**
   * Start a transfer of data to the I2C
   *
   * @param[in] source memory address of the source data bytes.
   * @param[in] count The number of bytes to transfer.
   */

  template<class TI2C,uint32_t TPriority>
  inline void I2CDmaWriterFeature<TI2C,TPriority>::beginWrite(const void *source,uint32_t count) {

    DMA_Channel_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_MemoryBaseAddr=reinterpret_cast<uint32_t>(source);
    _init.DMA_BufferSize=count;

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

      // disable and then re-enable

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
