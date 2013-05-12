/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#if defined(STM32PLUS_F1_CL) || defined(STM32PLUS_F4)

#include "config/stm32plus.h"
#include "config/exti.h"


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiInterruptEnabler<19>::FPTR ExtiInterruptEnabler<19>::_forceLinkage=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line19>::_extiInstance=nullptr;


#if defined(USE_EXTI_ETH_WAKEUP_INTERRUPT)

extern "C" {

	void __attribute__ ((interrupt("IRQ"))) ETH_WKUP_IRQHandler(void) {

		if(EXTI_GetITStatus(EXTI_Line19)!=RESET) {
				ExtiEthernetWakeup::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)19);
				EXTI_ClearITPendingBit(EXTI_Line19);
		}
	}
}

#endif
#endif
