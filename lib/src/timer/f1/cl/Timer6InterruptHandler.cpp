/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"

#if defined(STM32PLUS_F1_CL)


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<6>::FPTR TimerInterruptFeatureEnabler<6>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM6 interrupt handler
   * TIM6 can have Update
   */

#if defined(USE_TIM6_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM6_IRQHandler() {

    if(TIM_GetITStatus(TIM6,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<6>::_timerInstance->TimerInterruptEventSender.raiseEvent(TimerEventType::EVENT_UPDATE,6);
      TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }

#endif

} // extern "C"

#endif
