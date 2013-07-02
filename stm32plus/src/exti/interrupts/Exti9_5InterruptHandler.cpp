/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/exti.h"


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiInterruptEnabler<5>::FPTR ExtiInterruptEnabler<5>::_forceLinkage=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line5>::_extiInstance=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line6>::_extiInstance=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line7>::_extiInstance=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line8>::_extiInstance=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line9>::_extiInstance=nullptr;


#if defined(USE_EXTI9_5_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) EXTI9_5_IRQHandler(void) {

    if(EXTI_GetITStatus(EXTI_Line5)!=RESET) {
      Exti5::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)5);
      EXTI_ClearITPendingBit(EXTI_Line5);
    }
    else if(EXTI_GetITStatus(EXTI_Line6)!=RESET) {
      Exti6::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)6);
      EXTI_ClearITPendingBit(EXTI_Line6);
    }
    else if(EXTI_GetITStatus(EXTI_Line7)!=RESET) {
      Exti7::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)7);
      EXTI_ClearITPendingBit(EXTI_Line7);
    }
    else if(EXTI_GetITStatus(EXTI_Line8)!=RESET) {
      Exti8::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)8);
      EXTI_ClearITPendingBit(EXTI_Line8);
    }
    else if(EXTI_GetITStatus(EXTI_Line9)!=RESET) {
      Exti9::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)9);
      EXTI_ClearITPendingBit(EXTI_Line9);
    }
  }
}

#endif
