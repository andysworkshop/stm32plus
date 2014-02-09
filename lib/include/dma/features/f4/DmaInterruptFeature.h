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


/*
 * Forward declare the IRQ handler names
 */

extern "C" void DMA1_Stream0_IRQHandler();
extern "C" void DMA1_Stream1_IRQHandler();
extern "C" void DMA1_Stream2_IRQHandler();
extern "C" void DMA1_Stream3_IRQHandler();
extern "C" void DMA1_Stream4_IRQHandler();
extern "C" void DMA1_Stream5_IRQHandler();
extern "C" void DMA1_Stream6_IRQHandler();
extern "C" void DMA1_Stream7_IRQHandler();

extern "C" void DMA2_Stream0_IRQHandler();
extern "C" void DMA2_Stream1_IRQHandler();
extern "C" void DMA2_Stream2_IRQHandler();
extern "C" void DMA2_Stream3_IRQHandler();
extern "C" void DMA2_Stream4_IRQHandler();
extern "C" void DMA2_Stream5_IRQHandler();
extern "C" void DMA2_Stream6_IRQHandler();
extern "C" void DMA2_Stream7_IRQHandler();


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each DMA peripheral and channel
   * @tparam TDmaNumber The number of the DMA peripheral (1..2)
   * @tparam TStreamNumber The number of the DMA channel (0..7)
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
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
   * @tparam TStreamNumber The number of the DMA stream (0..7)
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
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

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
  DmaEventSource *DmaInterruptFeature<TDmaNumber,TStreamNumber>::_dmaInstance;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
  inline DmaInterruptFeature<TDmaNumber,TStreamNumber>::DmaInterruptFeature(Dma& dma)
    : DmaFeatureBase(dma) {
    _interruptMask=0;
    _nvicPriority=0;
    _nvicSubPriority=0;
    _dmaInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
  inline DmaInterruptFeature<TDmaNumber,TStreamNumber>::~DmaInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
  inline void DmaInterruptFeature<TDmaNumber,TStreamNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. DMA_IT_TC/HT/TE
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
  inline void DmaInterruptFeature<TDmaNumber,TStreamNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    DmaInterruptFeatureEnabler<TDmaNumber,TStreamNumber>::enable(_nvicPriority,_nvicSubPriority);
    DMA_ITConfig(_dma,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. DMA_IT_TC/HT/TE
   */

  template<uint8_t TDmaNumber,uint8_t TStreamNumber>
  inline void DmaInterruptFeature<TDmaNumber,TStreamNumber>::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    DMA_ITConfig(_dma,interruptMask,DISABLE);
  }


  /**
   * Enabler specialisations, DMA1
   */

  template<>
  inline void DmaInterruptFeatureEnabler<1,0>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream0_IRQHandler;
    Nvic::configureIrq(DMA1_Stream0_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,1>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream1_IRQHandler;
    Nvic::configureIrq(DMA1_Stream1_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream2_IRQHandler;
    Nvic::configureIrq(DMA1_Stream2_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream3_IRQHandler;
    Nvic::configureIrq(DMA1_Stream3_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,4>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream4_IRQHandler;
    Nvic::configureIrq(DMA1_Stream4_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,5>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream5_IRQHandler;
    Nvic::configureIrq(DMA1_Stream5_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,6>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream6_IRQHandler;
    Nvic::configureIrq(DMA1_Stream6_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<1,7>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA1_Stream7_IRQHandler;
    Nvic::configureIrq(DMA1_Stream7_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisations, DMA2
   */

  template<>
  inline void DmaInterruptFeatureEnabler<2,0>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream0_IRQHandler;
    Nvic::configureIrq(DMA2_Stream0_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,1>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream1_IRQHandler;
    Nvic::configureIrq(DMA2_Stream1_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream2_IRQHandler;
    Nvic::configureIrq(DMA2_Stream2_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream3_IRQHandler;
    Nvic::configureIrq(DMA2_Stream3_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,4>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream4_IRQHandler;
    Nvic::configureIrq(DMA2_Stream4_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,5>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream5_IRQHandler;
    Nvic::configureIrq(DMA2_Stream5_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,6>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream6_IRQHandler;
    Nvic::configureIrq(DMA2_Stream6_IRQn,ENABLE,priority,subPriority);
  }

  template<>
  inline void DmaInterruptFeatureEnabler<2,7>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&DMA2_Stream7_IRQHandler;
    Nvic::configureIrq(DMA2_Stream7_IRQn,ENABLE,priority,subPriority);
  }
}
