/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/exti.h"


using namespace stm32plus;


// static initialiser for the hack that forces the IRQ handlers to be linked

template<> ExtiInterruptEnabler<0>::FPTR ExtiInterruptEnabler<0>::_forceLinkage=nullptr;
template<> Observable *ExtiPeripheral<EXTI_Line0>::_extiInstance=nullptr;


#if defined(USE_EXTI0_INTERRUPT)

extern "C" {

	void __attribute__ ((interrupt("IRQ"))) EXTI0_IRQHandler(void) {

		if(EXTI_GetITStatus(EXTI_Line0)!=RESET) {
				Exti0::_extiInstance->notifyObservers(ObservableEvent::EXTI_Triggered,(void *)0);
				EXTI_ClearITPendingBit(EXTI_Line0);
		}
	}
}

#endif
