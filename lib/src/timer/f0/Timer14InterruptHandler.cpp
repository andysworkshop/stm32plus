/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


#if defined(STM32PLUS_F0)


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<14>::FPTR TimerInterruptFeatureEnabler<14>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM14 interrupt handler
   * TIM14 can have CC1..2, Update, Trigger, Break, COM
   */

#if defined(USE_TIM14_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM14_IRQHandler() {

    if(TIM_GetITStatus(TIM14,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<14>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,14);
      TIM_ClearITPendingBit(TIM14,TIM_IT_CC1);
    }
    if(TIM_GetITStatus(TIM14,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<14>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,14);
      TIM_ClearITPendingBit(TIM14,TIM_IT_CC2);
    }
    if(TIM_GetITStatus(TIM14,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<14>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,14);
      TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM14,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<14>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,14);
      TIM_ClearITPendingBit(TIM14,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"


#endif
