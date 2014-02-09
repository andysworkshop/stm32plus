/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


#if defined(STM32PLUS_F1)


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<5>::FPTR TimerInterruptFeatureEnabler<5>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM5 interrupt handler
   * TIM5 can have CC1..4, Update, Trigger
   */

#if defined(USE_TIM5_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM5_IRQHandler() {

    if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,5);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,5);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,5);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC3);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_CC4)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE4,5);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC4);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,5);
      TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,5);
      TIM_ClearITPendingBit(TIM5,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"


#endif
