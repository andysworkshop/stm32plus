/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#if defined(STM32PLUS_F4)

#include "config/can.h"


using namespace stm32plus;


// static initialisers for the hack that forces the IRQ handlers to be linked

Can2InterruptFeature::FPTR Can2InterruptFeature::_forceLinkage=nullptr;
CanEventSource *Can2InterruptFeature:: _canInstance=nullptr;


extern "C" {

#if defined(USE_CAN2_INTERRUPT)


  void __attribute__ ((interrupt("IRQ"))) CAN2_TX_IRQHandler(void) {

		if(CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_TRANSMIT_MAILBOX_EMPTY);

		__DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}


	void __attribute__ ((interrupt("IRQ"))) CAN2_RX0_IRQHandler(void) {

	  if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)
	    Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_MESSAGE_PENDING);
		else if(CAN_GetITStatus(CAN2,CAN_IT_FF0)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_FULL);
		else if(CAN_GetITStatus(CAN2,CAN_IT_FOV0)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_OVR);

	  __DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}


	void __attribute__ ((interrupt("IRQ"))) CAN2_RX1_IRQHandler(void) {

	  if(CAN_GetITStatus(CAN2,CAN_IT_FMP1)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_MESSAGE_PENDING);
		else if(CAN_GetITStatus(CAN2,CAN_IT_FF1)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_FULL);
		else if(CAN_GetITStatus(CAN2,CAN_IT_FOV1)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_OVR);

	  __DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}


	void __attribute__ ((interrupt("IRQ"))) CAN2_SCE_IRQHandler(void) {

	  if(CAN_GetITStatus(CAN2,CAN_IT_EWG)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_ERROR_WARNING);
		else if(CAN_GetITStatus(CAN2,CAN_IT_EPV)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_ERROR_PASSIVE);
		else if(CAN_GetITStatus(CAN2,CAN_IT_BOF)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_BUS_OFF);
		else if(CAN_GetITStatus(CAN2,CAN_IT_LEC)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_LAST_ERROR_CODE);
		else if(CAN_GetITStatus(CAN2,CAN_IT_WKU)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_WAKEUP);
		else if(CAN_GetITStatus(CAN2,CAN_IT_SLK)!=RESET)
			Can2InterruptFeature::_canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_SLEEP);

	  __DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}

#endif
}

#endif
