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

template<> DmaInterruptFeatureEnabler<1,7>::FPTR DmaInterruptFeatureEnabler<1,7>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA1, channel 7
   */

  #if defined(USE_DMA1_7_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA1_Channel7_IRQHandler() {

      if(DMA_GetITStatus(DMA1_IT_TC7)!=RESET) {
        DmaInterruptFeature<1,7>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00010007);
        DMA_ClearITPendingBit(DMA1_IT_TC7);
      }
      else if(DMA_GetITStatus(DMA1_IT_HT7)!=RESET) {
        DmaInterruptFeature<1,7>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00010007);
        DMA_ClearITPendingBit(DMA1_IT_HT7);
      }
      else if(DMA_GetITStatus(DMA1_IT_TE7)!=RESET) {
        DmaInterruptFeature<1,7>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00010007);
        DMA_ClearITPendingBit(DMA1_IT_TE7);
      }
    }
  #endif
}
#endif // STM32PLUS_F1
