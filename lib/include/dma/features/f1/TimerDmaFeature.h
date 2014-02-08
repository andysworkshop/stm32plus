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
   * Template definition of a DMA feature used for transferring data to a peripheral
   * where that transfer is regulated by events from a timer.
   *
   * @tparam TPeripheralInfo a helper class used to describe key data about the target peripheral
   * @tparam TTimerEvent The timer event that triggers a transfer, e.g. TIM_DMA_Update
   * @tparam TPriority The DMA priority, default is high
   * @tparam TDmaMode circular or normal mode (DMA_Mode_Normal is the default)
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  class TimerDmaFeature : public DmaFeatureBase {

    public:
      TimerDmaFeature(Dma& dma);

      void beginWriteByTimer(Timer& timer,const void *source,uint32_t count);
  };

  /**
   * Constructor. Set up as much of the DMA initialisation structure as can be
   * reused between calls.
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority,uint32_t TDmaMode>
  inline TimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode>::TimerDmaFeature(Dma& dma)
    : DmaFeatureBase(dma) {

    // set up the DMA peripheral to transfer to a peripheral register
    // the memory source is not set here

    _init.DMA_PeripheralBaseAddr=TPeripheralInfo::REGISTER_ADDRESS;
    _init.DMA_DIR=DMA_DIR_PeripheralDST;
    _init.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
    _init.DMA_MemoryInc=DMA_MemoryInc_Enable;
    _init.DMA_PeripheralDataSize=TPeripheralInfo::PERIPHERAL_WORD_SIZE;
    _init.DMA_MemoryDataSize=TPeripheralInfo::MEMORY_WORD_SIZE;
    _init.DMA_Priority=TPriority;
    _init.DMA_M2M=DMA_M2M_Disable;
    _init.DMA_Mode=TDmaMode;
  }


  /**
   * Start a transfer of data to the peripheral controlled by timer
   *
   * @param[in] source memory address of the source data bytes.
   * @param[in] count The number of words/half-words/bytes to transfer.
   */

  template<class TPeripheralInfo,uint16_t TTimerEvent,uint32_t TPriority,uint32_t TDmaMode>
  inline void TimerDmaFeature<TPeripheralInfo,TTimerEvent,TPriority,TDmaMode>::beginWriteByTimer(Timer& timer,const void *source,uint32_t count) {

    DMA_Channel_TypeDef *peripheralAddress;

    // set up the parameters for this transfer

    _init.DMA_MemoryBaseAddr=reinterpret_cast<uint32_t>(source);
    _init.DMA_BufferSize=count;

    // set the peripheral address from the overloaded operator

    peripheralAddress=_dma;

    // connect the timer to its associated DMA channel

    TIM_DMACmd(timer,TTimerEvent,ENABLE);

    // disable and then re-enable DMA

    DMA_Cmd(peripheralAddress,DISABLE);
    DMA_Init(peripheralAddress,&_init);
    DMA_Cmd(peripheralAddress,ENABLE);
  }



  /**
   * Partially specialised types for each timer event type
   */

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerUpdateDmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode> {
    TimerUpdateDmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_Update,TPriority,TDmaMode>(dma) {}
  };

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerCc1DmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC1,TPriority,TDmaMode> {
    TimerCc1DmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC1,TPriority,TDmaMode>(dma) {}
  };

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerCc2DmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC2,TPriority,TDmaMode> {
    TimerCc2DmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC2,TPriority,TDmaMode>(dma) {}
  };

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerCc3DmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC3,TPriority,TDmaMode> {
    TimerCc3DmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC3,TPriority,TDmaMode>(dma) {}
  };

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerCc4DmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC4,TPriority,TDmaMode> {
    TimerCc4DmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_CC4,TPriority,TDmaMode>(dma) {}
  };

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerComDmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_COM,TPriority,TDmaMode> {
    TimerComDmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_COM,TPriority,TDmaMode>(dma) {}
  };

  template<class TPeripheralInfo,uint32_t TPriority=DMA_Priority_High,uint32_t TDmaMode=DMA_Mode_Normal>
  struct TimerTriggerDmaFeature : public TimerDmaFeature<TPeripheralInfo,TIM_DMA_Trigger,TPriority,TDmaMode> {
    TimerTriggerDmaFeature(Dma& dma) : TimerDmaFeature<TPeripheralInfo,TIM_DMA_Trigger,TPriority,TDmaMode>(dma) {}
  };
}
