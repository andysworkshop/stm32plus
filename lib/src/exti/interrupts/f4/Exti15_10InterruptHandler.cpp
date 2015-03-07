/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/exti.h"


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiPeripheral<EXTI_Line10> *ExtiPeripheral<EXTI_Line10>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line11> *ExtiPeripheral<EXTI_Line11>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line12> *ExtiPeripheral<EXTI_Line12>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line13> *ExtiPeripheral<EXTI_Line13>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line14> *ExtiPeripheral<EXTI_Line14>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line15> *ExtiPeripheral<EXTI_Line15>::_extiInstance=nullptr;


#if defined(USE_EXTI15_10_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) EXTI15_10_IRQHandler(void) {

    if(EXTI_GetITStatus(EXTI_Line10)!=RESET) {
      Exti10::_extiInstance->ExtiInterruptEventSender.raiseEvent(10);
      EXTI_ClearITPendingBit(EXTI_Line10);
    }
    else if(EXTI_GetITStatus(EXTI_Line11)!=RESET) {
      Exti11::_extiInstance->ExtiInterruptEventSender.raiseEvent(11);
      EXTI_ClearITPendingBit(EXTI_Line11);
    }
    else if(EXTI_GetITStatus(EXTI_Line12)!=RESET) {
      Exti12::_extiInstance->ExtiInterruptEventSender.raiseEvent(12);
      EXTI_ClearITPendingBit(EXTI_Line12);
    }
    else if(EXTI_GetITStatus(EXTI_Line13)!=RESET) {
      Exti13::_extiInstance->ExtiInterruptEventSender.raiseEvent(13);
      EXTI_ClearITPendingBit(EXTI_Line13);
    }
    else if(EXTI_GetITStatus(EXTI_Line14)!=RESET) {
      Exti14::_extiInstance->ExtiInterruptEventSender.raiseEvent(14);
      EXTI_ClearITPendingBit(EXTI_Line14);
    }
    else if(EXTI_GetITStatus(EXTI_Line15)!=RESET) {
      Exti15::_extiInstance->ExtiInterruptEventSender.raiseEvent(15);
      EXTI_ClearITPendingBit(EXTI_Line15);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif
#endif
