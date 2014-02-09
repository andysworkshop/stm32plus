/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<2>::FPTR TimerInterruptFeatureEnabler<2>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM2 interrupt handler
   * TIM2 can have CC1..4, Update, Trigger
   */

#if defined(USE_TIM2_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM2_IRQHandler() {

    if(TIM_GetITStatus(TIM2,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<2>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,2);
      TIM_ClearITPendingBit(TIM2,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM2,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<2>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,2);
      TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM2,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<2>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,2);
      TIM_ClearITPendingBit(TIM2,TIM_IT_CC3);
    }
    else if(TIM_GetITStatus(TIM2,TIM_IT_CC4)!=RESET) {
      TimerInterruptFeature<2>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE4,2);
      TIM_ClearITPendingBit(TIM2,TIM_IT_CC4);
    }
    else if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<2>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,2);
      TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM2,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<2>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,2);
      TIM_ClearITPendingBit(TIM2,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif


} // extern "C"
