/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Forward declare the IRQ handler names
 */

extern "C" void SDIO_IRQHandler();


namespace stm32plus {


  /**
   * SDIO feature to handle interrupts
   */

  class SdioInterruptFeature : public SdioEventSource,
                               public SdCardFeatureBase {

    protected:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

      uint16_t _interruptMask;
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      static SdioEventSource *_sdioInstance;

    public:
      SdioInterruptFeature(SdCardBase& sdcard);
      ~SdioInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);
  };


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  inline SdioInterruptFeature::SdioInterruptFeature(SdCardBase& sdcard)
    : SdCardFeatureBase(sdcard) {
    _interruptMask=0;
    _nvicPriority=_nvicSubPriority=0;
    _sdioInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  inline SdioInterruptFeature::~SdioInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  inline void SdioInterruptFeature::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. SDIO_IT_DCRCFAIL
   */

  inline void SdioInterruptFeature::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    _forceLinkage=&SDIO_IRQHandler;

    Nvic::configureIrq(SDIO_IRQn,ENABLE,_nvicPriority,_nvicSubPriority);
    SDIO_ITConfig(interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. SDIO_IT_DCRCFAIL
   */

  inline void SdioInterruptFeature::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    SDIO_ITConfig(interruptMask,DISABLE);
  }
}
