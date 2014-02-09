/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1_CL
#error This class can only be used with the STM32F1 CL series
#endif


/*
 * Forward declare the IRQ handler names
 */

extern "C" void DMA1_Channel1_IRQHandler();
extern "C" void DMA1_Channel2_IRQHandler();
extern "C" void DMA1_Channel3_IRQHandler();
extern "C" void DMA1_Channel4_IRQHandler();
extern "C" void DMA1_Channel5_IRQHandler();
extern "C" void DMA1_Channel6_IRQHandler();
extern "C" void DMA1_Channel7_IRQHandler();
extern "C" void DMA2_Channel1_IRQHandler();
extern "C" void DMA2_Channel2_IRQHandler();
extern "C" void DMA2_Channel3_IRQHandler();
extern "C" void DMA2_Channel4_IRQHandler();
extern "C" void DMA2_Channel5_IRQHandler();


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each DMA peripheral and channel
   * @tparam TDmaNumber The number of the DMA peripheral (1..2)
   * @tparam TChannelNumber The number of the DMA channel (1..7) or (1..5)
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  class DmaInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable(uint8_t priority,uint8_t subPriority);
  };


  /**
   * Base class for the DMA feature to handle interrupts
   * @tparam TDmaNumber The number of the DMA peripheral (1..2)
   * @tparam TChannelNumber The number of the DMA channel (1..7) or (1..5)
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  class DmaInterruptFeature : public DmaEventSource,
                              public DmaFeatureBase {

    protected:
      uint16_t _interruptMask;
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      static DmaEventSource *_dmaInstance;

      enum {
        COMPLETE = DMA_IT_TC,
        HALF_COMPLETE = DMA_IT_HT,
        TRANSFER_ERROR = DMA_IT_TE
      };

    protected:
      DmaInterruptFeature(Dma& dma);

    public:
      ~DmaInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);
  };


  /**
   * Template static data member initialisation
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  DmaEventSource *DmaInterruptFeature<TDmaNumber,TChannelNumber>::_dmaInstance;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  inline DmaInterruptFeature<TDmaNumber,TChannelNumber>::DmaInterruptFeature(Dma& dma)
    : DmaFeatureBase(dma) {
    _nvicPriority=0;
    _nvicSubPriority=0;
    _interruptMask=0;
    _dmaInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  inline DmaInterruptFeature<TDmaNumber,TChannelNumber>::~DmaInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  inline void DmaInterruptFeature<TDmaNumber,TChannelNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. DMA_IT_TC/HT/TE
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  inline void DmaInterruptFeature<TDmaNumber,TChannelNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    DmaInterruptFeatureEnabler<TDmaNumber,TChannelNumber>::enable(_nvicPriority,_nvicSubPriority);
    DMA_ITConfig(_dma,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. DMA_IT_TC/HT/TE
   */

  template<uint8_t TDmaNumber,uint8_t TChannelNumber>
  inline void DmaInterruptFeature<TDmaNumber,TChannelNumber>::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    DMA_ITConfig(_dma,interruptMask,DISABLE);
  }


  /**
   * Enabler specialisation, Dma 1, channel 1
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,1>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel1_IRQHandler;
    Nvic::configureIrq(DMA1_Channel1_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 1, channel 2
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel2_IRQHandler;
    Nvic::configureIrq(DMA1_Channel2_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 1, channel 3
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel3_IRQHandler;
    Nvic::configureIrq(DMA1_Channel3_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 1, channel 4
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,4>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel4_IRQHandler;
    Nvic::configureIrq(DMA1_Channel4_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 1, channel 5
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,5>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel5_IRQHandler;
    Nvic::configureIrq(DMA1_Channel5_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 1, channel 6
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,6>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel6_IRQHandler;
    Nvic::configureIrq(DMA1_Channel6_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 1, channel 7
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,7>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Channel7_IRQHandler;
    Nvic::configureIrq(DMA1_Channel7_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 2, channel 1
   */

  template<>
  inline void DmaInterruptFeatureEnabler<2,1>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Channel1_IRQHandler;
    Nvic::configureIrq(DMA2_Channel1_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 2, channel 2
   */

  template<>
  inline void DmaInterruptFeatureEnabler<2,2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Channel2_IRQHandler;
    Nvic::configureIrq(DMA2_Channel2_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 2, channel 3
   */

  template<>
  inline void DmaInterruptFeatureEnabler<2,3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Channel3_IRQHandler;
    Nvic::configureIrq(DMA2_Channel3_IRQn,ENABLE,priority,subPriority);
  }


  /**
   * Enabler specialisation, Dma 2, channel 4
   */

  template<>
  inline void DmaInterruptFeatureEnabler<2,4>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Channel4_IRQHandler;
    Nvic::configureIrq(DMA2_Channel4_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Dma 2, channel 5
   */

  template<>
  inline void DmaInterruptFeatureEnabler<2,5>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Channel5_IRQHandler;
    Nvic::configureIrq(DMA2_Channel5_IRQn,ENABLE,priority,subPriority);
  }
}
