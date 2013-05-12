/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"


// this is only for the F4

#ifdef STM32PLUS_F4

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> DmaInterruptFeatureEnabler<2,2>::FPTR DmaInterruptFeatureEnabler<2,2>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA2, stream 2
   */

  #if defined(USE_DMA2_2_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA2_Stream2_IRQHandler() {

      if(DMA_GetITStatus(DMA2_Stream2,DMA_IT_TCIF2)!=RESET) {
        DmaInterruptFeature<2,2>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00020002);
        DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_TCIF2);
      }
      else if(DMA_GetITStatus(DMA2_Stream2,DMA_IT_HTIF2)!=RESET) {
        DmaInterruptFeature<2,2>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00020002);
        DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_HTIF2);
      }
      else if(DMA_GetITStatus(DMA2_Stream2,DMA_IT_TEIF2)!=RESET) {
        DmaInterruptFeature<2,2>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00020002);
        DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TEIF2);
      }
    }
  #endif
}
#endif // STM32PLUS_F4
