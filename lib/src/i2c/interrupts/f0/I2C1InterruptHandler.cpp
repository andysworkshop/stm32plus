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

template<> I2CInterruptFeatureEnabler<1>::FPTR I2CInterruptFeatureEnabler<1>::_forceLinkage=nullptr;
template<> I2CEventSource *I2CInterruptFeature<1> ::_i2cInstance=nullptr;


extern "C" {

  #if defined(USE_I2C1_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) I2C1_IRQHandler(void) {

      if(I2C_GetITStatus(I2C1,I2C_IT_TXIS)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_READY_TO_TRANSMIT);
        I2C_ClearITPendingBit(I2C1,I2C_IT_TXIS);
      }
      else if(I2C_GetITStatus(I2C1,I2C_IT_RXNE)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_RECEIVE);
        I2C_ClearITPendingBit(I2C1,I2C_IT_RXNE);
      }
      else if(I2C_GetITStatus(I2C1,I2C_IT_STOPF)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_STOP_BIT_RECEIVED);
        I2C_ClearITPendingBit(I2C1,I2C_IT_STOPF);
      }
      else if(I2C_GetITStatus(I2C1,I2C_IT_ADDR)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ADDRESS_MATCH);
        I2C_ClearITPendingBit(I2C1,I2C_IT_ADDR);
      }
      else if(I2C_GetITStatus(I2C1,I2C_IT_TC | I2C_IT_TCR)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_TRANSFER_COMPLETE);
        I2C_ClearITPendingBit(I2C1,I2C_IT_TC | I2C_IT_TCR);
      }
      else if(I2C_GetITStatus(I2C1,I2C_IT_BERR | I2C_IT_ARLO | I2C_IT_OVR | I2C_IT_PECERR | I2C_IT_TIMEOUT | I2C_IT_ALERT)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ERROR);
        I2C_ClearITPendingBit(I2C1,I2C_IT_BERR | I2C_IT_ARLO | I2C_IT_OVR | I2C_IT_PECERR | I2C_IT_TIMEOUT | I2C_IT_ALERT);
      }
      else if(I2C_GetITStatus(I2C1,I2C_IT_NACKF)!=RESET) {
        I2CInterruptFeature<1>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_NOT_ACKNOWLEDGE);
        I2C_ClearITPendingBit(I2C1,I2C_IT_NACKF);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

  #endif
}

#endif
