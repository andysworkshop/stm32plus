/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


/*
 * Forward declare the IRQ handler names
 */

extern "C" void USART1_IRQHandler();
extern "C" void USART2_IRQHandler();


namespace stm32plus {

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each USART peripheral
   * @tparam TUsartNumber The number of the USART peripheral (1..2)
   */

  template<uint8_t TUsartNumber>
  class UsartInterruptFeatureEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable();
  };


  /**
   * DMA feature to handle interrupts
   * @tparam TUsartNumber The number of the USART peripheral (1..2)
   */

  template<uint8_t TUsartNumber>
  class UsartInterruptFeature : public UsartEventSource,
                                public UsartFeatureBase {

    protected:
      uint32_t _interruptMask;

    public:

      enum {
        RECEIVE = USART_IT_RXNE,
        TRANSMIT = USART_IT_TXE
      };

      static UsartEventSource *_usartInstance;

    public:
      UsartInterruptFeature(Usart& usart);
      ~UsartInterruptFeature();

      void enableInterrupts(uint32_t interruptMask);
      void disableInterrupts(uint32_t interruptMask);
  };


  /**
   * Typedefs for each USART
   */

  typedef UsartInterruptFeature<1> Usart1InterruptFeature;
  typedef UsartInterruptFeature<2> Usart2InterruptFeature;


  /**
   * Template static data member initialisation
   */

  template<uint8_t TUsartNumber>
  UsartEventSource *UsartInterruptFeature<TUsartNumber> ::_usartInstance;


  /**
   * Constructor, initialises the interrupt mask for this class to zero.
   */

  template<uint8_t TUsartNumber>
  inline UsartInterruptFeature<TUsartNumber>::UsartInterruptFeature(Usart& usart)
    : UsartFeatureBase(usart) {

    _interruptMask=0;
    _usartInstance=this;
  }


  /**
   * Destructor, if any interrupts are configured for this class then
   * disable them so that ISR calls don't disappear off into nothing
   */

  template<uint8_t TUsartNumber>
  inline UsartInterruptFeature<TUsartNumber>::~UsartInterruptFeature() {
    if(_interruptMask!=0)
      disableInterrupts(_interruptMask);
  }


  /**
   * Enable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. USART_IT_TXE / USART_IT_RXNE
   */

  template<uint8_t TUsartNumber>
  inline void UsartInterruptFeature<TUsartNumber>::enableInterrupts(uint32_t interruptMask) {

    _interruptMask|=interruptMask;
    UsartInterruptFeatureEnabler<TUsartNumber>::enable();
    USART_ITConfig(_usart,interruptMask,ENABLE);
  }


  /**
   * Disable the interrupts specified in the mask
   * @param interruptMask The bitmask of interrupts, e.g. USART_IT_TXE / USART_IT_RXNE
   */

  template<uint8_t TUsartNumber>
  inline void UsartInterruptFeature<TUsartNumber>::disableInterrupts(uint32_t interruptMask) {
    _interruptMask&=~interruptMask;
    USART_ITConfig(_usart,interruptMask,DISABLE);
  }


  /**
   * Enabler specialisation, Usart 1
   */

  template<>
  inline void UsartInterruptFeatureEnabler<1>::enable() {
    _forceLinkage=&USART1_IRQHandler;
    Nvic::configureIrq(USART1_IRQn);
  }

  /**
   * Enabler specialisation, Usart 2
   */

  template<>
  inline void UsartInterruptFeatureEnabler<2>::enable() {
    _forceLinkage=&USART2_IRQHandler;
    Nvic::configureIrq(USART2_IRQn);
  }
}
