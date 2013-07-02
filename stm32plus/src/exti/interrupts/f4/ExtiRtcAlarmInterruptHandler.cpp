/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/exti.h"


#if defined(STM32PLUS_F4) || defined(STM32PLUS_F3)


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiInterruptEnabler<17>::FPTR ExtiInterruptEnabler<17>::_forceLinkage=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line17>::_extiInstance=nullptr;


#if defined(USE_EXTI_RTC_ALARM_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) RTC_Alarm_IRQHandler(void) {

    if(RTC_GetITStatus(RTC_IT_ALRA)!=RESET) {
      ExtiRtcAlarm::_extiInstance->notifyObservers(ObservableEvent::RTC_AlarmA,(void *)17);
      RTC_ClearITPendingBit(RTC_IT_ALRA);
      EXTI_ClearITPendingBit(EXTI_Line17);
    }
    else if(RTC_GetITStatus(RTC_IT_ALRB)!=RESET) {
      ExtiRtcAlarm::_extiInstance->notifyObservers(ObservableEvent::RTC_AlarmB,(void *)17);
      RTC_ClearITPendingBit(RTC_IT_ALRB);
      EXTI_ClearITPendingBit(EXTI_Line17);
    }
  }

  void RTCAlarm_IRQHandler(void) {
    RTC_Alarm_IRQHandler();
  }

}

#endif
#endif

