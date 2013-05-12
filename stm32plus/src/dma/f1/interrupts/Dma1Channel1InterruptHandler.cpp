/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"


// this is only for the F1

#ifdef STM32PLUS_F1

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> DmaInterruptFeatureEnabler<1,1>::FPTR DmaInterruptFeatureEnabler<1,1>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA1, channel 1
   */

  #if defined(USE_DMA1_1_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA1_Channel1_IRQHandler() {

      if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET) {
        DmaInterruptFeature<1,1>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00010001);
        DMA_ClearITPendingBit(DMA1_IT_TC1);
      }
      else if(DMA_GetITStatus(DMA1_IT_HT1)!=RESET) {
        DmaInterruptFeature<1,1>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00010001);
        DMA_ClearITPendingBit(DMA1_IT_HT1);
      }
      else if(DMA_GetITStatus(DMA1_IT_TE1)!=RESET) {
        DmaInterruptFeature<1,1>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00010001);
        DMA_ClearITPendingBit(DMA1_IT_TE1);
      }
    }
  #endif
}
#endif // STM32PLUS_F1
