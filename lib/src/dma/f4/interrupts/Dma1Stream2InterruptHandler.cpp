/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"


// this is only for the F4

#ifdef STM32PLUS_F4

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> DmaInterruptFeatureEnabler<1,2>::FPTR DmaInterruptFeatureEnabler<1,2>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA1, stream 2
   */

  #if defined(USE_DMA1_2_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA1_Stream2_IRQHandler() {

      if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_TCIF2)!=RESET) {
        DmaInterruptFeature<1,2>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_COMPLETE);
        DMA_ClearITPendingBit(DMA1_Stream2,DMA_IT_TCIF2);
      }
      else if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_HTIF2)!=RESET) {
        DmaInterruptFeature<1,2>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_HALF_COMPLETE);
        DMA_ClearITPendingBit(DMA1_Stream2,DMA_IT_HTIF2);
      }
      else if(DMA_GetITStatus(DMA1_Stream2,DMA_IT_TEIF2)!=RESET) {
        DmaInterruptFeature<1,2>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_TRANSFER_ERROR);
        DMA_ClearITPendingBit(DMA1_Stream2,DMA_IT_TEIF2);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }
  #endif
}
#endif // STM32PLUS_F4
