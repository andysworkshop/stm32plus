/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F0)

#include "config/i2c.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> I2CInterruptFeatureEnabler<2>::FPTR I2CInterruptFeatureEnabler<2>::_forceLinkage=nullptr;
template<> I2CEventSource *I2CInterruptFeature<2> ::_i2cInstance=nullptr;


extern "C" {

  #if defined(USE_I2C2_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) I2C2_IRQHandler(void) {

      if(I2C_GetITStatus(I2C2,I2C_IT_TXI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_READY_TO_TRANSMIT);
        I2C_ClearITPendingBit(I2C2,I2C_IT_TXI);
      }
      else if(I2C_GetITStatus(I2C2,I2C_IT_RXI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_RECEIVE);
        I2C_ClearITPendingBit(I2C2,I2C_IT_RXI);
      }
      else if(I2C_GetITStatus(I2C2,I2C_IT_STOPI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_STOP_BIT_RECEIVED);
        I2C_ClearITPendingBit(I2C2,I2C_IT_STOPI);
      }
      else if(I2C_GetITStatus(I2C2,I2C_IT_ADDRI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ADDRESS_MATCH);
        I2C_ClearITPendingBit(I2C2,I2C_IT_ADDRI);
      }
      else if(I2C_GetITStatus(I2C2,I2C_IT_TCI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_TRANSFER_COMPLETE);
        I2C_ClearITPendingBit(I2C2,I2C_IT_TCI);
      }
      else if(I2C_GetITStatus(I2C2,I2C_IT_ERRI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ERROR);
        I2C_ClearITPendingBit(I2C2,I2C_IT_ERRI);
      }
      else if(I2C_GetITStatus(I2C2,I2C_IT_NACKI)!=RESET) {
        I2CInterruptFeature<2>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_NOT_ACKNOWLEDGE);
        I2C_ClearITPendingBit(I2C2,I2C_IT_NACKI);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

  #endif
}

#endif
