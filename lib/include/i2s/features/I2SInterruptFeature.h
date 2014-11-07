/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Forward declare the IRQ handler names
 */

extern "C" void SPI2_IRQHandler();
extern "C" void SPI3_IRQHandler();


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each I2S peripheral
   * @tparam TI2SNumber The number of the I2S peripheral (1..3)
   */

  template<uint8_t TI2SNumber>
  class I2SInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable(uint8_t priority,uint8_t subPriority);
  };


  /**
   * SPI feature to handle interrupts. This class is the only
   * Observable in the I2S class suite.
   * @tparam TI2SNumber The number of the I2S peripheral (1..5)
   */

  template<uint8_t TI2SNumber>
  class I2SInterruptFeature : public SpiEventSource,
                              public I2SFeatureBase {

    protected:
      uint16_t _interruptMask;
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      I2SInterruptFeature(I2S& i2s);
      ~I2SInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);
  };


  /*
   * Typedefs for easy use
   */

  typedef I2SInterruptFeature<2> I2S2InterruptFeature;
  typedef I2SInterruptFeature<3> I2S3InterruptFeature;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TI2SNumber>
  inline I2SInterruptFeature<TI2SNumber>::I2SInterruptFeature(I2S& i2s)
    : I2SFeatureBase(i2s) {
    _interruptMask=0;
    _nvicPriority=_nvicSubPriority=0;
    SpiInterruptFeature<TI2SNumber>::_spiInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TI2SNumber>
  inline I2SInterruptFeature<TI2SNumber>::~I2SInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  template<uint8_t TI2SNumber>
  inline void I2SInterruptFeature<TI2SNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. SPI_I2S_IT_TXE / SPI_I2S_IT_RXNE / SPI_I2S_IT_ERR
   */

  template<uint8_t TI2SNumber>
  inline void I2SInterruptFeature<TI2SNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    I2SInterruptFeatureEnabler<TI2SNumber>::enable(_nvicPriority,_nvicSubPriority);
    SPI_I2S_ITConfig(_i2s,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. SPI_I2S_IT_TXE / SPI_I2S_IT_RXNE / SPI_I2S_IT_ERR
   */

  template<uint8_t TI2SNumber>
  inline void I2SInterruptFeature<TI2SNumber>::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    SPI_I2S_ITConfig(_i2s,interruptMask,DISABLE);
  }

  /**
   * Enabler specialisation, I2S 2
   */

  template<>
  inline void I2SInterruptFeatureEnabler<2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&SPI2_IRQHandler;
    Nvic::configureIrq(SPI2_IRQn,ENABLE,priority,subPriority);
  }

#if !defined(STM32PLUS_F1_MD) && !defined(STM32PLUS_F1_MD_VL) && !defined(STM32PLUS_F0)

  /**
   * Enabler specialisation, I2S 3
   */

  template<>
  inline void I2SInterruptFeatureEnabler<3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&SPI3_IRQHandler;
    Nvic::configureIrq(SPI3_IRQn,ENABLE,priority,subPriority);
  }

#endif

}
