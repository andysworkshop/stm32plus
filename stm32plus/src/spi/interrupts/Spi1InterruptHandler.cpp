/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> SpiInterruptFeatureEnabler<1>::FPTR SpiInterruptFeatureEnabler<1>::_forceLinkage=nullptr;


#if defined(USE_SPI1_INTERRUPT)

extern "C" {


    void __attribute__ ((interrupt("IRQ"))) SPI1_IRQHandler(void) {
      if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE)!=RESET) {
        SpiInterruptFeature<1>::_spiInstance->notifyObservers(ObservableEvent::SPI_Receive,(void *)1);
        SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);
      }
      else if(SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_TXE)!=RESET) {
        SpiInterruptFeature<1>::_spiInstance->notifyObservers(ObservableEvent::SPI_ReadyToTransmit,(void *)1);
        SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_TXE);
      }
    }

}

#endif // extern "C"
