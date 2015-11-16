/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"

#include "config/can.h"


using namespace stm32plus;

// static initialisers for the hack that forces the IRQ handlers to be linked

template<> CanInterruptFeatureEnabler<1>::FPTR CanInterruptFeatureEnabler<1>::_forceLinkage=nullptr;
template<> CanEventSource *CanInterruptFeature<1>:: _canInstance=nullptr;


extern "C" {

  #if defined(USE_CAN1_INTERRUPT)

	void __attribute__ ((interrupt("IRQ"))) USB_HP_CAN1_TX_IRQHandler(void) {

		if(CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET){
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_TRANSMIT_MAILBOX_EMPTY);
		}
		__DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}

	void __attribute__ ((interrupt("IRQ"))) USB_LP_CAN1_RX0_IRQHandler(void) {
		if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_MESSAGE_PENDING);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_FF0)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_FULL);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_FOV0)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO0_OVR);
		}
		__DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}

	void __attribute__ ((interrupt("IRQ"))) CAN1_RX1_IRQHandler(void) {
		if(CAN_GetITStatus(CAN1,CAN_IT_FMP1)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_MESSAGE_PENDING);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_FF1)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_FULL);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_FOV1)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_FIFO1_OVR);
		}
		__DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}
	void __attribute__ ((interrupt("IRQ"))) CAN1_SCE_IRQHandler(void) {
		if(CAN_GetITStatus(CAN1,CAN_IT_EWG)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_ERROR_WARNING);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_EPV)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_ERROR_PASSIVE);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_BOF)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_BUS_OFF);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_LEC)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_LAST_ERROR_CODE);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_WKU)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_WAKEUP);
		}
		else if(CAN_GetITStatus(CAN1,CAN_IT_SLK)!=RESET)
		{
			CanInterruptFeature<1>:: _canInstance->CanInterruptEventSender.raiseEvent(CanEventType::EVENT_SLEEP);
		}
		__DSB();      // prevent erroneous recall of this handler due to delayed memory write
	}






#endif
}
