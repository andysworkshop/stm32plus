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

template<> TimerInterruptFeatureEnabler<16>::FPTR TimerInterruptFeatureEnabler<16>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM16 interrupt handler
   * TIM16 can have CC1..2, Update, Trigger, Break, COM
   */

#if defined(USE_TIM16_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM16_IRQHandler() {

    if(TIM_GetITStatus(TIM16,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<16>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE1,16);
      TIM_ClearITPendingBit(TIM16,TIM_IT_CC1);
    }
    if(TIM_GetITStatus(TIM16,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<16>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_COMPARE2,16);
      TIM_ClearITPendingBit(TIM16,TIM_IT_CC2);
    }
    if(TIM_GetITStatus(TIM16,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<16>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,16);
      TIM_ClearITPendingBit(TIM16,TIM_IT_Update);
    }
    if(TIM_GetITStatus(TIM16,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<16>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_TRIGGER,16);
      TIM_ClearITPendingBit(TIM16,TIM_IT_Trigger);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"


#endif
