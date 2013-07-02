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

template<> DmaInterruptFeatureEnabler<2,3>::FPTR DmaInterruptFeatureEnabler<2,3>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA2, channel 3
   */

  #if defined(USE_DMA2_3_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA2_Channel3_IRQHandler() {

      if(DMA_GetITStatus(DMA2_IT_TC3)!=RESET) {
        DmaInterruptFeature<2,3>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00020003);
        DMA_ClearITPendingBit(DMA2_IT_TC3);
      }
      else if(DMA_GetITStatus(DMA2_IT_HT3)!=RESET) {
        DmaInterruptFeature<2,3>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00020003);
        DMA_ClearITPendingBit(DMA2_IT_HT3);
      }
      else if(DMA_GetITStatus(DMA2_IT_TE3)!=RESET) {
        DmaInterruptFeature<2,3>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00020003);
        DMA_ClearITPendingBit(DMA2_IT_TE3);
      }
    }
  #endif
}
#endif // STM32PLUS_F1

