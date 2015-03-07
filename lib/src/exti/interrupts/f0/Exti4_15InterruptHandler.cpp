/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F0)

#include "config/exti.h"


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiPeripheral<EXTI_Line4> *ExtiPeripheral<EXTI_Line4>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line5> *ExtiPeripheral<EXTI_Line5>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line6> *ExtiPeripheral<EXTI_Line6>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line7> *ExtiPeripheral<EXTI_Line7>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line8> *ExtiPeripheral<EXTI_Line8>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line9> *ExtiPeripheral<EXTI_Line9>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line10> *ExtiPeripheral<EXTI_Line10>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line11> *ExtiPeripheral<EXTI_Line11>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line12> *ExtiPeripheral<EXTI_Line12>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line13> *ExtiPeripheral<EXTI_Line13>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line14> *ExtiPeripheral<EXTI_Line14>::_extiInstance=nullptr;
template<> ExtiPeripheral<EXTI_Line15> *ExtiPeripheral<EXTI_Line15>::_extiInstance=nullptr;


#if defined(USE_EXTI4_15_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) EXTI4_15_IRQHandler(void) {

    if(EXTI_GetITStatus(EXTI_Line4)!=RESET) {
        Exti4::_extiInstance->ExtiInterruptEventSender.raiseEvent(4);
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
    if(EXTI_GetITStatus(EXTI_Line5)!=RESET) {
        Exti5::_extiInstance->ExtiInterruptEventSender.raiseEvent(5);
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line6)!=RESET) {
        Exti6::_extiInstance->ExtiInterruptEventSender.raiseEvent(6);
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if(EXTI_GetITStatus(EXTI_Line7)!=RESET) {
        Exti7::_extiInstance->ExtiInterruptEventSender.raiseEvent(7);
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
    if(EXTI_GetITStatus(EXTI_Line8)!=RESET) {
        Exti8::_extiInstance->ExtiInterruptEventSender.raiseEvent(8);
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
    if(EXTI_GetITStatus(EXTI_Line9)!=RESET) {
        Exti9::_extiInstance->ExtiInterruptEventSender.raiseEvent(9);
        EXTI_ClearITPendingBit(EXTI_Line9);
    }
    if(EXTI_GetITStatus(EXTI_Line10)!=RESET) {
        Exti10::_extiInstance->ExtiInterruptEventSender.raiseEvent(10);
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if(EXTI_GetITStatus(EXTI_Line11)!=RESET) {
        Exti11::_extiInstance->ExtiInterruptEventSender.raiseEvent(11);
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if(EXTI_GetITStatus(EXTI_Line12)!=RESET) {
        Exti12::_extiInstance->ExtiInterruptEventSender.raiseEvent(12);
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if(EXTI_GetITStatus(EXTI_Line13)!=RESET) {
        Exti13::_extiInstance->ExtiInterruptEventSender.raiseEvent(13);
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if(EXTI_GetITStatus(EXTI_Line14)!=RESET) {
        Exti14::_extiInstance->ExtiInterruptEventSender.raiseEvent(14);
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if(EXTI_GetITStatus(EXTI_Line15)!=RESET) {
        Exti15::_extiInstance->ExtiInterruptEventSender.raiseEvent(15);
        EXTI_ClearITPendingBit(EXTI_Line15);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif
#endif
