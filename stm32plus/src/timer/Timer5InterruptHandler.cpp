/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


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
      TimerInterruptFeature<5>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare1);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare2);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare3);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC3);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_CC4)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare4);
      TIM_ClearITPendingBit(TIM5,TIM_IT_CC4);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->notifyObservers(ObservableEvent::Timer_Update);
      TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM5,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<5>::_timerInstance->notifyObservers(ObservableEvent::Timer_Trigger);
      TIM_ClearITPendingBit(TIM5,TIM_IT_Trigger);
    }
  }

#endif

} // extern "C"
