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


namespace stm32plus {

  /*
   * Forward declare the IRQ handler names
   */

  extern "C" void EXTI0_1_IRQHandler();
  extern "C" void EXTI2_3_IRQHandler();
  extern "C" void EXTI4_15_IRQHandler();
  extern "C" void EXTI15_10_IRQHandler();
#if defined(STM32PLUS_F0_51)
  extern "C" void PVD_IRQHandler();
#endif
  extern "C" void RTC_IRQHandler();
#if defined(STM32PLUS_F0_51)
  extern "C" void TS_IRQHandler();
#endif

  /**
   * Helper class to enable only the desired interrupts in the NVIC. This will
   * be fully specialised for each EXTI peripheral
   * @tparam TExtiNumber The number of the Exti peripheral (0..19 | 22)
   */

  template<uint8_t TExtiNumber>
  class ExtiInterruptEnabler {

    private:
      typedef void (*FPTR)();         // this trick will force the linker to include the ISR
      static FPTR _forceLinkage;

    public:
      static void enable();
      static void disable();
  };

  template<uint8_t TExtiNumber>
  typename ExtiInterruptEnabler<TExtiNumber>::FPTR ExtiInterruptEnabler<TExtiNumber>::_forceLinkage=nullptr;

  /**
   * Enabler specialisations for lines 0 to 3
   */

  template<>
  inline void ExtiInterruptEnabler<0>::enable() {
    _forceLinkage=&EXTI0_1_IRQHandler;
    Nvic::configureIrq(EXTI0_1_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<0>::disable() {
    Nvic::configureIrq(EXTI0_1_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<1>::enable() {
    _forceLinkage=&EXTI0_1_IRQHandler;
    Nvic::configureIrq(EXTI0_1_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<1>::disable() {
    Nvic::configureIrq(EXTI0_1_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<2>::enable() {
    _forceLinkage=&EXTI2_3_IRQHandler;
    Nvic::configureIrq(EXTI2_3_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<2>::disable() {
    Nvic::configureIrq(EXTI2_3_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<3>::enable() {
    _forceLinkage=&EXTI2_3_IRQHandler;
    Nvic::configureIrq(EXTI2_3_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<3>::disable() {
    Nvic::configureIrq(EXTI2_3_IRQn,DISABLE);
  }

  /**
   * 4 through 15 are on a shared IRQ
   */

  template<>
  inline void ExtiInterruptEnabler<4>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<4>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<5>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<5>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<6>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<6>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<7>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<7>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<8>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<8>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<9>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<9>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<10>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<10>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<11>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<11>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<12>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<12>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<13>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<13>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<14>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<14>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  template<>
  inline void ExtiInterruptEnabler<15>::enable() {
    _forceLinkage=&EXTI4_15_IRQHandler;
    Nvic::configureIrq(EXTI4_15_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<15>::disable() {
    Nvic::configureIrq(EXTI4_15_IRQn,DISABLE);
  }

  /**
   * Non-GPIO EXTI lines
   */
#if defined(STM32PLUS_F0_51)
  template<>
  inline void ExtiInterruptEnabler<16>::enable() {
    _forceLinkage=&PVD_IRQHandler;
    Nvic::configureIrq(PVD_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<16>::disable() {
    Nvic::configureIrq(PVD_IRQn,DISABLE);
  }

#endif

  template<>
  inline void ExtiInterruptEnabler<17>::enable() {
    _forceLinkage=&RTC_IRQHandler;
    Nvic::configureIrq(RTC_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<17>::disable() {
    Nvic::configureIrq(RTC_IRQn,DISABLE);
  }

#if defined(STM32PLUS_F0_51)

  template<>
  inline void ExtiInterruptEnabler<19>::enable() {
    _forceLinkage=&TS_IRQHandler;
    Nvic::configureIrq(TS_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<19>::disable() {
    Nvic::configureIrq(TS_IRQn,DISABLE);
  }

#endif
}
