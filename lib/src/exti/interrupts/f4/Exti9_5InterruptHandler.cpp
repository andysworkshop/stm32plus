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

template<> ExtiPeripheral<EXTI_Line5> *ExtiPeripheral<EXTI_Line5>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line6> *ExtiPeripheral<EXTI_Line6>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line7> *ExtiPeripheral<EXTI_Line7>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line8> *ExtiPeripheral<EXTI_Line8>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line9> *ExtiPeripheral<EXTI_Line9>::_extiInstance=nullptr;


#if defined(USE_EXTI9_5_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) EXTI9_5_IRQHandler(void) {

    if(EXTI_GetITStatus(EXTI_Line5)!=RESET) {
      Exti5::_extiInstance->ExtiInterruptEventSender.raiseEvent(5);
      EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if(EXTI_GetITStatus(EXTI_Line6)!=RESET) {
      Exti6::_extiInstance->ExtiInterruptEventSender.raiseEvent(6);
      EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if(EXTI_GetITStatus(EXTI_Line7)!=RESET) {
      Exti7::_extiInstance->ExtiInterruptEventSender.raiseEvent(7);
      EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if(EXTI_GetITStatus(EXTI_Line8)!=RESET) {
      Exti8::_extiInstance->ExtiInterruptEventSender.raiseEvent(8);
      EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(EXTI_GetITStatus(EXTI_Line9)!=RESET) {
      Exti9::_extiInstance->ExtiInterruptEventSender.raiseEvent(9);
      EXTI_ClearITPendingBit(EXTI_Line9);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif
#endif
