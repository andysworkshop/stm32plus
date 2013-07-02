/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/i2c.h"


#if defined(STM32PLUS_F4)

using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> I2CInterruptFeatureEnabler<3>::FPTR I2CInterruptFeatureEnabler<3>::_forceLinkage=nullptr;
template<> Observable *I2CInterruptFeature<3> ::_i2cInstance=nullptr;


extern "C" {

  #if defined(USE_I2C3_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) I2C3_EV_IRQHandler(void) {

      if(I2C_GetITStatus(I2C3,I2C_IT_TXE)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_ReadyToTransmit,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_TXE);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_RXNE)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_Receive,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_RXNE);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_STOPF)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_StopBitReceived,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_STOPF);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_ADD10)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_10BitHeaderSent,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_ADD10);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_BTF)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_ByteTransferSent,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_BTF);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_ADDR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_AddressSent,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_ADDR);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_SB)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_StartBitSent,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_SB);
      }
    }

    void __attribute__ ((interrupt)) I2C3_ER_IRQHandler(void) {

      if(I2C_GetITStatus(I2C3,I2C_IT_SMBALERT)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_SmbAlert,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_SMBALERT);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_TIMEOUT)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_Timeout,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_TIMEOUT);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_PECERR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_PECError,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_PECERR);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_OVR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_Overrun,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_OVR);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_AF)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_AckFailure,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_AF);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_ARLO)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_ArbitrationLoss,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_ARLO);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_BERR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->notifyObservers(ObservableEvent::I2C_BusError,(void *)3);
        I2C_ClearITPendingBit(I2C3,I2C_IT_BERR);
      }
    }

  #endif
}

#endif // STM32PLUS_F4

