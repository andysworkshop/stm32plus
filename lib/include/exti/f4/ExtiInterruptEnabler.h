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


namespace stm32plus {

  /*
   * Forward declare the IRQ handler names
   */

  extern "C" void EXTI0_IRQHandler();
  extern "C" void EXTI1_IRQHandler();
  extern "C" void EXTI2_IRQHandler();
  extern "C" void EXTI3_IRQHandler();
  extern "C" void EXTI4_IRQHandler();
  extern "C" void EXTI9_5_IRQHandler();
  extern "C" void EXTI15_10_IRQHandler();
  extern "C" void PVD_IRQHandler();
  extern "C" void RTCAlarm_IRQHandler();
  extern "C" void OTG_FS_WKUP_IRQHandler();
  extern "C" void ETH_WKUP_IRQHandler();
  extern "C" void OTG_HS_WKUP_IRQHandler();
  extern "C" void TAMP_STAMP_IRQHandler();
  extern "C" void RTC_WKUP_IRQHandler();


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
   * Enabler specialisations for the F4
   */

  template<>
  inline void ExtiInterruptEnabler<0>::enable() {
    _forceLinkage=&EXTI0_IRQHandler;
    Nvic::configureIrq(EXTI0_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<0>::disable() {
    Nvic::configureIrq(EXTI0_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<1>::enable() {
    _forceLinkage=&EXTI1_IRQHandler;
    Nvic::configureIrq(EXTI1_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<1>::disable() {
    Nvic::configureIrq(EXTI1_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<2>::enable() {
    _forceLinkage=&EXTI2_IRQHandler;
    Nvic::configureIrq(EXTI2_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<2>::disable() {
    Nvic::configureIrq(EXTI2_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<3>::enable() {
    _forceLinkage=&EXTI3_IRQHandler;
    Nvic::configureIrq(EXTI3_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<3>::disable() {
    Nvic::configureIrq(EXTI3_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<4>::enable() {
    _forceLinkage=&EXTI4_IRQHandler;
    Nvic::configureIrq(EXTI4_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<4>::disable() {
    Nvic::configureIrq(EXTI4_IRQn,DISABLE);
  }


  /**
   * 5 through 9 are on a shared IRQ
   */

  template<>
  inline void ExtiInterruptEnabler<5>::enable() {
    _forceLinkage=&EXTI9_5_IRQHandler;
    Nvic::configureIrq(EXTI9_5_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<5>::disable() {
    Nvic::configureIrq(EXTI9_5_IRQn,DISABLE);
  }


  /**
   * 10 through 15 are on a shared IRQ
   */

  template<>
  inline void ExtiInterruptEnabler<10>::enable() {
    _forceLinkage=&EXTI15_10_IRQHandler;
    Nvic::configureIrq(EXTI15_10_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<10>::disable() {
    Nvic::configureIrq(EXTI15_10_IRQn,DISABLE);
  }


  /**
   * Non-GPIO EXTI lines
   */

  template<>
  inline void ExtiInterruptEnabler<16>::enable() {
    _forceLinkage=&PVD_IRQHandler;
    Nvic::configureIrq(PVD_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<16>::disable() {
    Nvic::configureIrq(PVD_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<17>::enable() {
    _forceLinkage=&RTCAlarm_IRQHandler;
    Nvic::configureIrq(RTC_Alarm_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<17>::disable() {
    Nvic::configureIrq(RTC_Alarm_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<18>::enable() {
    _forceLinkage=&OTG_FS_WKUP_IRQHandler;
    Nvic::configureIrq(OTG_FS_WKUP_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<18>::disable() {
    Nvic::configureIrq(OTG_FS_WKUP_IRQn,DISABLE);
  }


#if defined(STM32PLUS_F4_HAS_MAC)

  /**
   * Ethernet EXTI is available
   */


  template<>
  inline void ExtiInterruptEnabler<19>::enable() {
    _forceLinkage=&ETH_WKUP_IRQHandler;
    Nvic::configureIrq(ETH_WKUP_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<19>::disable() {
    Nvic::configureIrq(ETH_WKUP_IRQn,DISABLE);
  }

#endif


  /**
   * Additional EXTI lines on many F4 devices
   */

#if defined(STM32PLUS_F4_HAS_OTG_HS)

  template<>
  inline void ExtiInterruptEnabler<20>::enable() {
    _forceLinkage=&OTG_HS_WKUP_IRQHandler;
    Nvic::configureIrq(OTG_HS_WKUP_IRQn);
  }


  template<>
  inline void ExtiInterruptEnabler<20>::disable() {
    Nvic::configureIrq(OTG_HS_WKUP_IRQn,DISABLE);
  }

#endif


  template<>
  inline void ExtiInterruptEnabler<21>::enable() {
    _forceLinkage=&TAMP_STAMP_IRQHandler;
    Nvic::configureIrq(TAMP_STAMP_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<21>::disable() {
    Nvic::configureIrq(TAMP_STAMP_IRQn,DISABLE);
  }


  template<>
  inline void ExtiInterruptEnabler<22>::enable() {
    _forceLinkage=&RTC_WKUP_IRQHandler;
    Nvic::configureIrq(RTC_WKUP_IRQn);
  }

  template<>
  inline void ExtiInterruptEnabler<22>::disable() {
    Nvic::configureIrq(RTC_WKUP_IRQn,DISABLE);
  }
}
