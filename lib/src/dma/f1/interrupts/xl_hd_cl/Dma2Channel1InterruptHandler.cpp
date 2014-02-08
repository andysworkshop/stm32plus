/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/dma.h"


// this is only for the F1 XL/HD/CL

#if defined(STM32PLUS_F1_XL) || defined(STM32PLUS_F1_HD) || defined(STM32PLUS_F1_CL)

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> DmaInterruptFeatureEnabler<2,1>::FPTR DmaInterruptFeatureEnabler<2,1>::_forceLinkage=nullptr;


extern "C" {

  /**
   * DMA2, channel 1
   */

  #if defined(USE_DMA2_1_INTERRUPT)
    void __attribute__ ((interrupt("IRQ"))) DMA2_Channel1_IRQHandler() {

      if(DMA_GetITStatus(DMA2_IT_TC1)!=RESET) {
        DmaInterruptFeature<2,1>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_COMPLETE);
        DMA_ClearITPendingBit(DMA2_IT_TC1);
      }
      else if(DMA_GetITStatus(DMA2_IT_HT1)!=RESET) {
        DmaInterruptFeature<2,1>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_HALF_COMPLETE);
        DMA_ClearITPendingBit(DMA2_IT_HT1);
      }
      else if(DMA_GetITStatus(DMA2_IT_TE1)!=RESET) {
        DmaInterruptFeature<2,1>::_dmaInstance->DmaInterruptEventSender.raiseEvent(DmaEventType::EVENT_TRANSFER_ERROR);
        DMA_ClearITPendingBit(DMA2_IT_TE1);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }
  #endif
}
#endif // STM32PLUS_F1
