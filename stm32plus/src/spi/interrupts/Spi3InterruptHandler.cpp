/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/spi.h"
#include "config/i2s.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> SpiInterruptFeatureEnabler<3>::FPTR SpiInterruptFeatureEnabler<3>::_forceLinkage=nullptr;
template<> I2SInterruptFeatureEnabler<3>::FPTR I2SInterruptFeatureEnabler<3>::_forceLinkage=nullptr;


extern "C" {

#if defined(USE_SPI3_INTERRUPT) || defined(USE_I2S3_INTERRUPT)

  void __attribute__ ((interrupt("IRQ"))) SPI3_IRQHandler(void) {
    if(SPI_I2S_GetITStatus(SPI3,SPI_I2S_IT_RXNE)!=RESET) {
      SpiInterruptFeature<3>::_spiInstance->notifyObservers(ObservableEvent::SPI_Receive,(void *)3);
      SPI_I2S_ClearITPendingBit(SPI3,SPI_I2S_IT_RXNE);
    }
    else if(SPI_I2S_GetITStatus(SPI3,SPI_I2S_IT_TXE)!=RESET) {
      SpiInterruptFeature<3>::_spiInstance->notifyObservers(ObservableEvent::SPI_ReadyToTransmit,(void *)3);
      SPI_I2S_ClearITPendingBit(SPI3,SPI_I2S_IT_TXE);
    }
  }

#endif
} // extern "C"
