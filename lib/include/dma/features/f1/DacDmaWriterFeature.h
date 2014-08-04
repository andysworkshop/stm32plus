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
   * DMA feature to enable writing via to the DAC using a DMA channel.
   *
   * If the setup of the initialisation structure is not to your needs then it can be customised before the
   * transfers are started by casting this class to |DMA_InitTypeDef|
   *
   * @tparam TDacAlignmentFeature. This is the feature type that you parameterised your Dac type with. It can
   * be either of the 1/2 channels or the special DacDualChannel*Feature. Note that using the dual channel
   * modes implies that you have packed your data accordingly for transfer. See comments below regarding the
   * packed data format or the register description in RM0008 / RM0090.
   * @tparam TPriority One of the DMA priority constants. The default is |DMA_Priority_High|
   */

  template<class TDacAlignmentFeature,uint32_t TPriority=DMA_Priority_High>
  class DacDmaWriterFeature  : public DmaFeatureBase {

    public:
      DacDmaWriterFeature(Dma& dma);
      void beginWrite(const void *source,uint32_t count);
  };


  /**
   * Constructor, store the reference to the DMA base class
   * @param dma the base class reference
   */

  template<class TDacAlignmentFeature,uint32_t TPriority>
  inline DacDmaWriterFeature<TDacAlignmentFeature,TPriority>::DacDmaWriterFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    /*
     * The target for the DMA transfer depends on the alignment, channel (or dual channel).
     * All of these are available as compile-time constants in the alignment feature therefore
     * the optimiser will eliminate all the conditional tests below, remove the dead code and emit
     * only the resulting single storage instructions.
     */

    if(TDacAlignmentFeature::DUAL_CHANNEL_FEATURE) {

      // both channels

      if(TDacAlignmentFeature::getAlignment()==DAC_Align_12b_R)
        _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR12RD));
      else if(TDacAlignmentFeature::getAlignment()==DAC_Align_12b_L)
        _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR12LD));
      else if(TDacAlignmentFeature::getAlignment()==DAC_Align_8b_R)
        _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR8RD));
    }
    else {
      if(TDacAlignmentFeature::getChannel()==DAC_Channel_1) {

        if(TDacAlignmentFeature::getAlignment()==DAC_Align_12b_R)
          _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR12R1));
        else if(TDacAlignmentFeature::getAlignment()==DAC_Align_12b_L)
          _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR12L1));
        else if(TDacAlignmentFeature::getAlignment()==DAC_Align_8b_R)
          _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR8R1));
      }
      else if(TDacAlignmentFeature::getChannel()==DAC_Channel_2) {

        if(TDacAlignmentFeature::getAlignment()==DAC_Align_12b_R)
          _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR12R2));
        else if(TDacAlignmentFeature::getAlignment()==DAC_Align_12b_L)
          _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR12L2));
        else if(TDacAlignmentFeature::getAlignment()==DAC_Align_8b_R)
          _init.DMA_PeripheralBaseAddr=reinterpret_cast<uint32_t>(&(DAC->DHR8R2));
      }
    }

    if(TDacAlignmentFeature::DUAL_CHANNEL_FEATURE) {

      // 8 bit = half-word (packed channels: 22222221111111) transfers

      if(TDacAlignmentFeature::getAlignment()==DAC_Align_8b_R) {
        _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
        _init.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
      }
      else {

        // 12 bit = word (packed channels: xxxx222222222222xxxx111111111111) transfers

        _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;
        _init.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;
      }

    }
    else {

      if(TDacAlignmentFeature::getAlignment()==DAC_Align_8b_R) {
        _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;     // transferring bytes
        _init.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;             // transferring bytes
      }
      else {
        _init.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord; // transferring half-words
        _init.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;         // transferring half-words
      }
    }

    _init.DMA_DIR=DMA_DIR_PeripheralDST;                      // 'peripheral' is destination
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;        // 'peripheral' does not increment
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;                 // memory is incremented
    _init.DMA_Mode=DMA_Mode_Normal;                           // not a circular buffer
    _init.DMA_Priority=TPriority;                             // user-configurable priority
    _init.DMA_M2M=DMA_M2M_Disable;                            // memory->peripheral configuration
  }


  /**
   * Start a transfer of data to the DAC
   *
   * @param[in] source memory address of the source data bytes.
   * @param[in] count The number of bytes to transfer.
   */

  template<class TDacAlignmentFeature,uint32_t TPriority>
  inline void DacDmaWriterFeature<TTDacAlignmentFeature,TPriority>::beginWrite(const void *source,uint32_t count) {

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

    DAC_DMACmd(TDacAlignmentFeature::getChannel(),ENABLE);
  }
}
