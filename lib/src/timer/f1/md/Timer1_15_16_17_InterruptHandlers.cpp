/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


#if defined(STM32PLUS_F1_MD)

using namespace stm32plus;

// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<1>::FPTR TimerInterruptFeatureEnabler<1>::_forceLinkage=nullptr;


extern "C" {

#if defined(USE_TIM1_UP_TIM16_INTERRUPT)

  /**
   * TIM1_UP & TIM16
   */

  void __attribute__ ((interrupt("IRQ"))) TIM1_UP_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM16,TIM_IT_Break)!=RESET) {
      TimerInterruptFeature<16>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_BREAK,16);
      TIM_ClearITPendingBit(TIM16,TIM_IT_Break);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM1_BRK & TIM15
   */

#if defined(USE_TIM1_BRK_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM1_BRK_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Break)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_BREAK,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Break);
    }
    else if(TIM_GetITStatus(TIM15,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<15>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,15);
      TIM_ClearITPendingBit(TIM15,TIM_IT_Update);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM1_TRG_COM & TIM17
   */

#if defined(USE_TIM1_TRG_COM_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM1_TRG_COM_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Trigger);
    }
    else if(TIM_GetITStatus(TIM1,TIM_IT_COM)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COM,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_COM);
    }
    else if(TIM_GetITStatus(TIM17,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<17>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,17);
      TIM_ClearITPendingBit(TIM17,TIM_IT_Update);
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
    else if(TIM_GetITStatus(TIM1,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM1,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC3);
    }
    else if(TIM_GetITStatus(TIM1,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_CC3);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"

#endif
