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

template<> DmaInterruptFeatureEnabler<2,1>::FPTR DmaInterruptFeatureEnabler<2,1>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA2, stream 1
   */

  #if defined(USE_DMA2_1_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA2_Stream1_IRQHandler() {

      if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1)!=RESET) {
        DmaInterruptFeature<2,1>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00020001);
        DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
      }
      else if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_HTIF1)!=RESET) {
        DmaInterruptFeature<2,1>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00020001);
        DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_HTIF1);
      }
      else if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TEIF1)!=RESET) {
        DmaInterruptFeature<2,1>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00020001);
        DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TEIF1);
      }
    }
  #endif
}
#endif // STM32PLUS_F4
