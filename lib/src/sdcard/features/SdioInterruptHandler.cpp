/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F1_HD) || defined(STM32PLUS_F4)

#include "config/sdcard.h"


using namespace stm32plus;


// initialise the static member

SdioEventSource *SdioInterruptFeature::_sdioInstance=nullptr;

// static initialisers for the hack that forces the IRQ handlers to be linked

SdioInterruptFeature::FPTR SdioInterruptFeature::_forceLinkage=nullptr;


#if defined(USE_SDIO_INTERRUPT)

extern "C" {

    void __attribute__ ((interrupt("IRQ"))) SDIO_IRQHandler(void) {

    if(SDIO_GetITStatus(SDIO_IT_DATAEND) != RESET) {

      SdioInterruptFeature::_sdioInstance->SdioInterruptEventSender.raiseEvent(SdioEventType::EVENT_DATA_END);
      SDIO_ClearITPendingBit(SDIO_IT_DATAEND);

    } else if(SDIO_GetITStatus(SDIO_IT_DCRCFAIL) != RESET) {

      SdioInterruptFeature::_sdioInstance->SdioInterruptEventSender.raiseEvent(SdioEventType::EVENT_CRC_FAIL);
      SDIO_ClearITPendingBit(SDIO_IT_DCRCFAIL);

    } else if(SDIO_GetITStatus(SDIO_IT_DTIMEOUT) != RESET) {

      SdioInterruptFeature::_sdioInstance->SdioInterruptEventSender.raiseEvent(SdioEventType::EVENT_TIMEOUT);
      SDIO_ClearITPendingBit(SDIO_IT_DTIMEOUT);

    } else if(SDIO_GetITStatus(SDIO_IT_RXOVERR) != RESET) {

      SdioInterruptFeature::_sdioInstance->SdioInterruptEventSender.raiseEvent(SdioEventType::EVENT_RX_OVERRUN);
      SDIO_ClearITPendingBit(SDIO_IT_RXOVERR);

    } else if(SDIO_GetITStatus(SDIO_IT_TXUNDERR) != RESET) {

      SdioInterruptFeature::_sdioInstance->SdioInterruptEventSender.raiseEvent(SdioEventType::EVENT_TX_OVERRUN);
      SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);

    } else if(SDIO_GetITStatus(SDIO_IT_STBITERR) != RESET) {

      SdioInterruptFeature::_sdioInstance->SdioInterruptEventSender.raiseEvent(SdioEventType::EVENT_START_BIT_ERROR);
      SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
    }
    __DSB();      // prevent erroneous recall of this handler due to delayed memory write
  }
}

#endif // extern "C"

#endif
