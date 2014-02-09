/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013,2014 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once


// ensure the MCU series is correct
#ifndef STM32PLUS_F0
#error This class can only be used with the STM32F0 series
#endif


namespace stm32plus {


  /**
   * RTC feature to enable access to the alarm functionality. The F0 has one alarm
   * that can be configured to trigger on any or all of the time components and optionally
   * the date/weekday.
   */

  class RtcAlarmInterruptFeature : public RtcFeatureBase,
                                   public ExtiRtcAlarm {

    public:
      RtcAlarmInterruptFeature(RtcBase& rtc);

      void setAlarm(uint32_t mask,uint32_t dateOrWeekdaySelector,uint8_t dateOrWeekDayValue,uint8_t hour,uint8_t minute,uint8_t second,uint8_t am_pm=RTC_H12_AM) const;
      void cancelAlarm() const;
  };


  /**
   * Constructor
   * @param rtc The base RTC class
   */

  inline RtcAlarmInterruptFeature::RtcAlarmInterruptFeature(RtcBase& rtc)
    : RtcFeatureBase(rtc),
      ExtiRtcAlarm(EXTI_Mode_Interrupt,EXTI_Trigger_Rising) {
  }


  /**
   * Set the alarm
   * @param mask one of RTC_AlarmMask_Definitions, e.g. RTC_AlarmMask_All
   * @param dateOrWeekdaySelector one of RTC_AlarmDateWeekDay_Definitions. e.g. RTC_AlarmDateWeekDaySel_Date (don't care if date is masked off)
   * @param hour The hour, if not masked off
   * @param minute The minute, if not masked off
   * @param second The second, if not masked off
   * @param dateOrWeekDayValue The date (1..31) or day (1..7) where Monday = 1, if not masked off.
   */

  inline void RtcAlarmInterruptFeature::setAlarm(uint32_t mask,
                                                 uint32_t dateOrWeekdaySelector,
                                                 uint8_t dateOrWeekDayValue,
                                                 uint8_t hour,
                                                 uint8_t minute,
                                                 uint8_t second,
                                                 uint8_t am_pm) const {

    RTC_AlarmTypeDef alarm;

    // alarm register is RO while enabled

    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

    // set up the time component

    alarm.RTC_AlarmTime.RTC_Hours=hour;
    alarm.RTC_AlarmTime.RTC_Minutes=minute;
    alarm.RTC_AlarmTime.RTC_Seconds=second;
    alarm.RTC_AlarmTime.RTC_H12=am_pm;

    // set up the alarm configuration component

    alarm.RTC_AlarmMask=mask;
    alarm.RTC_AlarmDateWeekDaySel=dateOrWeekdaySelector;
    alarm.RTC_AlarmDateWeekDay=dateOrWeekDayValue;

    // set the alarm and enable the interrupt

    RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&alarm);
    RTC_ITConfig(RTC_IT_ALRA,ENABLE);

    // enable the alarm

    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
  }


  /**
   * Cancel the alarm interupts
   */

  inline void RtcAlarmInterruptFeature::cancelAlarm() const {
    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
    RTC_ITConfig(RTC_IT_ALRA,DISABLE);
  }
}
