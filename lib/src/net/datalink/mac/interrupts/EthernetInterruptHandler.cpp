/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4_HAS_MAC) || defined(STM32PLUS_F1_CL_E)

#include "config/net.h"


using namespace stm32plus::net;


/**
 * Static instance pointer back into the mac class
 */

MacBase *MacBase::_instance=NULL;


extern "C" {

  /**
   * Ethernet IRQ handler
   */

  void __attribute__ ((interrupt("IRQ"))) ETH_IRQHandler(void) {

    // receive interrupt

    if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R)==SET) {

      // clear the interrupt flags before servicing the interrupt. This will prevent a race condition
      // where an interrupt is raised in the short period between us finishing processing and then
      // clearing the interrupt flag before returning. The drawback is that we may receive an interrupt
      // for an already-processed frame which we will detect and return quickly with no harm done.

      ETH_DMAClearITPendingBit(ETH_DMA_IT_R | ETH_DMA_IT_NIS);      // NIS = normal interrupt summary
      MacBase::_instance->handleReceiveInterrupt();
    }

    // transmit interrupt

    if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_T)==SET) {

      // clear the interrupt flags and process the interrupt. Again this may result in an an
      // un-necessary, harmless interrupt as we avoid the race condition that would occur because
      // we cannot atomically finish processing and clear the interrupt flag.

      ETH_DMAClearITPendingBit(ETH_DMA_IT_T | ETH_DMA_IT_NIS);      // NIS = normal interrupt summary
      MacBase::_instance->handleTransmitInterrupt();
    }

    if(ETH_GetDMAFlagStatus(ETH_DMA_FLAG_AIS)==SET) {   // AIS = abnormal interrupt summary

      MacBase::_instance->handleErrorInterrupt(ETH->DMASR);

      // clear the interrupt flags

      ETH_DMAClearITPendingBit(
          ETH_DMA_IT_AIS | ETH_DMA_IT_TPS | ETH_DMA_IT_TJT | ETH_DMA_IT_RO | ETH_DMA_IT_TU |
          ETH_DMA_IT_RBU | ETH_DMA_IT_RPS | ETH_DMA_IT_RWT | ETH_DMA_IT_ET | ETH_DMA_IT_FBE);
    }

    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}


#endif
