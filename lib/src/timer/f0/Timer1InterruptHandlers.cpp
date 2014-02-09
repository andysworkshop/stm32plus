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

template<> TimerInterruptFeatureEnabler<1>::FPTR TimerInterruptFeatureEnabler<1>::_forceLinkage=nullptr;


extern "C" {

#if defined(USE_TIM1_BRK_UP_TRG_COM_INTERRUPT)

  /**
   * TIM1_UP
   */

  void __attribute__ ((interrupt("IRQ"))) TIM1_BRK_UP_TRG_COM_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Break)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_BREAK,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Break);
    }
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM1,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Trigger);
    }
    if(TIM_GetITStatus(TIM1,TIM_IT_COM)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COM,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_COM);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif


  /**
   * TIM1_CC interrupt handler
   */

#if defined(USE_TIM1_CC_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM1_CC_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
    }
    if(TIM_GetITStatus(TIM1,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC2);
    }
    if(TIM_GetITStatus(TIM1,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC3);
    }
    if(TIM_GetITStatus(TIM1,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC3);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"

#endif
