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


/**
 * Forward declare the IRQ handler name
 */

extern "C" void HASH_RNG_IRQHandler();


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC.
   */

  class RngInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable(uint8_t priority,uint8_t subPriority);
  };


  /**
   * Enabler specialisation, Rng 1
   */

  inline void RngInterruptFeatureEnabler::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&HASH_RNG_IRQHandler;
    Nvic::configureIrq(HASH_RNG_IRQn,ENABLE,priority,subPriority);
  }


  /**
   * RNG feature to handle interrupts
   */

  class RngInterruptFeature : public RngEventSource {

    protected:
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      static RngEventSource *_rngInstance;

    public:
      RngInterruptFeature();
      ~RngInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts();
      void disableInterrupts();
  };


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  inline RngInterruptFeature::RngInterruptFeature() {
    _nvicPriority=_nvicSubPriority=0;
    _rngInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  inline RngInterruptFeature::~RngInterruptFeature() {
    disableInterrupts();
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  inline void RngInterruptFeature::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   */

  inline void RngInterruptFeature::enableInterrupts() {
    RngInterruptFeatureEnabler::enable(_nvicPriority,_nvicSubPriority);
    RNG_ITConfig(ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   */

  inline void RngInterruptFeature::disableInterrupts() {
    RNG_ITConfig(DISABLE);
  }
}
