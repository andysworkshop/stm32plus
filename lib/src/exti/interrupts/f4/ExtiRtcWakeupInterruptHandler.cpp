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

template<> ExtiPeripheral<EXTI_Line22> *ExtiPeripheral<EXTI_Line22>::_extiInstance=nullptr;


#if defined(USE_EXTI_RTC_WAKEUP_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) RTC_WKUP_IRQHandler(void) {

    if(RTC_GetITStatus(RTC_IT_WUT)!=RESET) {
      ExtiRtcWakeup::_extiInstance->ExtiInterruptEventSender.raiseEvent(22);
      EXTI_ClearITPendingBit(EXTI_Line22);
      RTC_ClearITPendingBit(RTC_IT_WUT);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif
#endif
