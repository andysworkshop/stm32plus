/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/exti.h"

#if defined(STM32PLUS_F1)


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiInterruptEnabler<17>::FPTR ExtiInterruptEnabler<17>::_forceLinkage=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line17>::_extiInstance=nullptr;


#if defined(USE_EXTI_RTC_ALARM_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) RTCAlarm_IRQHandler(void) {

    if(EXTI_GetITStatus(EXTI_Line17)!=RESET) {
      ExtiRtcAlarm::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)17);
      EXTI_ClearITPendingBit(EXTI_Line17);
    }
  }
}

#endif

#endif
