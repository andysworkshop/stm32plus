/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Forward declare the IRQ handler names
 */

extern "C" void I2C1_EV_IRQHandler();
extern "C" void I2C2_EV_IRQHandler();
extern "C" void I2C1_IRQHandler();        // F0

#if !defined(STM32PLUS_F0_42)
extern "C" void I2C2_IRQHandler();        // F0
#endif


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each I2C peripheral
   * @tparam TI2CNumber The number of the I2C peripheral (1..3)
   */

  template<uint8_t TI2CNumber>
  class I2CInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable(uint8_t priority,uint8_t subPriority);
  };


  /**
   * I2C feature to handle interrupts
   * @tparam TI2CNumber The number of the I2C peripheral (1..5)
   */

  template<uint8_t TI2CNumber>
  class I2CInterruptFeature : public I2CEventSource,
                              public I2CFeatureBase {

    protected:
      uint16_t _interruptMask;
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      static I2CEventSource *_i2cInstance;

    public:
      I2CInterruptFeature(I2C& I2C);
      ~I2CInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);

      void clearPendingInterruptsFlag(uint16_t interruptMask) const;
  };


  /*
   * Typedefs for easy use
   */

  typedef I2CInterruptFeature<1> I2C1InterruptFeature;

#if !defined(STM32PLUS_F0_42)
  typedef I2CInterruptFeature<2> I2C2InterruptFeature;
#endif

  /**
   * Template static data member initialisation
   */

  template<uint8_t TI2CNumber>
  I2CEventSource *I2CInterruptFeature<TI2CNumber> ::_i2cInstance;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TI2CNumber>
  inline I2CInterruptFeature<TI2CNumber>::I2CInterruptFeature(I2C& I2C)
    : I2CFeatureBase(I2C) {
    _interruptMask=0;
    _nvicPriority=_nvicSubPriority=0;
    _i2cInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TI2CNumber>
  inline I2CInterruptFeature<TI2CNumber>::~I2CInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  template<uint8_t TI2CNumber>
  inline void I2CInterruptFeature<TI2CNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. I2C_IT_BUF / I2C_IT_EVT / I2C_IT_ERR
   */

  template<uint8_t TI2CNumber>
  inline void I2CInterruptFeature<TI2CNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    I2CInterruptFeatureEnabler<TI2CNumber>::enable(_nvicPriority,_nvicSubPriority);
    I2C_ITConfig(_i2c,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. I2C_IT_TXE / I2C_IT_RXNE
   */

  template<uint8_t TI2CNumber>
  inline void I2CInterruptFeature<TI2CNumber>::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    I2C_ITConfig(_i2c,interruptMask,DISABLE);
  }


  /**
   * Clear the pending bit(s) for interrupt
   * @param interruptMask The bitmask of interrupts, e.g. I2C_IT_TXE / I2C_IT_RXNE
   */

  template<uint8_t TI2CNumber>
  inline void I2CInterruptFeature<TI2CNumber>::clearPendingInterruptsFlag(uint16_t interruptMask) const {
    I2C_ClearITPendingBit(_i2c,interruptMask);
  }


#if defined(STM32PLUS_F1) || defined(STM32PLUS_F4)

  /**
   * Enabler specialisation, I2C 1
   */

  template<>
  inline void I2CInterruptFeatureEnabler<1>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&I2C1_EV_IRQHandler;
    Nvic::configureIrq(I2C1_EV_IRQn,ENABLE,priority,subPriority);
    Nvic::configureIrq(I2C1_ER_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, I2C 2
   */

  template<>
  inline void I2CInterruptFeatureEnabler<2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&I2C2_EV_IRQHandler;
    Nvic::configureIrq(I2C2_EV_IRQn,ENABLE,priority,subPriority);
    Nvic::configureIrq(I2C2_ER_IRQn,ENABLE,priority,subPriority);
  }

#elif defined(STM32PLUS_F0)

  /**
   * Enabler specialisation, I2C 1
   */

  template<>
  inline void I2CInterruptFeatureEnabler<1>::enable(uint8_t priority,uint8_t /* subPriority */) {
    _forceLinkage=&I2C1_IRQHandler;
    Nvic::configureIrq(I2C1_IRQn,ENABLE,priority);
  }


#if !defined(STM32PLUS_F0_42)
  /**
   * Enabler specialisation, I2C 2
   */

  template<>
  inline void I2CInterruptFeatureEnabler<2>::enable(uint8_t priority,uint8_t /* subPriority */) {
    _forceLinkage=&I2C2_IRQHandler;
    Nvic::configureIrq(I2C2_IRQn,ENABLE,priority);
  }
#endif

#endif
}
