/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/exti.h"


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiPeripheral<EXTI_Line16> *ExtiPeripheral<EXTI_Line16>::_extiInstance=nullptr;


#if defined(USE_EXTI_PVD_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) PVD_IRQHandler(void) {

    if(EXTI_GetITStatus(EXTI_Line16)!=RESET) {
        ExtiPvd::_extiInstance->ExtiInterruptEventSender.raiseEvent(16);
        EXTI_ClearITPendingBit(EXTI_Line16);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif
