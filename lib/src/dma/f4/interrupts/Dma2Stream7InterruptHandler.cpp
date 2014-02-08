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

template<> DmaInterruptFeatureEnabler<2,7>::FPTR DmaInterruptFeatureEnabler<2,7>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA2, stream 7
   */

  #if defined(USE_DMA2_7_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA2_Stream7_IRQHandler() {

      if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)!=RESET) {
        DmaInterruptFeature<2,7>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_COMPLETE);
        DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
      }
      else if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_HTIF7)!=RESET) {
        DmaInterruptFeature<2,7>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_HALF_COMPLETE);
        DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_HTIF7);
      }
      else if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TEIF7)!=RESET) {
        DmaInterruptFeature<2,7>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_TRANSFER_ERROR);
        DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TEIF7);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }
  #endif
}
#endif // STM32PLUS_F4
