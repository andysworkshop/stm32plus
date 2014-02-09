/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F4
#error This class can only be used with the STM32F4 series
#endif


namespace stm32plus {


  /**
   * RTC feature to enable access to the second
   * interrupt functionality. We do this on the F4 by using the wakeup facility
   */

  class RtcSecondInterruptFeature : public RtcFeatureBase,
                                    public ExtiRtcWakeup {

    public:
      RtcSecondInterruptFeature(RtcBase& rtc);

      void enableSecondInterrupt() const;
      void disableSecondInterrupt() const;
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcSecondInterruptFeature::RtcSecondInterruptFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc),
      ExtiRtcWakeup(EXTI_Mode_Interrupt,EXTI_Trigger_Rising) {

    // set the clock source to the 1Hz SPRE and reset the counter

    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_SetWakeUpCounter(0);
  }


  /**
   * Set the second interrupt
   */

  inline void RtcSecondInterruptFeature::enableSecondInterrupt() const {
    RTC_ITConfig(RTC_IT_WUT,ENABLE);
    RTC_WakeUpCmd(ENABLE);
  }


  /**
   * Cancel the second interupt
   */

  inline void RtcSecondInterruptFeature::disableSecondInterrupt() const {
    RTC_ITConfig(RTC_IT_WUT,DISABLE);
    RTC_WakeUpCmd(DISABLE);
  }
}
