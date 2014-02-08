/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#include "config/stm32plus.h"
#include "config/rtc.h"

// this is only for the F1

#ifdef STM32PLUS_F1


using namespace stm32plus;


RtcAlarmInterruptFeature *RtcAlarmInterruptFeature::_alarmInterruptFeature=nullptr;
RtcSecondInterruptFeature *RtcSecondInterruptFeature::_secondInterruptFeature=nullptr;
RtcOverflowInterruptFeature *RtcOverflowInterruptFeature::_overflowInterruptFeature=nullptr;


#if defined(USE_RTC_INTERRUPT)

extern "C" {

  void __attribute__ ((interrupt("IRQ"))) RTC_IRQHandler(void) {

    // check the ticker

    if(RTC_GetITStatus(RTC_IT_SEC)!=RESET) {
      RtcSecondInterruptFeature::_secondInterruptFeature->RtcSecondInterruptEventSender.raiseEvent();
      RTC_ClearITPendingBit(RTC_FLAG_SEC);
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

    // check the alarm

    else if(RTC_GetITStatus(RTC_IT_ALR)!=RESET) {
      RtcAlarmInterruptFeature::_alarmInterruptFeature->RtcAlarmInterruptEventSender.raiseEvent();
      RTC_ClearITPendingBit(RTC_FLAG_ALR);
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }

    // check the overflow

    else if(::RTC_GetITStatus(RTC_IT_OW)!=RESET) {
      RtcOverflowInterruptFeature::_overflowInterruptFeature->RtcOverflowInterruptEventSender.raiseEvent();
      RTC_ClearITPendingBit(RTC_FLAG_OW);
      __DSB();      // prevent erroneous recall of this handler due to delayed memory write
    }
  }
}

#endif  // USE_RTC_INTERRUPT
#endif  // STM32PLUS_F1
