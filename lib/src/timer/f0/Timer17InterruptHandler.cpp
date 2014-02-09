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

template<> TimerInterruptFeatureEnabler<17>::FPTR TimerInterruptFeatureEnabler<17>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM17 interrupt handler
   * TIM17 can have CC1..2, Update, Trigger, Break, COM
   */

#if defined(USE_TIM17_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM17_IRQHandler() {

    if(TIM_GetITStatus(TIM17,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<17>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,17);
      TIM_ClearITPendingBit(TIM17,TIM_IT_CC1);
    }
    if(TIM_GetITStatus(TIM17,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<17>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,17);
      TIM_ClearITPendingBit(TIM17,TIM_IT_CC2);
    }
    if(TIM_GetITStatus(TIM17,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<17>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,17);
      TIM_ClearITPendingBit(TIM17,TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM17,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<17>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,17);
      TIM_ClearITPendingBit(TIM17,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"


#endif
