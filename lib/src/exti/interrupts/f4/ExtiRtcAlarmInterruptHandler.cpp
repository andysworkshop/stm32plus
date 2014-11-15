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

template<> ExtiPeripheral<EXTI_Line17> *ExtiPeripheral<EXTI_Line17>::_extiInstance=nullptr;


#if defined(USE_EXTI_RTC_ALARM_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) RTC_Alarm_IRQHandler(void) {

    if(RTC_GetITStatus(RTC_IT_ALRA)!=RESET) {
      ExtiRtcAlarm::_extiInstance->ExtiInterruptEventSender.raiseEvent(17);
      RTC_ClearITPendingBit(RTC_IT_ALRA);
      EXTI_ClearITPendingBit(EXTI_Line17);
    }
    else if(RTC_GetITStatus(RTC_IT_ALRB)!=RESET) {
      ExtiRtcAlarm::_extiInstance->ExtiInterruptEventSender.raiseEvent(17);
      RTC_ClearITPendingBit(RTC_IT_ALRB);
      EXTI_ClearITPendingBit(EXTI_Line17);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

  void RTCAlarm_IRQHandler(void) {
    RTC_Alarm_IRQHandler();
  }

}

#endif
#endif

