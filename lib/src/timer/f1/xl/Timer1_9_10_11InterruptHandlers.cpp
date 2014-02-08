/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


#if defined(STM32PLUS_F1_XL)

using namespace stm32plus;

// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<1>::FPTR TimerInterruptFeatureEnabler<1>::_forceLinkage=nullptr;
template<> TimerInterruptFeatureEnabler<9>::FPTR TimerInterruptFeatureEnabler<9>::_forceLinkage=nullptr;
template<> TimerInterruptFeatureEnabler<10>::FPTR TimerInterruptFeatureEnabler<10>::_forceLinkage=nullptr;


extern "C" {

#if defined(USE_TIM1_UP_INTERRUPT) || defined(USE_TIM10_INTERRUPT)

  /**
   * TIM1_UP and TIM10 (XL-density only) share the same vector.
   * TIM10 can have CC1, Update
   */

  void __attribute__ ((interrupt("IRQ"))) TIM1_UP_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM10,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<10>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,10);
      TIM_ClearITPendingBit(TIM10,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM10,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<10>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,10);
      TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM1_BRK and TIM9 (XL-density only) share the same vector.
   * TIM9 can have CC1, CC2, Update, Trigger
   */

#if defined(USE_TIM1_BRK_INTERRUPT) || defined(USE_TIM9_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM1_BRK_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Break)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_BREAK,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Break);
    }
    else if(TIM_GetITStatus(TIM9,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<9>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,9);
      TIM_ClearITPendingBit(TIM9,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM9,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<9>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,9);
      TIM_ClearITPendingBit(TIM9,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM9,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<9>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,9);
      TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM9,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<9>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,9);
      TIM_ClearITPendingBit(TIM9,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM1_TRG_COM and TIM11 (XL-density only) share the same vector.
   * TIM11 can have CC1, Update
   */

#if defined(USE_TIM1_TRG_COM_INTERRUPT) || defined(USE_TIM11_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM1_TRG_COM_IRQHandler() {

    if(TIM_GetITStatus(TIM1,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_Trigger);
    }
    else if(TIM_GetITStatus(TIM1,TIM_IT_COM)!=RESET) {
      TimerInterruptFeature<1>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COM,1);
      TIM_ClearITPendingBit(TIM1,TIM_IT_COM);
    }
    else if(TIM_GetITStatus(TIM11,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<11>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,11);
      TIM_ClearITPendingBit(TIM11,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM11,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<11>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,11);
      TIM_ClearITPendingBit(TIM11,TIM_IT_Update);
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
