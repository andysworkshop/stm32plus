/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/timer.h"

#if defined(STM32PLUS_F4) || defined(STM32PLUS_F3)


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> TimerInterruptFeatureEnabler<8>::FPTR TimerInterruptFeatureEnabler<8>::_forceLinkage=nullptr;
template<> TimerInterruptFeatureEnabler<12>::FPTR TimerInterruptFeatureEnabler<12>::_forceLinkage=nullptr;
template<> TimerInterruptFeatureEnabler<13>::FPTR TimerInterruptFeatureEnabler<13>::_forceLinkage=nullptr;
template<> TimerInterruptFeatureEnabler<14>::FPTR TimerInterruptFeatureEnabler<14>::_forceLinkage=nullptr;


extern "C" {

  /**
   * TIM8_BRK and TIM12 share the same vector.
   * TIM12 can have CC1, CC2, Update, Trigger
   */

#if defined(USE_TIM8_BRK_INTERRUPT) || defined(USE_TIM12_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) TIM8_BRK_TIM12_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_Break)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Break);
      TIM_ClearITPendingBit(TIM8,TIM_IT_Break);
    }
    else if(TIM_GetITStatus(TIM12,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<12>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare1);
      TIM_ClearITPendingBit(TIM12,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM12,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<12>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare2);
      TIM_ClearITPendingBit(TIM12,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM12,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<12>::_timerInstance->notifyObservers(ObservableEvent::Timer_Update);
      TIM_ClearITPendingBit(TIM12,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM12,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<12>::_timerInstance->notifyObservers(ObservableEvent::Timer_Trigger);
      TIM_ClearITPendingBit(TIM12,TIM_IT_Trigger);
    }
  }

#endif

  /**
   * TIM8_UP and TIM13 share the same vector.
   * TIM13 can have CC1, Update
   */

#if defined(USE_TIM8_UP_INTERRUPT) || defined(USE_TIM13_INTERRUPT)

  void __attribute__ ((interrupt)) TIM8_UP_TIM13_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Update);
      TIM_ClearITPendingBit(TIM8,TIM_IT_Update);
    }
    else if(TIM_GetITStatus(TIM13,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<13>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare1);
      TIM_ClearITPendingBit(TIM13,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM13,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<13>::_timerInstance->notifyObservers(ObservableEvent::Timer_Update);
      TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
    }
  }

#endif

  /**
   * TIM8_TRG_COM and TIM14 share the same vector.
   * TIM14 can have CC1, Update
   */

#if defined(USE_TIM8_TRG_COM_INTERRUPT) || defined(USE_TIM14_INTERRUPT)

  void __attribute__ ((interrupt)) TIM8_TRG_COM_TIM14_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_Trigger)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Trigger);
      TIM_ClearITPendingBit(TIM8,TIM_IT_Trigger);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_COM)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Com);
      TIM_ClearITPendingBit(TIM8,TIM_IT_COM);
    }
    else if(TIM_GetITStatus(TIM14,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<14>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare1);
      TIM_ClearITPendingBit(TIM14,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM14,TIM_IT_Update)!=RESET) {
      TimerInterruptFeature<14>::_timerInstance->notifyObservers(ObservableEvent::Timer_Update);
      TIM_ClearITPendingBit(TIM14,TIM_IT_Update);
    }
  }

#endif

  /**
   * TIM8_CC interrupt handler
   */

#if defined(USE_TIM8_CC_INTERRUPT)

  void __attribute__ ((interrupt)) TIM8_CC_IRQHandler() {

    if(TIM_GetITStatus(TIM8,TIM_IT_CC1)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare1);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC2)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare2);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC2);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare3);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);
    }
    else if(TIM_GetITStatus(TIM8,TIM_IT_CC3)!=RESET) {
      TimerInterruptFeature<8>::_timerInstance->notifyObservers(ObservableEvent::Timer_Compare3);
      TIM_ClearITPendingBit(TIM8,TIM_IT_CC3);
    }
  }

#endif
} // extern "C"

#endif
