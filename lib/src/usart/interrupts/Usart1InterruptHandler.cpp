/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> UsartInterruptFeatureEnabler<1>::FPTR UsartInterruptFeatureEnabler<1>::_forceLinkage=nullptr;
template<> UsartEventSource *UsartInterruptFeature<1> ::_usartInstance=nullptr;


extern "C" {

  #if defined(USE_USART1_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) USART1_IRQHandler(void) {
      if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_RECEIVE);
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
      }
      else if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_READY_TO_TRANSMIT);
        USART_ClearITPendingBit(USART1,USART_IT_TXE);
      }
      else if(USART_GetITStatus(USART1,USART_IT_TC)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_TRANSMIT_COMPLETE);
        USART_ClearITPendingBit(USART1,USART_IT_TC);
      }
      else if(USART_GetITStatus(USART1,USART_IT_PE)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_PARITY_ERROR);
        USART_ClearITPendingBit(USART1,USART_IT_PE);
      }
      else if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_IDLE);
        USART_ClearITPendingBit(USART1,USART_IT_IDLE);
      }
      else if(USART_GetITStatus(USART1,USART_IT_LBD)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_LINE_BREAK_DETECT);
        USART_ClearITPendingBit(USART1,USART_IT_LBD);
      }
      else if(USART_GetITStatus(USART1,USART_IT_CTS)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_CLEAR_TO_SEND);
        USART_ClearITPendingBit(USART1,USART_IT_CTS);
      }
      else if(USART_GetITStatus(USART1,USART_IT_ERR)!=RESET) {
        UsartInterruptFeature<1>::_usartInstance->UsartInterruptEventSender.raiseEvent(UsartEventType::EVENT_ERROR);
        USART_ClearITPendingBit(USART1,USART_IT_ERR);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

  #endif
}
