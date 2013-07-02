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

template<> DmaInterruptFeatureEnabler<2,4>::FPTR DmaInterruptFeatureEnabler<2,4>::_forceLinkage=nullptr;
template<> DmaInterruptFeatureEnabler<2,5>::FPTR DmaInterruptFeatureEnabler<2,5>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA2, channel 4_5
   */

  #if defined(USE_DMA2_4_5_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA2_Channel4_5_IRQHandler() {

      if(DMA_GetITStatus(DMA2_IT_TC4)!=RESET) {
        DmaInterruptFeature<2,4>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00020004);
        DMA_ClearITPendingBit(DMA2_IT_TC4);
      }
      else if(DMA_GetITStatus(DMA2_IT_HT4)!=RESET) {
        DmaInterruptFeature<2,4>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00020004);
        DMA_ClearITPendingBit(DMA2_IT_HT4);
      }
      else if(DMA_GetITStatus(DMA2_IT_TE4)!=RESET) {
        DmaInterruptFeature<2,4>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00020004);
        DMA_ClearITPendingBit(DMA2_IT_TE4);
      }

      if(DMA_GetITStatus(DMA2_IT_TC5)!=RESET) {
        DmaInterruptFeature<2,5>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferComplete,(void *)0x00020005);
        DMA_ClearITPendingBit(DMA2_IT_TC5);
      }
      else if(DMA_GetITStatus(DMA2_IT_HT5)!=RESET) {
        DmaInterruptFeature<2,5>::_dmaInstance->notifyObservers(ObservableEvent::DMA_HalfTransfer,(void *)0x00020005);
        DMA_ClearITPendingBit(DMA2_IT_HT5);
      }
      else if(DMA_GetITStatus(DMA2_IT_TE5)!=RESET) {
        DmaInterruptFeature<2,5>::_dmaInstance->notifyObservers(ObservableEvent::DMA_TransferError,(void *)0x00020005);
        DMA_ClearITPendingBit(DMA2_IT_TE5);
      }
    }
  #endif
}

#endif // STM32PLUS_F1
