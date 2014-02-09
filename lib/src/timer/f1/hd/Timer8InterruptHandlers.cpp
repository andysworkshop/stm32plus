/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"


#if defined(STM32PLUS_F1_HD)


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<8>::FPTR TimerInterruptFeatureEnabler<8>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM8_BRK
   */

#if defined(USE_TIM8_BRK_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM8_BRK_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_Break)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_BREAK,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_Break);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM8_UP
   */

#if defined(USE_TIM8_UP_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM8_UP_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM8_TRG_COM
   */

#if defined(USE_TIM8_TRG_COM_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM8_TRG_COM_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_Trigger);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_COM)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COM,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_COM);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

  /**
   * TIM8_CC interrupt handler
   */

#if defined(USE_TIM8_CC_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM8_CC_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE3,8);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif
} // extern "C"

#endif
