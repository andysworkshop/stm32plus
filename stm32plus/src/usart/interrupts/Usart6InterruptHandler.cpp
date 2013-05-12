/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


#if defined(STM32PLUS_F4)

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> UsartInterruptFeatureEnabler<6>::FPTR UsartInterruptFeatureEnabler<6>::_forceLinkage=nullptr;
template<> Observable *UsartInterruptFeature<6>::_usartInstance=nullptr;


extern "C" {

  #if defined(USE_USART6_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) USART6_IRQHandler(void) {
      if(USART_GetITStatus(USART6,USART_IT_RXNE)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_Receive,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_RXNE);
      }
      else if(USART_GetITStatus(USART6,USART_IT_TXE)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_ReadyToTransmit,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_TXE);
      }
      else if(USART_GetITStatus(USART6,USART_IT_TC)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_TransmitComplete,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_TC);
      }
      else if(USART_GetITStatus(USART6,USART_IT_PE)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_ParityError,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_PE);
      }
      else if(USART_GetITStatus(USART6,USART_IT_IDLE)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_Idle,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_IDLE);
      }
      else if(USART_GetITStatus(USART6,USART_IT_LBD)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_LineBreakDetect,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_LBD);
      }
      else if(USART_GetITStatus(USART6,USART_IT_CTS)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_ClearToSend,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_CTS);
      }
      else if(USART_GetITStatus(USART6,USART_IT_ERR)!=RESET) {
        UsartInterruptFeature<6>::_usartInstance->notifyObservers(ObservableEvent::USART_Error,(void *)6);
        USART_ClearITPendingBit(USART6,USART_IT_ERR);
      }
    }

  #endif
}

#endif
