/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"


// this is only for the F4

#if defined(STM32PLUS_F4) || defined(STM32PLUS_F3)

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> DmaInterruptFeatureEnabler<1,4>::FPTR DmaInterruptFeatureEnabler<1,4>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA1, stream 4
   */

  #if defined(USE_DMA1_4_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA1_Stream4_IRQHandler() {

      if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TCIF4)!=RESET) {
        DmaInterruptFeature<1,4>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00010004);
        DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
      }
      else if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_HTIF4)!=RESET) {
        DmaInterruptFeature<1,4>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00010004);
        DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_HTIF4);
      }
      else if(DMA_GetITStatus(DMA1_Stream4,DMA_IT_TEIF4)!=RESET) {
        DmaInterruptFeature<1,4>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00010004);
        DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TEIF4);
      }
    }
  #endif
}
#endif // STM32PLUS_F4
