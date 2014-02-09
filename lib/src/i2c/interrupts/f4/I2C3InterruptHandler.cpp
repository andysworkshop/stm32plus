/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/i2c.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

template<> I2CInterruptFeatureEnabler<3>::FPTR I2CInterruptFeatureEnabler<3>::_forceLinkage=nullptr;
template<> I2CEventSource *I2CInterruptFeature<3> ::_i2cInstance=nullptr;


extern "C" {

  #if defined(USE_I2C3_INTERRUPT)

    void __attribute__ ((interrupt("IRQ"))) I2C3_EV_IRQHandler(void) {

      if(I2C_GetITStatus(I2C3,I2C_IT_TXE)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_READY_TO_TRANSMIT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_TXE);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_RXNE)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_RECEIVE);
        I2C_ClearITPendingBit(I2C3,I2C_IT_RXNE);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_STOPF)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_STOP_BIT_RECEIVED);
        I2C_ClearITPendingBit(I2C3,I2C_IT_STOPF);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_ADD10)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_10_BIT_HEADER_SENT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_ADD10);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_BTF)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_BYTE_TRANSFER_SENT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_BTF);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_ADDR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ADDRESS_SENT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_ADDR);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_SB)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_START_BIT_SENT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_SB);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

    void __attribute__ ((interrupt("IRQ"))) I2C3_ER_IRQHandler(void) {

      if(I2C_GetITStatus(I2C3,I2C_IT_SMBALERT)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_SMB_ALERT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_SMBALERT);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_TIMEOUT)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_TIMEOUT);
        I2C_ClearITPendingBit(I2C3,I2C_IT_TIMEOUT);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_PECERR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_PEC_ERROR);
        I2C_ClearITPendingBit(I2C3,I2C_IT_PECERR);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_OVR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_OVERRUN);
        I2C_ClearITPendingBit(I2C3,I2C_IT_OVR);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_AF)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ACK_FAILURE);
        I2C_ClearITPendingBit(I2C3,I2C_IT_AF);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_ARLO)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_ARBITRATION_LOSS);
        I2C_ClearITPendingBit(I2C3,I2C_IT_ARLO);
      }
      else if(I2C_GetITStatus(I2C3,I2C_IT_BERR)!=RESET) {
        I2CInterruptFeature<3>::_i2cInstance->I2CInterruptEventSender.raiseEvent(I2CEventType::EVENT_BUS_ERROR);
        I2C_ClearITPendingBit(I2C3,I2C_IT_BERR);
      }
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

  #endif
}

#endif // STM32PLUS_F4

