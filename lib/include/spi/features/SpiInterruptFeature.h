/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


/*
 * Forward declare the IRQ handler names
 */

extern "C" void SPI1_IRQHandler();
extern "C" void SPI2_IRQHandler();
extern "C" void SPI3_IRQHandler();


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each Spi peripheral
   * @tparam TSpiNumber The number of the Spi peripheral (1..3)
   */

  template<uint8_t TSpiNumber>
  class SpiInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable(uint8_t priority,uint8_t subPriority);
  };


  /**
   * SPI feature to handle interrupts.
   * @tparam TSpiNumber The number of the Spi peripheral (1..5)
   */

  template<uint8_t TSpiNumber>
  class SpiInterruptFeature : public SpiEventSource,
                              public SpiFeatureBase {

    protected:
      uint16_t _interruptMask;
      uint8_t _nvicPriority;
      uint8_t _nvicSubPriority;

    public:
      static SpiEventSource *_spiInstance;

    public:
      SpiInterruptFeature(Spi& spi);
      ~SpiInterruptFeature();

      void setNvicPriorities(uint8_t priority,uint8_t subPriority=0);

      void enableInterrupts(uint16_t interruptMask);
      void disableInterrupts(uint16_t interruptMask);

      void clearPendingInterruptsFlag(uint16_t interruptMask) const;
  };


  /**
   * Typedefs for easy use
   */

  typedef SpiInterruptFeature<1> Spi1InterruptFeature;
  typedef SpiInterruptFeature<2> Spi2InterruptFeature;
  typedef SpiInterruptFeature<3> Spi3InterruptFeature;


  /**
   * Template static data member initialisation
   */

  template<uint8_t TSpiNumber>
  SpiEventSource *SpiInterruptFeature<TSpiNumber> ::_spiInstance;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TSpiNumber>
  inline SpiInterruptFeature<TSpiNumber>::SpiInterruptFeature(Spi& spi)
    : SpiFeatureBase(spi) {
    _interruptMask=0;
    _nvicPriority=_nvicSubPriority=0;
    _spiInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TSpiNumber>
  inline SpiInterruptFeature<TSpiNumber>::~SpiInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Set the NVIC priority and subpriority numbers.  This function just remembers your
   * numbers. The actual setting of the priority happens in enableInterrupts()
   * @param priority
   * @param subPriority
   */

  template<uint8_t TSpiNumber>
  inline void SpiInterruptFeature<TSpiNumber>::setNvicPriorities(uint8_t priority,uint8_t subPriority) {
    _nvicPriority=priority;
    _nvicSubPriority=subPriority;
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. SPI_I2S_IT_TXE / SPI_I2S_IT_RXNE / SPI_I2S_IT_ERR
   */

  template<uint8_t TSpiNumber>
  inline void SpiInterruptFeature<TSpiNumber>::enableInterrupts(uint16_t interruptMask) {

    _interruptMask|=interruptMask;
    SpiInterruptFeatureEnabler<TSpiNumber>::enable(_nvicPriority,_nvicSubPriority);
    SPI_I2S_ITConfig(_spi,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. SPI_I2S_IT_TXE / SPI_I2S_IT_RXNE / SPI_I2S_IT_ERR
   */

  template<uint8_t TSpiNumber>
  inline void SpiInterruptFeature<TSpiNumber>::disableInterrupts(uint16_t interruptMask) {
    _interruptMask&=~interruptMask;
    SPI_I2S_ITConfig(_spi,interruptMask,DISABLE);
  }


  /**
   * Clear the selected pending interrupt flags
   * @param interruptMask The bitmask of interrupts, e.g. SPI_I2S_IT_TXE / SPI_I2S_IT_RXNE / SPI_I2S_IT_ERR
   */

  template<uint8_t TSpiNumber>
  inline void SpiInterruptFeature<TSpiNumber>::clearPendingInterruptsFlag(uint16_t interruptMask) const {
    SPI_I2S_ClearITPendingBit(_spi,interruptMask);
  }


  /**
   * Enabler specialisation, Spi 1
   */

  template<>
  inline void SpiInterruptFeatureEnabler<1>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&SPI1_IRQHandler;
    Nvic::configureIrq(SPI1_IRQn,ENABLE,priority,subPriority);
  }

  /**
   * Enabler specialisation, Spi 2
   */

  template<>
  inline void SpiInterruptFeatureEnabler<2>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&SPI2_IRQHandler;
    Nvic::configureIrq(SPI2_IRQn,ENABLE,priority,subPriority);
  }

#if !(defined(STM32PLUS_F1_MD) || defined(STM32PLUS_F1_MD_VL) || defined(STM32PLUS_F0))

  /**
   * Enabler specialisation, Spi 3
   */

  template<>
  inline void SpiInterruptFeatureEnabler<3>::enable(uint8_t priority,uint8_t subPriority) {
    _forceLinkage=&SPI3_IRQHandler;
    Nvic::configureIrq(SPI3_IRQn,ENABLE,priority,subPriority);
  }
#endif

}
