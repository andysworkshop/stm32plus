/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


/*
 * Forward declare the IRQ handler names
 */

extern "C" void RTC_IRQHandler();


namespace stm32plus {


  /**
   * RTC feature to enable access to the alarm functionality
   */

  DECLARE_EVENT_SIGNATURE(RtcAlarmInterrupt,void());

  class RtcAlarmInterruptFeature : public RtcFeatureBase {

    public:
      static RtcAlarmInterruptFeature *_alarmInterruptFeature;
      void (*_irqRef)();

      DECLARE_EVENT_SOURCE(RtcAlarmInterrupt);

    public:
      RtcAlarmInterruptFeature(RtcBase& rtc);

      void setAlarm(uint32_t alarm) const;
      void cancelAlarm() const;
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcAlarmInterruptFeature::RtcAlarmInterruptFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc) {

    _alarmInterruptFeature=this;
    _irqRef=RTC_IRQHandler;

    Nvic::configureIrq(RTC_IRQn);
  }


  /**
   * Set the alarm.
   * @param[in] alarm The alarm value.
   */

  inline void RtcAlarmInterruptFeature::setAlarm(uint32_t alarm) const {
    RTC_WaitForLastTask();
    RTC_ITConfig(RTC_IT_ALR,ENABLE);
    RTC_SetAlarm(alarm);
    RTC_WaitForLastTask();
  }


  /**
   * Cancel the alarm interupts
   */

  inline void RtcAlarmInterruptFeature::cancelAlarm() const {
    RTC_ITConfig(RTC_IT_ALR,DISABLE);
  }
}
