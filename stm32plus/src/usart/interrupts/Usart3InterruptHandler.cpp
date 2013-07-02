/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/usart.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> UsartInterruptFeatureEnabler<3>::FPTR UsartInterruptFeatureEnabler<3>::_forceLinkage=nullptr;
template<> Observable *UsartInterruptFeature<3>::_usartInstance=nullptr;


extern "C" {

  #if defined(USE_USART3_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) USART3_IRQHandler(void) {
      if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_Receive,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);
      }
      else if(USART_GetITStatus(USART3,USART_IT_TXE)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_ReadyToTransmit,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_TXE);
      }
      else if(USART_GetITStatus(USART3,USART_IT_TC)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_TransmitComplete,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_TC);
      }
      else if(USART_GetITStatus(USART3,USART_IT_PE)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_ParityError,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_PE);
      }
      else if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_Idle,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_IDLE);
      }
      else if(USART_GetITStatus(USART3,USART_IT_LBD)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_LineBreakDetect,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_LBD);
      }
      else if(USART_GetITStatus(USART3,USART_IT_CTS)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_ClearToSend,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_CTS);
      }
      else if(USART_GetITStatus(USART3,USART_IT_ERR)!=RESET) {
        UsartInterruptFeature<3>::_usartInstance->notifyObservers(ObservableEvent::USART_Error,(void *)3);
        USART_ClearITPendingBit(USART3,USART_IT_ERR);
      }
    }

  #endif
}
