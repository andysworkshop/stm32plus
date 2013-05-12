/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/rtc.h"

// this is only for the F1

#ifdef STM32PLUS_F1


using namespace stm32plus;


Observable *RtcAlarmInterruptFeature::_observableInstance=nullptr;
Observable *RtcSecondInterruptFeature::_observableInstance=nullptr;
Observable *RtcOverflowInterruptFeature::_observableInstance=nullptr;


#if defined(USE_RTC_INTERRUPT)

extern "C" {

	void __attribute__ ((interrupt("IRQ"))) RTC_IRQHandler(void) {

		// check the ticker

		if(RTC_GetITStatus(RTC_IT_SEC)!=RESET) {
			RtcSecondInterruptFeature::_observableInstance->notifyObservers(ObservableEvent::RTC_Tick);
			RTC_ClearITPendingBit(RTC_FLAG_SEC);
		}

		// check the alarm

		else if(RTC_GetITStatus(RTC_IT_ALR)!=RESET) {
			RtcAlarmInterruptFeature::_observableInstance->notifyObservers(ObservableEvent::RTC_Alarm);
			RTC_ClearITPendingBit(RTC_FLAG_ALR);
		}

		// check the overflow

		else if(::RTC_GetITStatus(RTC_IT_OW)!=RESET) {
			RtcOverflowInterruptFeature::_observableInstance->notifyObservers(ObservableEvent::RTC_Overflow);
			RTC_ClearITPendingBit(RTC_FLAG_OW);
		}
	}
}

#endif  // USE_RTC_INTERRUPT
#endif  // STM32PLUS_F1
