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
   * @brief Copy data to an LCD display connected via an 8080 interface.
   *
   * Custom DMA configuration to bulk copy data into the lcd data register through the FSMC.
   * This can free the CPU to do other things while data is transferred to the display. Access
   * to this class from the graphics library is done via the DmaLcdWriter template base class
   * because the graphics library cannot assume that the FSMC is the peripheral being used to
   * access the LCD behind the scenes.
   *
   * @tparam TFsmcAccessMode The access mode class (Fsmc8BitAccessMode/Fsmc16BitAccessMode)
   */

  template<class TFsmcAccessMode>
  class DmaFsmcLcdMemoryCopyFeature : public DmaFeatureBase,
                                      public DmaLcdWriter<DmaFsmcLcdMemoryCopyFeature<TFsmcAccessMode>> {

    private:
      uint32_t _byteSize;

    public:
      DmaFsmcLcdMemoryCopyFeature(Dma& dma);
      void beginCopyToLcd(void *dest,void *source,uint32_t byteCount,uint32_t priority);
  };


  /**
   * Constructor
   */

  template<class TFsmcAccessMode>
  inline DmaFsmcLcdMemoryCopyFeature<TFsmcAccessMode>::DmaFsmcLcdMemoryCopyFeature(Dma& dma)
    : DmaFeatureBase(dma),
      DmaLcdWriter<DmaFsmcLcdMemoryCopyFeature<TFsmcAccessMode>>(*this) {

    DMA_StructInit(&_init);

    _init.DMA_Channel=dma.getChannelNumber();               // channel id
    _init.DMA_DIR=DMA_DIR_MemoryToMemory;                   // memory to memory mode
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Enable;       // 'peripheral' gets incremented
    _init.DMA_MemoryInc=DMA_MemoryInc_Disable;              // LCD register is not incremented
    _init.DMA_Mode=DMA_Mode_Normal;                         // not a circular buffer
    _init.DMA_FIFOMode=DMA_FIFOMode_Enable;                 // FIFO required for memory-to-memory
    _init.DMA_FIFOThreshold=DMA_FIFOThreshold_HalfFull;     // flush on half-full
    _init.DMA_MemoryBurst=DMA_MemoryBurst_Single;           // burst size
    _init.DMA_PeripheralBurst=DMA_PeripheralBurst_Single;   // burst size

    TFsmcAccessMode::getDmaTransferSizes(_init.DMA_PeripheralDataSize,_init.DMA_MemoryDataSize);

    // get the width in bytes

    _byteSize=_init.DMA_PeripheralDataSize==DMA_PeripheralDataSize_Byte ? 1 : 2;
  }


  /**
   * Start a transfer of data from source to the dest. The count is in units
   * of the word size.
   *
   * @param[in] dest The destination of the transfer.
   * @param[in] source The source of the transfer.
   * @param[in] byteCount The number of bytes to transfer.
   * @param[in] priority The DMA priority level
   */

  template<class TFsmcAccessMode>
  inline void DmaFsmcLcdMemoryCopyFeature<TFsmcAccessMode>::beginCopyToLcd(void *dest,void *source,uint32_t byteCount,uint32_t priority) {

    DMA_Stream_TypeDef *peripheralAddress;

    _init.DMA_Priority=priority;
    _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(source);
    _init.DMA_Memory0BaseAddr=reinterpret_cast<uint32_t>(dest);
    _init.DMA_BufferSize=byteCount/_byteSize;

    // this class is always in a hierarchy with DmaPeripheral

    peripheralAddress=_dma;

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_DeInit(peripheralAddress);

    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }
}
