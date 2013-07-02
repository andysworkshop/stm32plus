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

template<> DmaInterruptFeatureEnabler<1,6>::FPTR DmaInterruptFeatureEnabler<1,6>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA1, channel 6
   */

  #if defined(USE_DMA1_6_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA1_Channel6_IRQHandler() {

      if(DMA_GetITStatus(DMA1_IT_TC6)!=RESET) {
        DmaInterruptFeature<1,6>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00010006);
        DMA_ClearITPendingBit(DMA1_IT_TC6);
      }
      else if(DMA_GetITStatus(DMA1_IT_HT6)!=RESET) {
        DmaInterruptFeature<1,6>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00010006);
        DMA_ClearITPendingBit(DMA1_IT_HT6);
      }
      else if(DMA_GetITStatus(DMA1_IT_TE6)!=RESET) {
        DmaInterruptFeature<1,6>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00010006);
        DMA_ClearITPendingBit(DMA1_IT_TE6);
      }
    }
  #endif
}
#endif // STM32PLUS_F1
