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

template<> TimerInterruptFeatureEnabler<15>::FPTR TimerInterruptFeatureEnabler<15>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM15 interrupt handler
   * TIM15 can have CC1..2, Update, Trigger, Break, COM
   */

#if defined(USE_TIM15_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM15_IRQHandler() {

    if(TIM_GetITStatus(TIM15,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<15>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,15);
      TIM_ClearITPendingBit(TIM15,TIM_IT_CC1);
    }
    if(TIM_GetITStatus(TIM15,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<15>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,15);
      TIM_ClearITPendingBit(TIM15,TIM_IT_CC2);
    }
    if(TIM_GetITStatus(TIM15,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<15>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,15);
      TIM_ClearITPendingBit(TIM15,TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM15,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<15>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,15);
      TIM_ClearITPendingBit(TIM15,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"


#endif
